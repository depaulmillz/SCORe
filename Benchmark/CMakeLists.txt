add_executable(smallbank src/smallbank.cc)
target_link_libraries(smallbank PRIVATE ccontrol Boost::boost)

add_executable(smallbank_correctcheck src/smallbank_correctcheck.cc)
target_link_libraries(smallbank_correctcheck PRIVATE ccontrol Boost::boost)