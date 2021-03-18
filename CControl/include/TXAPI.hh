//
// Created by depaulsmiller on 3/18/21.
//

#include <Context.hh>
#include <InternalClient.hh>
#include <utility>

#ifndef SCORE_TXAPI_HH
#define SCORE_TXAPI_HH

namespace score {
    class TXAPI {
    public:
        TXAPI(std::shared_ptr<Context> ctx, std::vector<std::shared_ptr<InternalClient>> clients) : ctx_(
                std::move(ctx)),
                                                                                                    c(std::move(
                                                                                                            clients)) {
            std::cerr << "Creating TXAPI" << std::endl;
        }

        ~TXAPI() {
        }

        void StartTx(const Empty &request, TxIDMsg *response) {
            txid_t id = ctx_->txid_counter.fetch_add(1);
            Context::txMapAccessor a;
            ctx_->txMap.insert(a, id);
            a->second.txid = id;
            assert(a->second.firstRead == true);
            response->set_txid(id);
        }

        void Read(const ReadOperation &request, ReadOperationResponse *response) {
            response->set_txid(request.txid());
            response->set_aborted(false);

            // checking if read has been done before
            Context::txMapAccessor a;
            ctx_->txMap.find(a, request.txid());
            auto iter = a->second.ws.find(request.key());
            if (iter != a->second.ws.end()) {
                response->set_txid(request.txid());
                response->set_value(iter->second);
                return;
            }


            if (a->second.firstRead) {
                std::unique_lock<std::mutex> stateLock(ctx_->stateMtx);
                a->second.sid = ctx_->getCommitID(stateLock);
            }
            ReadRequest req;
            req.set_txid(request.txid());
            req.set_readsid(a->second.sid);
            req.set_key(request.key());
            req.set_firstread(a->second.firstRead);
            ReadReturn ret;

            if (ctx_->thisNodeIsAReplica(request.key())) {
                c[ctx_->rank]->DoReadRequest(req, &ret);
            } else {
                auto reps = ctx_->replicas(request.key());
                for (auto &rep : reps) {
                    c[rep]->DoReadRequest(req, &ret);
                    {
                        std::unique_lock<std::mutex> stateLock(ctx_->stateMtx);
                        ctx_->updateNodeTimestamps(ret.lastcommitted(), stateLock);
                    }
                }
            }

            if (a->second.firstRead) {
                a->second.sid = ret.lastcommitted();
            }

            if (a->second.isUpdate && !ret.mostrecent()) {
                a->second.aborted = true;
                response->set_aborted(true);
                auto id = a->second.txid;
                a.release();
                ctx_->txMap.erase(id);
                return;
            }
            a->second.rs[ret.key()] = ret.lastcommitted();
            response->set_value(ret.value());
        }

        void Write(const WriteOperation &request, WriteOperationResponse *response) {
            Context::txMapAccessor a;
            ctx_->txMap.find(a, request.txid());
            a->second.ws[request.key()] = request.value();
            response->set_txid(request.txid());
        }

        void Commit(const TxIDMsg &request, Committed *response) {

            std::cerr << "Starting commit" << std::endl;

            Context::txMapAccessor a;
            ctx_->txMap.find(a, request.txid());
            Transaction tx = a->second;
            a.release();

            if (tx.ws.empty()) {
                response->set_success(true);
                return;
            }

            std::unordered_map<uint64_t, Prepare> prepareMsgs;

            for (auto &elm : tx.rs) {
                auto replicas = ctx_->replicas(elm.first);
                for (auto &node : replicas) {
                    if (prepareMsgs.find(node) == prepareMsgs.end()) {
                        prepareMsgs[node] = Prepare();
                    }
                    auto rsElement = prepareMsgs[node].add_rs();
                    rsElement->set_key(elm.first);
                    rsElement->set_ver(elm.second);
                }
            }

            for (auto &elm : tx.ws) {
                auto replicas = ctx_->replicas(elm.first);
                for (auto &node : replicas) {
                    if (prepareMsgs.find(node) == prepareMsgs.end()) {
                        prepareMsgs[node] = Prepare();
                    }
                    auto rsElement = prepareMsgs[node].add_ws();
                    rsElement->set_key(elm.first);
                    rsElement->set_value(elm.second);
                }
            }

            std::cerr << "Created ws and rs" << std::endl;

            std::unordered_map<uint64_t, Decide> decisions;
            uint64_t maxSid = 0;
            bool proceed = true;

            for (auto &prep : prepareMsgs) {
                prep.second.set_txid(tx.txid);
                prep.second.set_sid(tx.sid);

                Vote v;
                std::cerr << "Voting" << std::endl;

                c[prep.first]->DoPrepare(prep.second, &v);
                decisions[prep.first] = Decide();
                proceed = proceed && v.outcome();
                maxSid = std::max(maxSid, v.sn());
            }

            Committed committed;
            std::cerr << "Checking if committed" << std::endl;

            for (auto &decision : decisions) {
                decision.second.set_txid(tx.txid);
                decision.second.set_fsn(maxSid);
                decision.second.set_outcome(proceed);

                c[decision.first]->DoDecide(decision.second, &committed);
            }

            *response = committed;
        }

    private:
        std::shared_ptr<Context> ctx_;
        std::vector<std::shared_ptr<InternalClient>> c;
    };
}

#endif //SCORE_TXAPI_HH
