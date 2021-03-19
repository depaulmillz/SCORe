//
// Created by depaulsmiller on 2/11/21.
//

#include <CControlRPC.hh>


namespace score {

    CControlRPCImpl::CControlRPCImpl(std::shared_ptr<CControl> &control) : cc(control) {

    }

    CControlRPCImpl::~CControlRPCImpl() {

    }

    grpc::Status CControlRPCImpl::DoReadRequest(::grpc::ServerContext *context, const ::score::ReadRequest *request,
                                                ::score::ReadReturn *response) {
        SPDLOG_TRACE("In RPC");
        cc->DoReadRequest(*request, response);
        return ::grpc::Status::OK;
    }

    grpc::Status
    CControlRPCImpl::DoPrepare(::grpc::ServerContext *context, const ::score::Prepare *request,
                               ::score::Vote *response) {
        SPDLOG_TRACE("In RPC");

        cc->DoPrepare(*request, response);
        return ::grpc::Status::OK;
    }

    grpc::Status
    CControlRPCImpl::DoDecide(::grpc::ServerContext *context, const ::score::Decide *request,
                              ::score::Committed *response) {
        SPDLOG_TRACE("In RPC");

        cc->DoDecide(*request, response);
        return ::grpc::Status::OK;
    }

    std::unique_ptr<grpc::Server> RunServer(const std::string &server_address, std::shared_ptr<CControl> &cc) {
        CControlRPCImpl *service = new CControlRPCImpl(cc); // TODO fix leak

        grpc::ServerBuilder builder;
        builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
        builder.RegisterService(service);
        std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
        return std::move(server);
    }

}