set(protobuf_MODULE_COMPATIBLE ON)
find_package(protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

#file(GLOB ProtoFiles "${CMAKE_CURRENT_SOURCE_DIR}/pb/*.proto")
#protobuf_generate_cpp(ProtoSources ProtoHeaders ${ProtoFiles})

add_library(protocol SHARED src/protocol.grpc.pb.cc src/protocol.pb.cc)
target_link_libraries(protocol PUBLIC protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite)
target_link_libraries(protocol PUBLIC gRPC::gpr gRPC::grpc gRPC::grpc++ gRPC::grpc++_alts)
target_link_libraries(protocol PUBLIC re2::re2 c-ares::cares)
target_include_directories(protocol PUBLIC include)