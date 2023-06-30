set(server_src workqueue.c server.c processor.c network.c main.c)

add_executable(factorizesrv ${server_src})
target_include_directories(factorizesrv PRIVATE ${CMAKE_SOURCE_DIR}/server)
target_link_libraries(factorizesrv LINK_PUBLIC libfactors)
