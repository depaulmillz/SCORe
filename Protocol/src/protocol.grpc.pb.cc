// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: protocol.proto

#include "protocol.pb.h"
#include "protocol.grpc.pb.h"

#include <functional>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/channel_interface.h>
#include <grpcpp/impl/codegen/client_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/message_allocator.h>
#include <grpcpp/impl/codegen/method_handler.h>
#include <grpcpp/impl/codegen/rpc_service_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/server_callback_handlers.h>
#include <grpcpp/impl/codegen/server_context.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/sync_stream.h>
namespace score {

static const char* TxRPC_method_names[] = {
  "/score.TxRPC/StartTx",
  "/score.TxRPC/Read",
  "/score.TxRPC/Write",
  "/score.TxRPC/Commit",
};

std::unique_ptr< TxRPC::Stub> TxRPC::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< TxRPC::Stub> stub(new TxRPC::Stub(channel));
  return stub;
}

TxRPC::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel)
  : channel_(channel), rpcmethod_StartTx_(TxRPC_method_names[0], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_Read_(TxRPC_method_names[1], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_Write_(TxRPC_method_names[2], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_Commit_(TxRPC_method_names[3], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status TxRPC::Stub::StartTx(::grpc::ClientContext* context, const ::score::Empty& request, ::score::TxIDMsg* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_StartTx_, context, request, response);
}

void TxRPC::Stub::experimental_async::StartTx(::grpc::ClientContext* context, const ::score::Empty* request, ::score::TxIDMsg* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_StartTx_, context, request, response, std::move(f));
}

void TxRPC::Stub::experimental_async::StartTx(::grpc::ClientContext* context, const ::score::Empty* request, ::score::TxIDMsg* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_StartTx_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::score::TxIDMsg>* TxRPC::Stub::PrepareAsyncStartTxRaw(::grpc::ClientContext* context, const ::score::Empty& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::score::TxIDMsg>::Create(channel_.get(), cq, rpcmethod_StartTx_, context, request, false);
}

::grpc::ClientAsyncResponseReader< ::score::TxIDMsg>* TxRPC::Stub::AsyncStartTxRaw(::grpc::ClientContext* context, const ::score::Empty& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncStartTxRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status TxRPC::Stub::Read(::grpc::ClientContext* context, const ::score::ReadOperation& request, ::score::ReadOperationResponse* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_Read_, context, request, response);
}

void TxRPC::Stub::experimental_async::Read(::grpc::ClientContext* context, const ::score::ReadOperation* request, ::score::ReadOperationResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_Read_, context, request, response, std::move(f));
}

void TxRPC::Stub::experimental_async::Read(::grpc::ClientContext* context, const ::score::ReadOperation* request, ::score::ReadOperationResponse* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_Read_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::score::ReadOperationResponse>* TxRPC::Stub::PrepareAsyncReadRaw(::grpc::ClientContext* context, const ::score::ReadOperation& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::score::ReadOperationResponse>::Create(channel_.get(), cq, rpcmethod_Read_, context, request, false);
}

::grpc::ClientAsyncResponseReader< ::score::ReadOperationResponse>* TxRPC::Stub::AsyncReadRaw(::grpc::ClientContext* context, const ::score::ReadOperation& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncReadRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status TxRPC::Stub::Write(::grpc::ClientContext* context, const ::score::WriteOperation& request, ::score::WriteOperationResponse* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_Write_, context, request, response);
}

void TxRPC::Stub::experimental_async::Write(::grpc::ClientContext* context, const ::score::WriteOperation* request, ::score::WriteOperationResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_Write_, context, request, response, std::move(f));
}

void TxRPC::Stub::experimental_async::Write(::grpc::ClientContext* context, const ::score::WriteOperation* request, ::score::WriteOperationResponse* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_Write_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::score::WriteOperationResponse>* TxRPC::Stub::PrepareAsyncWriteRaw(::grpc::ClientContext* context, const ::score::WriteOperation& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::score::WriteOperationResponse>::Create(channel_.get(), cq, rpcmethod_Write_, context, request, false);
}

::grpc::ClientAsyncResponseReader< ::score::WriteOperationResponse>* TxRPC::Stub::AsyncWriteRaw(::grpc::ClientContext* context, const ::score::WriteOperation& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncWriteRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status TxRPC::Stub::Commit(::grpc::ClientContext* context, const ::score::TxIDMsg& request, ::score::Committed* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_Commit_, context, request, response);
}

void TxRPC::Stub::experimental_async::Commit(::grpc::ClientContext* context, const ::score::TxIDMsg* request, ::score::Committed* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_Commit_, context, request, response, std::move(f));
}

void TxRPC::Stub::experimental_async::Commit(::grpc::ClientContext* context, const ::score::TxIDMsg* request, ::score::Committed* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_Commit_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::score::Committed>* TxRPC::Stub::PrepareAsyncCommitRaw(::grpc::ClientContext* context, const ::score::TxIDMsg& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::score::Committed>::Create(channel_.get(), cq, rpcmethod_Commit_, context, request, false);
}

::grpc::ClientAsyncResponseReader< ::score::Committed>* TxRPC::Stub::AsyncCommitRaw(::grpc::ClientContext* context, const ::score::TxIDMsg& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncCommitRaw(context, request, cq);
  result->StartCall();
  return result;
}

TxRPC::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      TxRPC_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< TxRPC::Service, ::score::Empty, ::score::TxIDMsg>(
          [](TxRPC::Service* service,
             ::grpc::ServerContext* ctx,
             const ::score::Empty* req,
             ::score::TxIDMsg* resp) {
               return service->StartTx(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      TxRPC_method_names[1],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< TxRPC::Service, ::score::ReadOperation, ::score::ReadOperationResponse>(
          [](TxRPC::Service* service,
             ::grpc::ServerContext* ctx,
             const ::score::ReadOperation* req,
             ::score::ReadOperationResponse* resp) {
               return service->Read(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      TxRPC_method_names[2],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< TxRPC::Service, ::score::WriteOperation, ::score::WriteOperationResponse>(
          [](TxRPC::Service* service,
             ::grpc::ServerContext* ctx,
             const ::score::WriteOperation* req,
             ::score::WriteOperationResponse* resp) {
               return service->Write(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      TxRPC_method_names[3],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< TxRPC::Service, ::score::TxIDMsg, ::score::Committed>(
          [](TxRPC::Service* service,
             ::grpc::ServerContext* ctx,
             const ::score::TxIDMsg* req,
             ::score::Committed* resp) {
               return service->Commit(ctx, req, resp);
             }, this)));
}

TxRPC::Service::~Service() {
}

::grpc::Status TxRPC::Service::StartTx(::grpc::ServerContext* context, const ::score::Empty* request, ::score::TxIDMsg* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status TxRPC::Service::Read(::grpc::ServerContext* context, const ::score::ReadOperation* request, ::score::ReadOperationResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status TxRPC::Service::Write(::grpc::ServerContext* context, const ::score::WriteOperation* request, ::score::WriteOperationResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status TxRPC::Service::Commit(::grpc::ServerContext* context, const ::score::TxIDMsg* request, ::score::Committed* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


static const char* CControlRPC_method_names[] = {
  "/score.CControlRPC/DoReadRequest",
  "/score.CControlRPC/DoPrepare",
  "/score.CControlRPC/DoDecide",
};

std::unique_ptr< CControlRPC::Stub> CControlRPC::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< CControlRPC::Stub> stub(new CControlRPC::Stub(channel));
  return stub;
}

CControlRPC::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel)
  : channel_(channel), rpcmethod_DoReadRequest_(CControlRPC_method_names[0], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_DoPrepare_(CControlRPC_method_names[1], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_DoDecide_(CControlRPC_method_names[2], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status CControlRPC::Stub::DoReadRequest(::grpc::ClientContext* context, const ::score::ReadRequest& request, ::score::ReadReturn* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_DoReadRequest_, context, request, response);
}

void CControlRPC::Stub::experimental_async::DoReadRequest(::grpc::ClientContext* context, const ::score::ReadRequest* request, ::score::ReadReturn* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_DoReadRequest_, context, request, response, std::move(f));
}

void CControlRPC::Stub::experimental_async::DoReadRequest(::grpc::ClientContext* context, const ::score::ReadRequest* request, ::score::ReadReturn* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_DoReadRequest_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::score::ReadReturn>* CControlRPC::Stub::PrepareAsyncDoReadRequestRaw(::grpc::ClientContext* context, const ::score::ReadRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::score::ReadReturn>::Create(channel_.get(), cq, rpcmethod_DoReadRequest_, context, request, false);
}

::grpc::ClientAsyncResponseReader< ::score::ReadReturn>* CControlRPC::Stub::AsyncDoReadRequestRaw(::grpc::ClientContext* context, const ::score::ReadRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncDoReadRequestRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status CControlRPC::Stub::DoPrepare(::grpc::ClientContext* context, const ::score::Prepare& request, ::score::Vote* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_DoPrepare_, context, request, response);
}

void CControlRPC::Stub::experimental_async::DoPrepare(::grpc::ClientContext* context, const ::score::Prepare* request, ::score::Vote* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_DoPrepare_, context, request, response, std::move(f));
}

void CControlRPC::Stub::experimental_async::DoPrepare(::grpc::ClientContext* context, const ::score::Prepare* request, ::score::Vote* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_DoPrepare_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::score::Vote>* CControlRPC::Stub::PrepareAsyncDoPrepareRaw(::grpc::ClientContext* context, const ::score::Prepare& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::score::Vote>::Create(channel_.get(), cq, rpcmethod_DoPrepare_, context, request, false);
}

::grpc::ClientAsyncResponseReader< ::score::Vote>* CControlRPC::Stub::AsyncDoPrepareRaw(::grpc::ClientContext* context, const ::score::Prepare& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncDoPrepareRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status CControlRPC::Stub::DoDecide(::grpc::ClientContext* context, const ::score::Decide& request, ::score::Committed* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_DoDecide_, context, request, response);
}

void CControlRPC::Stub::experimental_async::DoDecide(::grpc::ClientContext* context, const ::score::Decide* request, ::score::Committed* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_DoDecide_, context, request, response, std::move(f));
}

void CControlRPC::Stub::experimental_async::DoDecide(::grpc::ClientContext* context, const ::score::Decide* request, ::score::Committed* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_DoDecide_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::score::Committed>* CControlRPC::Stub::PrepareAsyncDoDecideRaw(::grpc::ClientContext* context, const ::score::Decide& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::score::Committed>::Create(channel_.get(), cq, rpcmethod_DoDecide_, context, request, false);
}

::grpc::ClientAsyncResponseReader< ::score::Committed>* CControlRPC::Stub::AsyncDoDecideRaw(::grpc::ClientContext* context, const ::score::Decide& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncDoDecideRaw(context, request, cq);
  result->StartCall();
  return result;
}

CControlRPC::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      CControlRPC_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< CControlRPC::Service, ::score::ReadRequest, ::score::ReadReturn>(
          [](CControlRPC::Service* service,
             ::grpc::ServerContext* ctx,
             const ::score::ReadRequest* req,
             ::score::ReadReturn* resp) {
               return service->DoReadRequest(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      CControlRPC_method_names[1],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< CControlRPC::Service, ::score::Prepare, ::score::Vote>(
          [](CControlRPC::Service* service,
             ::grpc::ServerContext* ctx,
             const ::score::Prepare* req,
             ::score::Vote* resp) {
               return service->DoPrepare(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      CControlRPC_method_names[2],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< CControlRPC::Service, ::score::Decide, ::score::Committed>(
          [](CControlRPC::Service* service,
             ::grpc::ServerContext* ctx,
             const ::score::Decide* req,
             ::score::Committed* resp) {
               return service->DoDecide(ctx, req, resp);
             }, this)));
}

CControlRPC::Service::~Service() {
}

::grpc::Status CControlRPC::Service::DoReadRequest(::grpc::ServerContext* context, const ::score::ReadRequest* request, ::score::ReadReturn* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status CControlRPC::Service::DoPrepare(::grpc::ServerContext* context, const ::score::Prepare* request, ::score::Vote* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status CControlRPC::Service::DoDecide(::grpc::ServerContext* context, const ::score::Decide* request, ::score::Committed* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace score

