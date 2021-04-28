//
// Created by depaulsmiller on 3/18/21.
//

#include <TXAPI.hh>
#include <spdlog/spdlog.h>

namespace score {

    TXAPI::TXAPI(std::shared_ptr<Context> ctx, std::vector<std::shared_ptr<InternalClient>> clients) : ctx_(
            std::move(ctx)),
                                                                                                       c(std::move(
                                                                                                               clients)) {
        std::cerr << "Creating TXAPI" << std::endl;
    }

    TXAPI::~TXAPI() {
    }

    void TXAPI::StartTx(const Empty &request, TxIDMsg *response) {
        SPDLOG_TRACE("In TXAPI");

        txid_t id = ctx_->txid_counter.fetch_add(1);
        Context::txMapAccessor a;
        ctx_->txMap.insert(a, {id, ctx_->rank});
        SPDLOG_TRACE("Aware of TX ({},{})", id, ctx_->rank);

        a->second.txid = id;
        assert(a->second.firstRead == true);
        response->set_txid(id);
        response->set_nodeid(ctx_->rank);
    }

    void TXAPI::Read(const ReadOperation &request, ReadOperationResponse *response) {
        SPDLOG_TRACE("In TXAPI");

        assert(request.nodeid() == ctx_->rank);

        response->set_txid(request.txid());
        response->set_nodeid(ctx_->rank);
        response->set_aborted(false);

        // checking if read has been done before
        Context::txMapAccessor a;
        ctx_->txMap.find(a, {request.txid(), ctx_->rank});
        auto iter = a->second.ws.find(request.key());
        if (iter != a->second.ws.end()) {
            response->set_value(iter->second);
            return;
        }


        if (a->second.firstRead) {
            std::unique_lock<std::mutex> stateLock(ctx_->stateMtx);
            a->second.sid = ctx_->getCommitID(stateLock);
            SPDLOG_TRACE("SET SID to {} for TX {}", a->second.sid, request.txid());
        }
        ReadRequest req;
        req.set_txid(request.txid());
        req.set_nodeid(ctx_->rank);
        req.set_readsid(a->second.sid);
        req.set_key(request.key());
        req.set_firstread(a->second.firstRead);
        a.release(); // to prevent deadlock
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

        ctx_->txMap.find(a, {request.txid(), ctx_->rank});

        if (a->second.firstRead) {
            a->second.sid = ret.lastcommitted();
        }

        if (a->second.isUpdate && !ret.mostrecent()) {
            a->second.aborted = true;
            response->set_aborted(true);
            auto id = a->second.txid;
            a.release();
            ctx_->txMap.erase({request.txid(), ctx_->rank});
            return;
        }
        a->second.rs[ret.key()] = ret.lastcommitted();
        response->set_value(ret.value());
    }

    void TXAPI::Write(const WriteOperation &request, WriteOperationResponse *response) {
        SPDLOG_TRACE("In TXAPI");

        Context::txMapAccessor a;
        ctx_->txMap.find(a, {request.txid(), ctx_->rank});
        a->second.ws[request.key()] = request.value();
        response->set_txid(request.txid());
        response->set_nodeid(request.txid());
    }

    void TXAPI::Commit(const TxIDMsg &request, Committed *response) {
        SPDLOG_TRACE("Starting commit");

        Context::txMapAccessor a;
        ctx_->txMap.find(a, {request.txid(), ctx_->rank});
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

        std::unordered_map<uint64_t, Decide> decisions;
        uint64_t maxSid = 0;
        bool proceed = true;

        for (auto &prep : prepareMsgs) {
            prep.second.set_txid(tx.txid);
            prep.second.set_nodeid(request.nodeid());
            prep.second.set_sid(tx.sid);

            Vote v;

            c[prep.first]->DoPrepare(prep.second, &v);
            decisions[prep.first] = Decide();
            proceed = proceed && v.outcome();
            maxSid = std::max(maxSid, v.sn());
        }

        Committed committed;

        for (auto &decision : decisions) {
            decision.second.set_txid(tx.txid);
            decision.second.set_nodeid(request.nodeid());
            decision.second.set_fsn(maxSid);
            decision.second.set_outcome(proceed);

            c[decision.first]->DoDecide(decision.second, &committed);
        }

        *response = committed;
    }


    TxRPCImpl::TxRPCImpl(std::shared_ptr<Context> ctx, std::vector<std::shared_ptr<InternalClient>> clients) : txapi(
            ctx,
            clients) {

    }

    TxRPCImpl::~TxRPCImpl() {

    }

    grpc::Status
    TxRPCImpl::StartTx(::grpc::ServerContext *context, const ::score::Empty *request, ::score::TxIDMsg *response) {
        txapi.StartTx(*request, response);
        return grpc::Status::OK;
    }

    grpc::Status TxRPCImpl::Read(::grpc::ServerContext *context, const ::score::ReadOperation *request,
                                 ::score::ReadOperationResponse *response) {
        txapi.Read(*request, response);

        return grpc::Status::OK;

    }

    grpc::Status TxRPCImpl::Write(::grpc::ServerContext *context, const ::score::WriteOperation *request,
                                  ::score::WriteOperationResponse *response) {
        txapi.Write(*request, response);

        return grpc::Status::OK;

    }

    grpc::Status
    TxRPCImpl::Commit(::grpc::ServerContext *context, const ::score::TxIDMsg *request, ::score::Committed *response) {
        txapi.Commit(*request, response);
        return grpc::Status::OK;
    }

    std::unique_ptr<grpc::Server> RunClientSideServer(const std::string &server_address, std::shared_ptr<Context> ctx,
                                                      std::vector<std::shared_ptr<InternalClient>> clients) {
        auto *service = new TxRPCImpl(std::move(ctx), std::move(clients)); // TODO fix leak

        grpc::ServerBuilder builder;
        builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
        builder.RegisterService(service);
        std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
        return std::move(server);
    }

}
