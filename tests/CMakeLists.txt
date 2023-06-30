find_package(cmocka)

enable_testing()

include_directories(${CMAKE_SOURCE_DIR}/lib)

add_definitions(-DTESTING_MODE=ON)

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure)

add_subdirectory(lib)
add_subdirectory(server)
add_subdirectory(client)
