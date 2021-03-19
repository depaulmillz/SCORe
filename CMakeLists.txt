cmake_minimum_required(VERSION 3.14)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake
        CACHE STRING "Vcpkg toolchain file")

project(SCORe LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

enable_testing()

find_package(GTest CONFIG REQUIRED)
set(protobuf_MODULE_COMPATIBLE ON)
find_package(protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

add_subdirectory(Protocol)
add_subdirectory(Store)
add_subdirectory(CControl)