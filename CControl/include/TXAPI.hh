//
// Created by depaulsmiller on 3/18/21.
//

#include <Context.hh>
#include <InternalClient.hh>
#include <utility>
#include <protocol.pb.h>
#include <protocol.grpc.pb.h>
#include <grpc/grpc.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <grpcpp/security/server_credentials.h>

#ifndef SCORE_TXAPI_HH
#define SCORE_TXAPI_HH

namespace score {

    class TXAPI {
    public:
        TXAPI(std::shared_ptr<Context> ctx, std::vector<std::shared_ptr<InternalClient>> clients);


        ~TXAPI();

        void StartTx(const Empty &request, TxIDMsg *response);

        void Read(const ReadOperation &request, ReadOperationResponse *response);

        void Write(const WriteOperation &request, WriteOperationResponse *response);

        void Commit(const TxIDMsg &request, Committed *response);

    private:
        std::shared_ptr<Context> ctx_;
        std::vector<std::shared_ptr<InternalClient>> c;
    };

    class TxRPCImpl final : public TxRPC::Service {
    public:
        TxRPCImpl(std::shared_ptr<Context> ctx, std::vector<std::shared_ptr<InternalClient>> clients);

        ~TxRPCImpl() override;

        grpc::Status
        StartTx(::grpc::ServerContext *context, const ::score::Empty *request, ::score::TxIDMsg *response) override;

        grpc::Status Read(::grpc::ServerContext *context, const ::score::ReadOperation *request,
                          ::score::ReadOperationResponse *response) override;

        grpc::Status Write(::grpc::ServerContext *context, const ::score::WriteOperation *request,
                           ::score::WriteOperationResponse *response) override;

        grpc::Status
        Commit(::grpc::ServerContext *context, const ::score::TxIDMsg *request, ::score::Committed *response) override;

    private:
        TXAPI txapi;
    };

    std::unique_ptr<grpc::Server> RunClientSideServer(const std::string &server_address, std::shared_ptr<Context> ctx,
                                                      std::vector<std::shared_ptr<InternalClient>> clients);
}

#endif //SCORE_TXAPI_HH
