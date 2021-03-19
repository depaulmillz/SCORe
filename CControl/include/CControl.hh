//
// Created by depaulsmiller on 3/18/21.
//

#include <protocol.pb.h>

#ifndef SCORE_CCONTROL_HH
#define SCORE_CCONTROL_HH

namespace score {
    class CControl {
    public:
        CControl(std::shared_ptr<Context> ctx) : ctx_(std::move(ctx)) {

        }

        ~CControl() {
        }

        void DoReadRequest(const ReadRequest &request,
                           ReadReturn *response) {
            version_t newReadSid = request.readsid();

            std::unique_lock<std::mutex> stateLock(ctx_->stateMtx);
            if (request.firstread() && ctx_->getCommitID(stateLock) > newReadSid) {
                newReadSid = ctx_->getCommitID(stateLock);
            }
            stateLock.unlock();

            std::string key = request.key();
            std::tuple<data_t, version_t, bool> p = ctx_->doRead(newReadSid, key);

            stateLock.lock();
            ctx_->updateNodeTimestamps(request.readsid(), stateLock);
            stateLock.unlock();

            response->set_txid(request.txid());
            response->set_nodeid(request.nodeid());
            response->set_key(key);
            response->set_value(std::get<0>(p));
            response->set_mostrecent(std::get<2>(p));
            stateLock.lock();
            response->set_lastcommitted(ctx_->getCommitID(stateLock));
        }

        void DoPrepare(const Prepare &request, Vote *response) {
            //std::cerr << "Running DoPrepare" << std::endl;
            std::map<data_t, bool> toLock;
            for (auto &r : request.ws()) {
                toLock[r.key()] = false;
            }
            for (auto &r : request.rs()) {
                toLock[r.key()] = true;
            }
            version_t sn = 0;
            bool outcome = ctx_->getLocksWithTimeout(toLock);
            if (outcome) {
                for (auto &r : request.rs()) {
                    accessor_t a;
                    ctx_->m.find(a, r.key());
                    if (a->second->l.begin()->second > request.sid()) {
                        outcome = false;
                        break;
                    }
                }
                if (outcome) {
                    std::unique_lock<std::mutex> stateLock(ctx_->stateMtx);

                    ctx_->getNextID(stateLock) += 1;
                    sn = ctx_->getNextID(stateLock);
                    ctx_->getPendQ(stateLock).push_back({request.txid(), sn});
                    stateLock.unlock();
                    // put tx in txMap
                    Context::txMapAccessor a;
                    std::cerr << "Inserting " << request.txid() << "," << request.nodeid() << std::endl;
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
            }
            response->set_txid(request.txid());
            response->set_nodeid(request.nodeid());
            response->set_sn(sn);
            response->set_outcome(outcome);
            //std::cerr << "Returned from DoPrepare" << std::endl;
        }

        void DoDecide(const Decide &request, Committed *response) {
            std::unique_lock<std::mutex> stateLock(ctx_->stateMtx);

            if (request.outcome()) {
                ctx_->getNextID(stateLock) = std::max(ctx_->getNextID(stateLock), request.fsn());
                OpTriple o;
                o.txid = request.txid();
                o.node = request.nodeid();
                o.sn = request.fsn();
                ctx_->getStableQ(stateLock).push(o);
                response->set_success(true);
            }
            for (auto iter = ctx_->getPendQ(stateLock).begin(); iter != ctx_->getPendQ(stateLock).end(); ++iter) {
                if (iter->txid == request.txid()) {
                    ctx_->getPendQ(stateLock).erase(iter);
                    break;
                }
            }
            if (!request.outcome()) {
                std::map<data_t, bool> toLock;
                Context::txMapAccessor a;
                //std::cerr << "Finding " << request.txid() << "," << request.nodeid() << std::endl;
                ctx_->txMap.find(a, {request.txid(), request.nodeid()});

                for (auto &r : a->second.ws) {
                    toLock[r.first] = false;
                }
                for (auto &r : a->second.rs) {
                    toLock[r.first] = true;
                }
                ctx_->releaseLocks(toLock);
                a->second.aborted = true;
                response->set_success(false);
            }
            commitCondition(stateLock);
        }

        void commitCondition(const std::unique_lock<std::mutex> &stateLock) {
            if (!ctx_->getStableQ(stateLock).empty()) {
                version_t fsn = ctx_->getStableQ(stateLock).front().sn;
                // does not exist a pending transaction with a timestamp less than the newest one to be committed
                if (ctx_->getPendQ(stateLock).empty() || (ctx_->getPendQ(stateLock).front().sn >= fsn)) {
                    std::map<data_t, bool> toLock;
                    Context::txMapAccessor a;
                    txid_t txid = ctx_->getStableQ(stateLock).front().txid;
                    uint64_t nodeid = ctx_->getStableQ(stateLock).front().node;
                    //std::cerr << "Finding " << txid << "," << nodeid << std::endl;
                    ctx_->txMap.find(a, {txid, nodeid});

                    for (auto &r : a->second.ws) {
                        accessor_t ma;
                        ctx_->m.find(ma, r.first);
                        ma->second->l.push_front({r.second, fsn});
                        toLock[r.first] = false;
                    }
                    for (auto &r : a->second.rs) {
                        toLock[r.first] = true;
                    }
                    ctx_->releaseLocks(toLock);
                    ctx_->getStableQ(stateLock).pop();
                    a->second.committed = true;
                }
            }
            ctx_->uponCondition(stateLock);
        }

    private:
        std::shared_ptr<Context> ctx_;
    };
}

#endif //SCORE_CCONTROL_HH
