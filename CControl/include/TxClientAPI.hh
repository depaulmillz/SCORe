//
// Created by depaulsmiller on 3/18/21.
//

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include <protocol.grpc.pb.h>

#ifndef SCORE_TXCLIENTAPI_HH
#define SCORE_TXCLIENTAPI_HH

namespace score {

    class TxClient;

    struct Internals {

        explicit Internals(const std::string &addr) :
                c(grpc::CreateChannel(addr, grpc::InsecureChannelCredentials())),
                stub_(std::move(TxRPC::NewStub(c))) {

        }

        ~Internals() {

        }

        std::shared_ptr<grpc::Channel> c;
        std::unique_ptr<TxRPC::Stub> stub_;
    };

    class Tx {
    private:
        Tx(uint64_t t, uint64_t n, std::shared_ptr<Internals> i) : txid(t), nodeid(n), internals(i), aborted(false) {

        }

    public:

        Tx(const Tx &) = delete;

        Tx(Tx &&) = default;

        ~Tx() {

        }

        std::pair<bool, std::string> Read(std::string key) {
            if(aborted){
                return {false, ""};
            }

            grpc::ClientContext context;
            ReadOperation request;

            request.set_txid(txid);
            request.set_nodeid(nodeid);
            request.set_key(key);

            ReadOperationResponse response;
            internals->stub_->Read(&context, request, &response);

            if(response.aborted()){
                return {false, ""};
            }

            return {true, response.value()};
        }

        bool Write(std::string key, std::string value) {
            if(aborted)
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

        bool TryCommit() {
            if(aborted)
                return false;

            grpc::ClientContext context;
            TxIDMsg rq;

            rq.set_txid(txid);
            rq.set_nodeid(nodeid);

            Committed resp;

            internals->stub_->Commit(&context, rq, &resp);

            return resp.success();
        }

        friend TxClient;
    private:


        uint64_t txid;
        uint64_t nodeid;
        std::shared_ptr<Internals> internals;
        bool aborted;
    };

    class TxClient {
    public:

        explicit TxClient(const std::string &addr) : internals(std::make_shared<Internals>(addr)) {

        }

        ~TxClient() {

        }

        Tx StartTx() {
            grpc::ClientContext context;
            Empty e;
            TxIDMsg t;
            internals->stub_->StartTx(&context, e, &t);
            return Tx(t.txid(), t.nodeid(), internals);
        }

    private:
        std::shared_ptr<Internals> internals;
    };
}

#endif //SCORE_TXCLIENTAPI_HH
