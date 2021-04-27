//
// Created by depaulsmiller on 3/18/21.
//

#include <TxClientAPI.hh>
#include <spdlog/spdlog.h>

namespace score {

    Internals::Internals(const std::string &addr) :
            c(grpc::CreateChannel(addr, grpc::InsecureChannelCredentials())),
            stub_(std::move(TxRPC::NewStub(c))) {

    }

    Internals::~Internals() {

    }

    Tx::Tx(uint64_t t, uint64_t n, std::shared_ptr<Internals> i) : txid(t), nodeid(n), internals(i), aborted(false) {

    }
    
    std::pair<bool, std::string> Tx::Read(std::string key) {
        SPDLOG_TRACE("Reading {} for TX {}", key, txid);

        if (aborted) {
            return {false, ""};
        }

        grpc::ClientContext context;
        ReadOperation request;

        request.set_txid(txid);
        request.set_nodeid(nodeid);
        request.set_key(key);

        ReadOperationResponse response;
        internals->stub_->Read(&context, request, &response);

        if (response.aborted()) {
            return {false, ""};
        }

        return {true, response.value()};
    }

    bool Tx::Write(std::string key, std::string value) {
        SPDLOG_TRACE("Writing {} for TX {}", key, txid);

        if (aborted)
            return false;

        grpc::ClientContext context;
        WriteOperation w;
        WriteOperationResponse r;

        w.set_txid(txid);
        w.set_nodeid(nodeid);
        w.set_key(key);
        w.set_value(value);

        internals->stub_->Write(&context, w, &r);

        return true;
    }

    bool Tx::TryCommit() {
        SPDLOG_TRACE("Trying to commit to TX {}", txid);

        if (aborted)
            return false;

        grpc::ClientContext context;
        TxIDMsg rq;

        rq.set_txid(txid);
        rq.set_nodeid(nodeid);

        Committed resp;

        internals->stub_->Commit(&context, rq, &resp);

        return resp.success();
    }

    TxClient::TxClient(const std::string &addr) : internals(std::make_shared<Internals>(addr)) {

    }

    TxClient::~TxClient() {

    }

    Tx TxClient::StartTx() {
        grpc::ClientContext context;
        Empty e;
        TxIDMsg t;
        internals->stub_->StartTx(&context, e, &t);
        return Tx(t.txid(), t.nodeid(), internals);
    }
}