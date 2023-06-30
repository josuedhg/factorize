set(client_src main.c client.c network_connection.c result.c)

add_executable(factorizecli ${client_src})
target_include_directories(factorizecli PRIVATE ${CMAKE_SOURCE_DIR}/client)
target_include_directories(factorizecli PRIVATE ${CMAKE_SOURCE_DIR}/server)
