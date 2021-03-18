//
// Created by depaulsmiller on 3/18/21.
//

#include "InternalClient.hh"
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <grpcpp/security/server_credentials.h>

using namespace score;

class CControlRPCImpl2 final : public CControlRPC::Service {
public:
    CControlRPCImpl2() {

    }

    ~CControlRPCImpl2() override {

    }

    grpc::Status DoReadRequest(::grpc::ServerContext *context, const ::score::ReadRequest *request,
                               ::score::ReadReturn *response) override {
        std::cerr << "In RPC " << __FUNCTION__ << std::endl;
        response->set_txid(1);
        response->set_value("hi");
        response->set_key("hi");
        // ...
        return ::grpc::Status::OK;
    }

    grpc::Status
    DoPrepare(::grpc::ServerContext *context, const ::score::Prepare *request, ::score::Vote *response) override {
        std::cerr << "In RPC " << __FUNCTION__ << std::endl;
        response->set_outcome(false);
        response->set_txid(1);
        return ::grpc::Status::OK;
    }

    grpc::Status
    DoDecide(::grpc::ServerContext *context, const ::score::Decide *request,
             ::score::Committed *response) override {
        std::cerr << "In RPC " << __FUNCTION__ << std::endl;
        response->set_success(true);
        return ::grpc::Status::OK;
    }
};

std::unique_ptr<grpc::Server> RunServer2(const std::string &server_address) {
    CControlRPCImpl2* service = new CControlRPCImpl2();
    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(service);
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    return std::move(server);
}

int main(){

    std::string addr = "127.0.0.1:8080";

    auto server = RunServer2(addr);

    auto f = std::async([&]() {
        server->Wait();
    });

    InternalClient c("127.0.0.1:8080");
    Decide d;
    d.set_txid(1);
    d.set_outcome(false);
    d.set_fsn(1);
    Committed r;
    c.DoDecide(d, &r);
    server->Shutdown();
    f.get();
    return 0;
}