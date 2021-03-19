//
// Created by depaulsmiller on 3/18/21.
//

#include <Map.hh>
#include <Transaction.hh>
#include <queue>
#include <map>
#include <protocol.pb.h>
#include <future>
#include <atomic>
#include <utility>
#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include <protocol.grpc.pb.h>
#include <spdlog/spdlog.h>

#ifndef SCORE_INTERNALCLIENT_HH
#define SCORE_INTERNALCLIENT_HH

namespace score {
    struct InternalClient {

        explicit InternalClient(const std::string &addr) :
                c(grpc::CreateChannel(addr, grpc::InsecureChannelCredentials())),
                stub_(std::move(CControlRPC::NewStub(c))) {

        }

        InternalClient(InternalClient &&other) {
            this->c = std::move(other.c);
            this->stub_ = std::move(other.stub_);
        }

        ~InternalClient() {
        }

        void DoReadRequest(const ReadRequest &request, ReadReturn *response) {
            SPDLOG_TRACE("Calling stub");

            grpc::ClientContext context;
            stub_->DoReadRequest(&context, request, response);
        }

        void DoPrepare(const Prepare &request, Vote *response) {

            SPDLOG_TRACE("Calling stub");

            grpc::ClientContext context;
            stub_->DoPrepare(&context, request, response);
        }

        void DoDecide(const Decide &request, Committed *response) {
            SPDLOG_TRACE("Calling stub");

            grpc::ClientContext context;
            stub_->DoDecide(&context, request, response);
        }

        std::shared_ptr<grpc::Channel> c;
        std::unique_ptr<CControlRPC::Stub> stub_;
    };
}

#endif //SCORE_INTERNALCLIENT_HH
