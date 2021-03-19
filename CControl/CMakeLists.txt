add_library(ccontrol INTERFACE)
target_include_directories(ccontrol INTERFACE include)
target_link_libraries(ccontrol INTERFACE store protocol)

add_executable(CControlTest test/CControlTest.cc)
target_link_libraries(CControlTest PRIVATE ccontrol protocol pthread tbb GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)
add_test(CControlTest CControlTest)