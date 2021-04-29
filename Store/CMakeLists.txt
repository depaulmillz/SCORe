add_library(store INTERFACE)
target_include_directories(store INTERFACE include)
target_link_libraries(store INTERFACE LibCDS::cds-s)
