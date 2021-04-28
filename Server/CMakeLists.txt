add_executable(server src/server.cc)
target_link_libraries(server PRIVATE ccontrol Boost::boost)