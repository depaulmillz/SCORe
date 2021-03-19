//
// Created by depaulsmiller on 3/18/21.
//

#include <InternalClient.hh>

namespace score {

    InternalClient::InternalClient(const std::string &addr) :
            c(grpc::CreateChannel(addr, grpc::InsecureChannelCredentials())),
            stub_(std::move(CControlRPC::NewStub(c))) {

    }

    InternalClient::InternalClient(InternalClient &&other) {
        this->c = std::move(other.c);
        this->stub_ = std::move(other.stub_);
    }

    InternalClient::~InternalClient() {
    }

    void InternalClient::DoReadRequest(const ReadRequest &request, ReadReturn *response) {
        SPDLOG_TRACE("Calling stub");

        grpc::ClientContext context;
        stub_->DoReadRequest(&context, request, response);
    }

    void InternalClient::DoPrepare(const Prepare &request, Vote *response) {

        SPDLOG_TRACE("Calling stub");

        grpc::ClientContext context;
        stub_->DoPrepare(&context, request, response);
    }

    void InternalClient::DoDecide(const Decide &request, Committed *response) {
        SPDLOG_TRACE("Calling stub");

        grpc::ClientContext context;
        stub_->DoDecide(&context, request, response);
    }

}