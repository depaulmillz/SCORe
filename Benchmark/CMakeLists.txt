add_executable(smallbank src/smallbank.cc)
target_link_libraries(smallbank PRIVATE ccontrol Boost::boost)