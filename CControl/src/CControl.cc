//
// Created by depaulsmiller on 3/18/21.
//

#include <CControl.hh>
#include <spdlog/spdlog.h>

namespace score {

    CControl::CControl(std::shared_ptr<Context> ctx) : ctx_(std::move(ctx)) {

    }

    CControl::~CControl() {
    }

    void CControl::DoReadRequest(const ReadRequest &request,
                                 ReadReturn *response) {
        if (!cds::threading::Manager::isThreadAttached())
            cds::threading::Manager::attachThread();

        SPDLOG_TRACE("{}", __FUNCTION__);
        version_t newReadSid = request.readsid();

        SPDLOG_TRACE("Grabbing state lock {}", __FUNCTION__);
        SPDLOG_TRACE("Grabbed state lock {}", __FUNCTION__);
        if (request.firstread() && ctx_->getCommitID() > newReadSid) {
            newReadSid = ctx_->getCommitID();
        }
        SPDLOG_TRACE("Unlocked state lock {}", __FUNCTION__);

        std::string key = request.key();
        SPDLOG_TRACE("Grabbing state lock {}", __FUNCTION__);

        std::tuple<data_t, version_t, bool> p = ctx_->doRead(newReadSid, key);
        std::unique_lock<std::mutex> stateLock(ctx_->stateMtx);
        SPDLOG_TRACE("Grabbed state lock {}", __FUNCTION__);
        ctx_->getLogFile(stateLock) << request.txid() << "," << request.nodeid() << "," << newReadSid << ",r," << key
                                    << "," << std::get<0>(p) << std::endl;
        stateLock.unlock();
        SPDLOG_TRACE("Unlocked state lock {}", __FUNCTION__);
        ctx_->updateNodeTimestamps(request.readsid());

        response->set_txid(request.txid());
        response->set_nodeid(request.nodeid());
        response->set_key(key);
        response->set_value(std::get<0>(p));
        response->set_mostrecent(std::get<2>(p));
        response->set_lastcommitted(std::min((unsigned long)newReadSid, (unsigned long)ctx_->getCommitID()));
        SPDLOG_TRACE("Completed {}", __FUNCTION__);
    }

    void CControl::DoPrepare(const Prepare &request, Vote *response) {
        if (!cds::threading::Manager::isThreadAttached())
            cds::threading::Manager::attachThread();

        SPDLOG_TRACE("Preparing {}, {}", request.txid(), request.nodeid());
        Context::txMapAccessor txa;
        ctx_->txMap.insert(txa, {request.txid(), request.nodeid()});
        txa->second = Transaction{};
        SPDLOG_TRACE("Aware of TX ({},{})", request.txid(), request.nodeid());
        txa.release();

        //std::cerr << "Running DoPrepare" << std::endl;
        std::map<data_t, bool> toLock;
        for (auto &r : request.rs()) {
            toLock[r.key()] = true;
        }
        for (auto &r : request.ws()) {
            SPDLOG_DEBUG("Writing {}", r.key());
            toLock[r.key()] = false;
        }
        version_t sn = 0;
        bool outcome = ctx_->getLocksWithTimeout(toLock);
        if (outcome) {
            for (auto &r : request.rs()) {

                ctx_->m.find(r.key(), [&](map_t::value_type &item) {
                    SPDLOG_TRACE("Validating {} > {} for key {}", item.second->l.front().second, request.sid(), r.key());
                    if (item.second->l.front().second > request.sid()) {
                        outcome = false;
                    }
                });

                if (!outcome) {
                    ctx_->releaseLocks(toLock);
                    SPDLOG_DEBUG("Abort {}, {}", request.txid(), request.nodeid());
                    Context::txMapAccessor txa2;
                    ctx_->txMap.insert(txa2, {request.txid(), request.nodeid()});
                    txa2->second.aborted = true;
                    break;
                }
            }
            if (outcome) {

                std::unique_lock<std::mutex> stateLock(ctx_->stateMtx);
                SPDLOG_TRACE("Got statelock");

                sn = ctx_->getNextID().fetch_add(1) + 1;
                ctx_->getLogFile(stateLock) << request.txid() << "," << request.nodeid() << "," << sn
                                            << ",u,x,x" << std::endl;

                ctx_->getPendQ(stateLock).push_back({request.txid(), sn});
                stateLock.unlock();
                // put tx in txMap
                Context::txMapAccessor a;
                ctx_->txMap.insert(a, {request.txid(), request.nodeid()});

                Transaction tx;

                tx.txid = request.txid();
                tx.sid = sn;

                for (auto &r : request.ws()) {
                    tx.ws[r.key()] = r.value();
                }

                for (auto &r : request.rs()) {
                    tx.rs[r.key()] = r.ver();
                }

                a->second = tx;
            }
        } else {
            SPDLOG_DEBUG("Abort {}, {}", request.txid(), request.nodeid());
            Context::txMapAccessor txa2;
            ctx_->txMap.insert(txa2, {request.txid(), request.nodeid()});
            txa2->second.aborted = true;
        }
        response->set_txid(request.txid());
        response->set_nodeid(request.nodeid());
        response->set_sn(sn);
        response->set_outcome(outcome);
        //std::cerr << "Returned from DoPrepare" << std::endl;
    }

