file(GLOB c_sources ${CMAKE_SOURCE_DIR}/client/*.c)

file(GLOB c_tests test_*.c)

include_directories(${CMAKE_SOURCE_DIR}/client)
include_directories(${CMAKE_SOURCE_DIR}/server)

foreach(file ${c_tests})
	get_filename_component(test_name ${file} NAME_WE)
	add_executable(${test_name} ${file} ${c_sources})

	target_link_libraries(${test_name} cmocka libfactors)

	add_test(NAME ${test_name}
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMAND ${test_name})
	add_dependencies(check ${test_name})
endforeach()
