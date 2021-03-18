// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: protocol.proto
#ifndef GRPC_protocol_2eproto__INCLUDED
#define GRPC_protocol_2eproto__INCLUDED

#include "protocol.pb.h"

#include <functional>
#include <grpc/impl/codegen/port_platform.h>
#include <grpcpp/impl/codegen/async_generic_service.h>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/client_context.h>
#include <grpcpp/impl/codegen/completion_queue.h>
#include <grpcpp/impl/codegen/message_allocator.h>
#include <grpcpp/impl/codegen/method_handler.h>
#include <grpcpp/impl/codegen/proto_utils.h>
#include <grpcpp/impl/codegen/rpc_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/server_callback_handlers.h>
#include <grpcpp/impl/codegen/server_context.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/status.h>
#include <grpcpp/impl/codegen/stub_options.h>
#include <grpcpp/impl/codegen/sync_stream.h>

namespace score {

//
// Use to send to server to work on transaction
//
// message TxOperation {
// enum OpType {
// START = 0;
// READ = 1;
// WRITE = 2;
// COMMIT = 3;
// ROLLBACK = 4;
// }
// required OpType opType = 1;
// optional string key = 2;
// optional string value = 3;
// optional uint64 txid = 4;
// }
//
// service TxRPC {
// rpc StartTx(Empty) returns (TxIDMsg) {}
// rpc Read(ReadOperation) returns (ReadOperationResponse) {}
// rpc Write(WriteOperation) returns (WriteOperationResponse) {}
// rpc Commit(TxIDMsg) returns (Committed) {}
// }
//
class CControlRPC final {
 public:
  static constexpr char const* service_full_name() {
    return "score.CControlRPC";
  }
  class StubInterface {
   public:
    virtual ~StubInterface() {}
    virtual ::grpc::Status DoReadRequest(::grpc::ClientContext* context, const ::score::ReadRequest& request, ::score::ReadReturn* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::score::ReadReturn>> AsyncDoReadRequest(::grpc::ClientContext* context, const ::score::ReadRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::score::ReadReturn>>(AsyncDoReadRequestRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::score::ReadReturn>> PrepareAsyncDoReadRequest(::grpc::ClientContext* context, const ::score::ReadRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::score::ReadReturn>>(PrepareAsyncDoReadRequestRaw(context, request, cq));
    }
    virtual ::grpc::Status DoPrepare(::grpc::ClientContext* context, const ::score::Prepare& request, ::score::Vote* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::score::Vote>> AsyncDoPrepare(::grpc::ClientContext* context, const ::score::Prepare& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::score::Vote>>(AsyncDoPrepareRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::score::Vote>> PrepareAsyncDoPrepare(::grpc::ClientContext* context, const ::score::Prepare& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::score::Vote>>(PrepareAsyncDoPrepareRaw(context, request, cq));
    }
    virtual ::grpc::Status DoDecide(::grpc::ClientContext* context, const ::score::Decide& request, ::score::Committed* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::score::Committed>> AsyncDoDecide(::grpc::ClientContext* context, const ::score::Decide& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::score::Committed>>(AsyncDoDecideRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::score::Committed>> PrepareAsyncDoDecide(::grpc::ClientContext* context, const ::score::Decide& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::score::Committed>>(PrepareAsyncDoDecideRaw(context, request, cq));
    }
    class experimental_async_interface {
     public:
      virtual ~experimental_async_interface() {}
      virtual void DoReadRequest(::grpc::ClientContext* context, const ::score::ReadRequest* request, ::score::ReadReturn* response, std::function<void(::grpc::Status)>) = 0;
      #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      virtual void DoReadRequest(::grpc::ClientContext* context, const ::score::ReadRequest* request, ::score::ReadReturn* response, ::grpc::ClientUnaryReactor* reactor) = 0;
      #else
      virtual void DoReadRequest(::grpc::ClientContext* context, const ::score::ReadRequest* request, ::score::ReadReturn* response, ::grpc::experimental::ClientUnaryReactor* reactor) = 0;
      #endif
      virtual void DoPrepare(::grpc::ClientContext* context, const ::score::Prepare* request, ::score::Vote* response, std::function<void(::grpc::Status)>) = 0;
      #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      virtual void DoPrepare(::grpc::ClientContext* context, const ::score::Prepare* request, ::score::Vote* response, ::grpc::ClientUnaryReactor* reactor) = 0;
      #else
      virtual void DoPrepare(::grpc::ClientContext* context, const ::score::Prepare* request, ::score::Vote* response, ::grpc::experimental::ClientUnaryReactor* reactor) = 0;
      #endif
      virtual void DoDecide(::grpc::ClientContext* context, const ::score::Decide* request, ::score::Committed* response, std::function<void(::grpc::Status)>) = 0;
      #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      virtual void DoDecide(::grpc::ClientContext* context, const ::score::Decide* request, ::score::Committed* response, ::grpc::ClientUnaryReactor* reactor) = 0;
      #else
      virtual void DoDecide(::grpc::ClientContext* context, const ::score::Decide* request, ::score::Committed* response, ::grpc::experimental::ClientUnaryReactor* reactor) = 0;
      #endif
    };
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    typedef class experimental_async_interface async_interface;
    #endif
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    async_interface* async() { return experimental_async(); }
    #endif
    virtual class experimental_async_interface* experimental_async() { return nullptr; }
  private:
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::score::ReadReturn>* AsyncDoReadRequestRaw(::grpc::ClientContext* context, const ::score::ReadRequest& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::score::ReadReturn>* PrepareAsyncDoReadRequestRaw(::grpc::ClientContext* context, const ::score::ReadRequest& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::score::Vote>* AsyncDoPrepareRaw(::grpc::ClientContext* context, const ::score::Prepare& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::score::Vote>* PrepareAsyncDoPrepareRaw(::grpc::ClientContext* context, const ::score::Prepare& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::score::Committed>* AsyncDoDecideRaw(::grpc::ClientContext* context, const ::score::Decide& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::score::Committed>* PrepareAsyncDoDecideRaw(::grpc::ClientContext* context, const ::score::Decide& request, ::grpc::CompletionQueue* cq) = 0;
  };
  class Stub final : public StubInterface {
   public:
    Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel);
    ::grpc::Status DoReadRequest(::grpc::ClientContext* context, const ::score::ReadRequest& request, ::score::ReadReturn* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::score::ReadReturn>> AsyncDoReadRequest(::grpc::ClientContext* context, const ::score::ReadRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::score::ReadReturn>>(AsyncDoReadRequestRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::score::ReadReturn>> PrepareAsyncDoReadRequest(::grpc::ClientContext* context, const ::score::ReadRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::score::ReadReturn>>(PrepareAsyncDoReadRequestRaw(context, request, cq));
    }
    ::grpc::Status DoPrepare(::grpc::ClientContext* context, const ::score::Prepare& request, ::score::Vote* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::score::Vote>> AsyncDoPrepare(::grpc::ClientContext* context, const ::score::Prepare& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::score::Vote>>(AsyncDoPrepareRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::score::Vote>> PrepareAsyncDoPrepare(::grpc::ClientContext* context, const ::score::Prepare& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::score::Vote>>(PrepareAsyncDoPrepareRaw(context, request, cq));
    }
    ::grpc::Status DoDecide(::grpc::ClientContext* context, const ::score::Decide& request, ::score::Committed* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::score::Committed>> AsyncDoDecide(::grpc::ClientContext* context, const ::score::Decide& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::score::Committed>>(AsyncDoDecideRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::score::Committed>> PrepareAsyncDoDecide(::grpc::ClientContext* context, const ::score::Decide& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::score::Committed>>(PrepareAsyncDoDecideRaw(context, request, cq));
    }
    class experimental_async final :
      public StubInterface::experimental_async_interface {
     public:
      void DoReadRequest(::grpc::ClientContext* context, const ::score::ReadRequest* request, ::score::ReadReturn* response, std::function<void(::grpc::Status)>) override;
      #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      void DoReadRequest(::grpc::ClientContext* context, const ::score::ReadRequest* request, ::score::ReadReturn* response, ::grpc::ClientUnaryReactor* reactor) override;
      #else
      void DoReadRequest(::grpc::ClientContext* context, const ::score::ReadRequest* request, ::score::ReadReturn* response, ::grpc::experimental::ClientUnaryReactor* reactor) override;
      #endif
      void DoPrepare(::grpc::ClientContext* context, const ::score::Prepare* request, ::score::Vote* response, std::function<void(::grpc::Status)>) override;
      #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      void DoPrepare(::grpc::ClientContext* context, const ::score::Prepare* request, ::score::Vote* response, ::grpc::ClientUnaryReactor* reactor) override;
      #else
      void DoPrepare(::grpc::ClientContext* context, const ::score::Prepare* request, ::score::Vote* response, ::grpc::experimental::ClientUnaryReactor* reactor) override;
      #endif
      void DoDecide(::grpc::ClientContext* context, const ::score::Decide* request, ::score::Committed* response, std::function<void(::grpc::Status)>) override;
      #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      void DoDecide(::grpc::ClientContext* context, const ::score::Decide* request, ::score::Committed* response, ::grpc::ClientUnaryReactor* reactor) override;
      #else
      void DoDecide(::grpc::ClientContext* context, const ::score::Decide* request, ::score::Committed* response, ::grpc::experimental::ClientUnaryReactor* reactor) override;
      #endif
     private:
      friend class Stub;
      explicit experimental_async(Stub* stub): stub_(stub) { }
      Stub* stub() { return stub_; }
      Stub* stub_;
    };
    class experimental_async_interface* experimental_async() override { return &async_stub_; }

   private:
    std::shared_ptr< ::grpc::ChannelInterface> channel_;
    class experimental_async async_stub_{this};
    ::grpc::ClientAsyncResponseReader< ::score::ReadReturn>* AsyncDoReadRequestRaw(::grpc::ClientContext* context, const ::score::ReadRequest& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::score::ReadReturn>* PrepareAsyncDoReadRequestRaw(::grpc::ClientContext* context, const ::score::ReadRequest& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::score::Vote>* AsyncDoPrepareRaw(::grpc::ClientContext* context, const ::score::Prepare& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::score::Vote>* PrepareAsyncDoPrepareRaw(::grpc::ClientContext* context, const ::score::Prepare& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::score::Committed>* AsyncDoDecideRaw(::grpc::ClientContext* context, const ::score::Decide& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::score::Committed>* PrepareAsyncDoDecideRaw(::grpc::ClientContext* context, const ::score::Decide& request, ::grpc::CompletionQueue* cq) override;
    const ::grpc::internal::RpcMethod rpcmethod_DoReadRequest_;
    const ::grpc::internal::RpcMethod rpcmethod_DoPrepare_;
    const ::grpc::internal::RpcMethod rpcmethod_DoDecide_;
  };
  static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());

  class Service : public ::grpc::Service {
   public:
    Service();
    virtual ~Service();
    virtual ::grpc::Status DoReadRequest(::grpc::ServerContext* context, const ::score::ReadRequest* request, ::score::ReadReturn* response);
    virtual ::grpc::Status DoPrepare(::grpc::ServerContext* context, const ::score::Prepare* request, ::score::Vote* response);
    virtual ::grpc::Status DoDecide(::grpc::ServerContext* context, const ::score::Decide* request, ::score::Committed* response);
  };
  template <class BaseClass>
  class WithAsyncMethod_DoReadRequest : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithAsyncMethod_DoReadRequest() {
      ::grpc::Service::MarkMethodAsync(0);
    }
    ~WithAsyncMethod_DoReadRequest() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status DoReadRequest(::grpc::ServerContext* /*context*/, const ::score::ReadRequest* /*request*/, ::score::ReadReturn* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestDoReadRequest(::grpc::ServerContext* context, ::score::ReadRequest* request, ::grpc::ServerAsyncResponseWriter< ::score::ReadReturn>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithAsyncMethod_DoPrepare : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithAsyncMethod_DoPrepare() {
      ::grpc::Service::MarkMethodAsync(1);
    }
    ~WithAsyncMethod_DoPrepare() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status DoPrepare(::grpc::ServerContext* /*context*/, const ::score::Prepare* /*request*/, ::score::Vote* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestDoPrepare(::grpc::ServerContext* context, ::score::Prepare* request, ::grpc::ServerAsyncResponseWriter< ::score::Vote>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(1, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithAsyncMethod_DoDecide : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithAsyncMethod_DoDecide() {
      ::grpc::Service::MarkMethodAsync(2);
    }
    ~WithAsyncMethod_DoDecide() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status DoDecide(::grpc::ServerContext* /*context*/, const ::score::Decide* /*request*/, ::score::Committed* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestDoDecide(::grpc::ServerContext* context, ::score::Decide* request, ::grpc::ServerAsyncResponseWriter< ::score::Committed>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(2, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  typedef WithAsyncMethod_DoReadRequest<WithAsyncMethod_DoPrepare<WithAsyncMethod_DoDecide<Service > > > AsyncService;
  template <class BaseClass>
  class ExperimentalWithCallbackMethod_DoReadRequest : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    ExperimentalWithCallbackMethod_DoReadRequest() {
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      ::grpc::Service::
    #else
      ::grpc::Service::experimental().
    #endif
        MarkMethodCallback(0,
          new ::grpc::internal::CallbackUnaryHandler< ::score::ReadRequest, ::score::ReadReturn>(
            [this](
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
                   ::grpc::CallbackServerContext*
    #else
                   ::grpc::experimental::CallbackServerContext*
    #endif
                     context, const ::score::ReadRequest* request, ::score::ReadReturn* response) { return this->DoReadRequest(context, request, response); }));}
    void SetMessageAllocatorFor_DoReadRequest(
        ::grpc::experimental::MessageAllocator< ::score::ReadRequest, ::score::ReadReturn>* allocator) {
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      ::grpc::internal::MethodHandler* const handler = ::grpc::Service::GetHandler(0);
    #else
      ::grpc::internal::MethodHandler* const handler = ::grpc::Service::experimental().GetHandler(0);
    #endif
      static_cast<::grpc::internal::CallbackUnaryHandler< ::score::ReadRequest, ::score::ReadReturn>*>(handler)
              ->SetMessageAllocator(allocator);
    }
    ~ExperimentalWithCallbackMethod_DoReadRequest() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status DoReadRequest(::grpc::ServerContext* /*context*/, const ::score::ReadRequest* /*request*/, ::score::ReadReturn* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    virtual ::grpc::ServerUnaryReactor* DoReadRequest(
      ::grpc::CallbackServerContext* /*context*/, const ::score::ReadRequest* /*request*/, ::score::ReadReturn* /*response*/)
    #else
    virtual ::grpc::experimental::ServerUnaryReactor* DoReadRequest(
      ::grpc::experimental::CallbackServerContext* /*context*/, const ::score::ReadRequest* /*request*/, ::score::ReadReturn* /*response*/)
    #endif
      { return nullptr; }
  };
  template <class BaseClass>
  class ExperimentalWithCallbackMethod_DoPrepare : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    ExperimentalWithCallbackMethod_DoPrepare() {
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      ::grpc::Service::
    #else
      ::grpc::Service::experimental().
    #endif
        MarkMethodCallback(1,
          new ::grpc::internal::CallbackUnaryHandler< ::score::Prepare, ::score::Vote>(
            [this](
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
                   ::grpc::CallbackServerContext*
    #else
                   ::grpc::experimental::CallbackServerContext*
    #endif
                     context, const ::score::Prepare* request, ::score::Vote* response) { return this->DoPrepare(context, request, response); }));}
    void SetMessageAllocatorFor_DoPrepare(
        ::grpc::experimental::MessageAllocator< ::score::Prepare, ::score::Vote>* allocator) {
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      ::grpc::internal::MethodHandler* const handler = ::grpc::Service::GetHandler(1);
    #else
      ::grpc::internal::MethodHandler* const handler = ::grpc::Service::experimental().GetHandler(1);
    #endif
      static_cast<::grpc::internal::CallbackUnaryHandler< ::score::Prepare, ::score::Vote>*>(handler)
              ->SetMessageAllocator(allocator);
    }
    ~ExperimentalWithCallbackMethod_DoPrepare() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status DoPrepare(::grpc::ServerContext* /*context*/, const ::score::Prepare* /*request*/, ::score::Vote* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    virtual ::grpc::ServerUnaryReactor* DoPrepare(
      ::grpc::CallbackServerContext* /*context*/, const ::score::Prepare* /*request*/, ::score::Vote* /*response*/)
    #else
    virtual ::grpc::experimental::ServerUnaryReactor* DoPrepare(
      ::grpc::experimental::CallbackServerContext* /*context*/, const ::score::Prepare* /*request*/, ::score::Vote* /*response*/)
    #endif
      { return nullptr; }
  };
  template <class BaseClass>
  class ExperimentalWithCallbackMethod_DoDecide : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    ExperimentalWithCallbackMethod_DoDecide() {
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      ::grpc::Service::
    #else
      ::grpc::Service::experimental().
    #endif
        MarkMethodCallback(2,
          new ::grpc::internal::CallbackUnaryHandler< ::score::Decide, ::score::Committed>(
            [this](
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
                   ::grpc::CallbackServerContext*
    #else
                   ::grpc::experimental::CallbackServerContext*
    #endif
                     context, const ::score::Decide* request, ::score::Committed* response) { return this->DoDecide(context, request, response); }));}
    void SetMessageAllocatorFor_DoDecide(
        ::grpc::experimental::MessageAllocator< ::score::Decide, ::score::Committed>* allocator) {
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      ::grpc::internal::MethodHandler* const handler = ::grpc::Service::GetHandler(2);
    #else
      ::grpc::internal::MethodHandler* const handler = ::grpc::Service::experimental().GetHandler(2);
    #endif
      static_cast<::grpc::internal::CallbackUnaryHandler< ::score::Decide, ::score::Committed>*>(handler)
              ->SetMessageAllocator(allocator);
    }
    ~ExperimentalWithCallbackMethod_DoDecide() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status DoDecide(::grpc::ServerContext* /*context*/, const ::score::Decide* /*request*/, ::score::Committed* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    virtual ::grpc::ServerUnaryReactor* DoDecide(
      ::grpc::CallbackServerContext* /*context*/, const ::score::Decide* /*request*/, ::score::Committed* /*response*/)
    #else
    virtual ::grpc::experimental::ServerUnaryReactor* DoDecide(
      ::grpc::experimental::CallbackServerContext* /*context*/, const ::score::Decide* /*request*/, ::score::Committed* /*response*/)
    #endif
      { return nullptr; }
  };
  #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
  typedef ExperimentalWithCallbackMethod_DoReadRequest<ExperimentalWithCallbackMethod_DoPrepare<ExperimentalWithCallbackMethod_DoDecide<Service > > > CallbackService;
  #endif

  typedef ExperimentalWithCallbackMethod_DoReadRequest<ExperimentalWithCallbackMethod_DoPrepare<ExperimentalWithCallbackMethod_DoDecide<Service > > > ExperimentalCallbackService;
  template <class BaseClass>
  class WithGenericMethod_DoReadRequest : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithGenericMethod_DoReadRequest() {
      ::grpc::Service::MarkMethodGeneric(0);
    }
    ~WithGenericMethod_DoReadRequest() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status DoReadRequest(::grpc::ServerContext* /*context*/, const ::score::ReadRequest* /*request*/, ::score::ReadReturn* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithGenericMethod_DoPrepare : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithGenericMethod_DoPrepare() {
      ::grpc::Service::MarkMethodGeneric(1);
    }
    ~WithGenericMethod_DoPrepare() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status DoPrepare(::grpc::ServerContext* /*context*/, const ::score::Prepare* /*request*/, ::score::Vote* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithGenericMethod_DoDecide : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithGenericMethod_DoDecide() {
      ::grpc::Service::MarkMethodGeneric(2);
    }
    ~WithGenericMethod_DoDecide() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status DoDecide(::grpc::ServerContext* /*context*/, const ::score::Decide* /*request*/, ::score::Committed* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithRawMethod_DoReadRequest : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawMethod_DoReadRequest() {
      ::grpc::Service::MarkMethodRaw(0);
    }
    ~WithRawMethod_DoReadRequest() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status DoReadRequest(::grpc::ServerContext* /*context*/, const ::score::ReadRequest* /*request*/, ::score::ReadReturn* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestDoReadRequest(::grpc::ServerContext* context, ::grpc::ByteBuffer* request, ::grpc::ServerAsyncResponseWriter< ::grpc::ByteBuffer>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithRawMethod_DoPrepare : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawMethod_DoPrepare() {
      ::grpc::Service::MarkMethodRaw(1);
    }
    ~WithRawMethod_DoPrepare() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status DoPrepare(::grpc::ServerContext* /*context*/, const ::score::Prepare* /*request*/, ::score::Vote* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestDoPrepare(::grpc::ServerContext* context, ::grpc::ByteBuffer* request, ::grpc::ServerAsyncResponseWriter< ::grpc::ByteBuffer>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(1, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithRawMethod_DoDecide : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawMethod_DoDecide() {
      ::grpc::Service::MarkMethodRaw(2);
    }
    ~WithRawMethod_DoDecide() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status DoDecide(::grpc::ServerContext* /*context*/, const ::score::Decide* /*request*/, ::score::Committed* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestDoDecide(::grpc::ServerContext* context, ::grpc::ByteBuffer* request, ::grpc::ServerAsyncResponseWriter< ::grpc::ByteBuffer>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(2, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class ExperimentalWithRawCallbackMethod_DoReadRequest : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    ExperimentalWithRawCallbackMethod_DoReadRequest() {
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      ::grpc::Service::
    #else
      ::grpc::Service::experimental().
    #endif
        MarkMethodRawCallback(0,
          new ::grpc::internal::CallbackUnaryHandler< ::grpc::ByteBuffer, ::grpc::ByteBuffer>(
            [this](
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
                   ::grpc::CallbackServerContext*
    #else
                   ::grpc::experimental::CallbackServerContext*
    #endif
                     context, const ::grpc::ByteBuffer* request, ::grpc::ByteBuffer* response) { return this->DoReadRequest(context, request, response); }));
    }
    ~ExperimentalWithRawCallbackMethod_DoReadRequest() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status DoReadRequest(::grpc::ServerContext* /*context*/, const ::score::ReadRequest* /*request*/, ::score::ReadReturn* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    virtual ::grpc::ServerUnaryReactor* DoReadRequest(
      ::grpc::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/)
    #else
    virtual ::grpc::experimental::ServerUnaryReactor* DoReadRequest(
      ::grpc::experimental::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/)
    #endif
      { return nullptr; }
  };
  template <class BaseClass>
  class ExperimentalWithRawCallbackMethod_DoPrepare : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    ExperimentalWithRawCallbackMethod_DoPrepare() {
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      ::grpc::Service::
    #else
      ::grpc::Service::experimental().
    #endif
        MarkMethodRawCallback(1,
          new ::grpc::internal::CallbackUnaryHandler< ::grpc::ByteBuffer, ::grpc::ByteBuffer>(
            [this](
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
                   ::grpc::CallbackServerContext*
    #else
                   ::grpc::experimental::CallbackServerContext*
    #endif
                     context, const ::grpc::ByteBuffer* request, ::grpc::ByteBuffer* response) { return this->DoPrepare(context, request, response); }));
    }
    ~ExperimentalWithRawCallbackMethod_DoPrepare() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status DoPrepare(::grpc::ServerContext* /*context*/, const ::score::Prepare* /*request*/, ::score::Vote* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    virtual ::grpc::ServerUnaryReactor* DoPrepare(
      ::grpc::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/)
    #else
    virtual ::grpc::experimental::ServerUnaryReactor* DoPrepare(
      ::grpc::experimental::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/)
    #endif
      { return nullptr; }
  };
  template <class BaseClass>
  class ExperimentalWithRawCallbackMethod_DoDecide : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    ExperimentalWithRawCallbackMethod_DoDecide() {
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      ::grpc::Service::
    #else
      ::grpc::Service::experimental().
    #endif
        MarkMethodRawCallback(2,
          new ::grpc::internal::CallbackUnaryHandler< ::grpc::ByteBuffer, ::grpc::ByteBuffer>(
            [this](
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
                   ::grpc::CallbackServerContext*
    #else
                   ::grpc::experimental::CallbackServerContext*
    #endif
                     context, const ::grpc::ByteBuffer* request, ::grpc::ByteBuffer* response) { return this->DoDecide(context, request, response); }));
    }
    ~ExperimentalWithRawCallbackMethod_DoDecide() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status DoDecide(::grpc::ServerContext* /*context*/, const ::score::Decide* /*request*/, ::score::Committed* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    virtual ::grpc::ServerUnaryReactor* DoDecide(
      ::grpc::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/)
    #else
    virtual ::grpc::experimental::ServerUnaryReactor* DoDecide(
      ::grpc::experimental::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/)
    #endif
      { return nullptr; }
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_DoReadRequest : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithStreamedUnaryMethod_DoReadRequest() {
      ::grpc::Service::MarkMethodStreamed(0,
        new ::grpc::internal::StreamedUnaryHandler<
          ::score::ReadRequest, ::score::ReadReturn>(
            [this](::grpc::ServerContext* context,
                   ::grpc::ServerUnaryStreamer<
                     ::score::ReadRequest, ::score::ReadReturn>* streamer) {
                       return this->StreamedDoReadRequest(context,
                         streamer);
                  }));
    }
    ~WithStreamedUnaryMethod_DoReadRequest() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status DoReadRequest(::grpc::ServerContext* /*context*/, const ::score::ReadRequest* /*request*/, ::score::ReadReturn* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedDoReadRequest(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::score::ReadRequest,::score::ReadReturn>* server_unary_streamer) = 0;
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_DoPrepare : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithStreamedUnaryMethod_DoPrepare() {
      ::grpc::Service::MarkMethodStreamed(1,
        new ::grpc::internal::StreamedUnaryHandler<
          ::score::Prepare, ::score::Vote>(
            [this](::grpc::ServerContext* context,
                   ::grpc::ServerUnaryStreamer<
                     ::score::Prepare, ::score::Vote>* streamer) {
                       return this->StreamedDoPrepare(context,
                         streamer);
                  }));
    }
    ~WithStreamedUnaryMethod_DoPrepare() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status DoPrepare(::grpc::ServerContext* /*context*/, const ::score::Prepare* /*request*/, ::score::Vote* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedDoPrepare(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::score::Prepare,::score::Vote>* server_unary_streamer) = 0;
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_DoDecide : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithStreamedUnaryMethod_DoDecide() {
      ::grpc::Service::MarkMethodStreamed(2,
        new ::grpc::internal::StreamedUnaryHandler<
          ::score::Decide, ::score::Committed>(
            [this](::grpc::ServerContext* context,
                   ::grpc::ServerUnaryStreamer<
                     ::score::Decide, ::score::Committed>* streamer) {
                       return this->StreamedDoDecide(context,
                         streamer);
                  }));
    }
    ~WithStreamedUnaryMethod_DoDecide() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status DoDecide(::grpc::ServerContext* /*context*/, const ::score::Decide* /*request*/, ::score::Committed* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedDoDecide(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::score::Decide,::score::Committed>* server_unary_streamer) = 0;
  };
  typedef WithStreamedUnaryMethod_DoReadRequest<WithStreamedUnaryMethod_DoPrepare<WithStreamedUnaryMethod_DoDecide<Service > > > StreamedUnaryService;
  typedef Service SplitStreamedService;
  typedef WithStreamedUnaryMethod_DoReadRequest<WithStreamedUnaryMethod_DoPrepare<WithStreamedUnaryMethod_DoDecide<Service > > > StreamedService;
};

}  // namespace score


#endif  // GRPC_protocol_2eproto__INCLUDED
