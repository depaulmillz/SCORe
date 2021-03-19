//
// Created by depaulsmiller on 2/11/21.
//

#include <CControlAPI.hh>
#include <cassert>
#include <CControl.hh>
#include <protocol.pb.h>
#include <protocol.grpc.pb.h>
#include <grpc/grpc.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <grpcpp/security/server_credentials.h>
#include <spdlog/spdlog.h>

#ifndef SCORE_CCONTROLRPC_HH
#define SCORE_CCONTROLRPC_HH

namespace score {

    class CControlRPCImpl final : public CControlRPC::Service {
    public:
        CControlRPCImpl(std::shared_ptr<CControl> &control) : cc(control) {

        }

        virtual ~CControlRPCImpl() {

        }

        grpc::Status DoReadRequest(::grpc::ServerContext *context, const ::score::ReadRequest *request,
                                   ::score::ReadReturn *response) override {
            SPDLOG_TRACE("In RPC");
            cc->DoReadRequest(*request, response);
            return ::grpc::Status::OK;
        }

        grpc::Status
        DoPrepare(::grpc::ServerContext *context, const ::score::Prepare *request, ::score::Vote *response) override {
            SPDLOG_TRACE("In RPC");

            cc->DoPrepare(*request, response);
            return ::grpc::Status::OK;
        }

        grpc::Status
        DoDecide(::grpc::ServerContext *context, const ::score::Decide *request,
                 ::score::Committed *response) override {
            SPDLOG_TRACE("In RPC");

            cc->DoDecide(*request, response);
            return ::grpc::Status::OK;
        }

    private:
        std::shared_ptr<CControl> cc;
    };

    std::unique_ptr<grpc::Server> RunServer(const std::string &server_address, std::shared_ptr<CControl> &cc) {
        CControlRPCImpl* service = new CControlRPCImpl(cc); // TODO fix leak

        grpc::ServerBuilder builder;
        builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
        builder.RegisterService(service);
        std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
        return std::move(server);
    }

}

#endif //SCORE_CCONTROLRPC_HH
