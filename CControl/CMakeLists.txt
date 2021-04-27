set(CControlFiles src/CControl.cc src/CControlContextPair.cc
        src/CControlRPC.cc src/Context.cc src/InternalClient.cc src/Server.cc
        src/TXAPI.cc src/TxClientAPI.cc)

add_library(ccontrol STATIC ${CControlFiles})
target_include_directories(ccontrol PUBLIC include)
target_link_libraries(ccontrol PUBLIC store protocol)
target_link_libraries(ccontrol PUBLIC spdlog::spdlog spdlog::spdlog_header_only TBB::tbb pthread)
target_compile_definitions(ccontrol PUBLIC SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)

add_executable(CControlTest test/CControlTest.cc)
target_link_libraries(CControlTest PRIVATE ccontrol GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)
add_test(CControlTest CControlTest)