all: pbuf

pbuf: ../vcpkg/installed/x64-linux/tools/protobuf/protoc  ../vcpkg/installed/x64-linux/tools/grpc
	ls ../vcpkg/installed/x64-linux/tools/protobuf/protoc
	../vcpkg/installed/x64-linux/tools/protobuf/protoc -I ./pb --cpp_out=. protocol.proto
	../vcpkg/installed/x64-linux/tools/protobuf/protoc -I ./pb --grpc_out=. --plugin=protoc-gen-grpc=../vcpkg/installed/x64-linux/tools/grpc/grpc_cpp_plugin protocol.proto
	mv *.cc src
	mv *.h include

clean:
	- rm src/*
	- rm include/*