    void CControl::DoDecide(const Decide &request, Committed *response) {
        if (!cds::threading::Manager::isThreadAttached())
            cds::threading::Manager::attachThread();

        SPDLOG_TRACE("Decided {} for TX {}", request.outcome() ? "commit" : "abort", request.txid());

        SPDLOG_TRACE("Grabbing statelock");
        std::unique_lock<std::mutex> stateLock(ctx_->stateMtx);
        SPDLOG_TRACE("Got statelock");

        if (request.outcome()) {

            auto expected = ctx_->getNextID().load();
            while(!ctx_->getNextID().compare_exchange_strong(expected, std::max(expected, request.fsn()))) {
                expected = ctx_->getNextID().load();
            }

            OpTriple o;
            o.txid = request.txid();
            o.node = request.nodeid();
            o.sn = request.fsn();
            ctx_->getStableQ(stateLock).push(o);
            response->set_success(true);
            SPDLOG_DEBUG("Will commit {}, {}", request.txid(), request.nodeid());
            Context::txMapAccessor txa;
            ctx_->txMap.insert(txa, {request.txid(), request.nodeid()});
            if (!txa.empty()) {
                for (auto &elm : txa->second.ws) {
                    ctx_->getLogFile(stateLock) << request.txid() << "," << request.nodeid() << "," << request.fsn()
                                                << ",w," << elm.first << "," << elm.second << std::endl;
                }
            }
            ctx_->getLogFile(stateLock) << request.txid() << "," << request.nodeid() << "," << request.fsn()
                                        << ",c,x,x" << std::endl;

        }
        for (auto iter = ctx_->getPendQ(stateLock).begin(); iter != ctx_->getPendQ(stateLock).end(); ++iter) {
            if (iter->txid == request.txid()) {
                ctx_->getPendQ(stateLock).erase(iter);
                break;
            }
        }
        if (!request.outcome()) {

            ctx_->getLogFile(stateLock) << request.txid() << "," << request.nodeid() << "," << request.fsn()
                                        << ",a,x,x" << std::endl;

            std::map<data_t, bool> toLock;
            SPDLOG_TRACE("Accessing tx map");
            Context::txMapAccessor a;
            //std::cerr << "Finding " << request.txid() << "," << request.nodeid() << std::endl;
            ctx_->txMap.find(a, {request.txid(), request.nodeid()});

            assert(!a.empty());

            if (!a->second.aborted) {
                for (auto &r : a->second.rs) {
                    toLock[r.first] = true;
                }
                for (auto &r : a->second.ws) {
                    toLock[r.first] = false;
                }
                ctx_->releaseLocks(toLock);
            }
            a->second.aborted = true;
            response->set_success(false);
        }
        commitCondition(stateLock);
    }

    void CControl::commitCondition(const std::unique_lock<std::mutex> &stateLock) {
        SPDLOG_TRACE("In commit condition");
        while (!ctx_->getStableQ(stateLock).empty()) {
            version_t fsn = ctx_->getStableQ(stateLock).front().sn;
            SPDLOG_DEBUG("Pending commit with fsn {} and txid {}", fsn, ctx_->getStableQ(stateLock).front().txid);
            // does not exist a pending transaction with a timestamp less than the newest one to be committed
            if (ctx_->getPendQ(stateLock).empty() || (ctx_->getPendQ(stateLock).front().sn >= fsn)) {
                std::map<data_t, bool> toLock;
                Context::txMapAccessor a;
                txid_t txid = ctx_->getStableQ(stateLock).front().txid;
                uint64_t nodeid = ctx_->getStableQ(stateLock).front().node;
                //std::cerr << "Finding " << txid << "," << nodeid << std::endl;
                ctx_->txMap.find(a, {txid, nodeid});

                for (auto &r : a->second.rs) {
                    toLock[r.first] = true;
                }
                for (auto &r : a->second.ws) {
                    ctx_->m.find(r.first, [&](map_t::value_type &item) {
                        item.second->l.push_front({r.second, fsn});
                        SPDLOG_DEBUG("Updated {}->{}", r.first, r.second);
                        toLock[r.first] = false;
                    });
                }
                ctx_->releaseLocks(toLock);
                ctx_->getStableQ(stateLock).pop();
                a->second.committed = true;
                while (fsn > ctx_->getMaxSeen()) {
                    auto expected = ctx_->getMaxSeen().load();
                    ctx_->getMaxSeen().compare_exchange_strong(expected, fsn);
                }
                SPDLOG_DEBUG("Committed TX {} with timestamp {}", a->second.txid, a->second.sid);
            } else {
                SPDLOG_DEBUG("Unable to apply: {} not >= {}", ctx_->getPendQ(stateLock).front().sn, fsn);
                break;
            }
        }
        ctx_->uponCondition();
    }

}
