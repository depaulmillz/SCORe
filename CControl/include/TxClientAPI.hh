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

        explicit Internals(const std::string &addr);

        ~Internals();

        std::shared_ptr<grpc::Channel> c;
        std::unique_ptr<TxRPC::Stub> stub_;
    };

    class Tx {
    private:
        Tx(uint64_t t, uint64_t n, std::shared_ptr<Internals> i);

    public:

        Tx(const Tx &) = delete;

        Tx(Tx &&) = default;

        ~Tx() = default;

        std::pair<bool, std::string> Read(std::string key);

        bool Write(std::string key, std::string value);

        bool TryCommit();

        friend TxClient;
    private:


        uint64_t txid;
        uint64_t nodeid;
        std::shared_ptr<Internals> internals;
        bool aborted;
    };

    class TxClient {
    public:

        explicit TxClient(const std::string &addr);

        ~TxClient();

        Tx StartTx();

    private:
        std::shared_ptr<Internals> internals;
    };
}

#endif //SCORE_TXCLIENTAPI_HH
