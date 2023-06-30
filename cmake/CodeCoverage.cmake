find_program(gcov gcov REQUIRED)
find_program(lcov lcov REQUIRED)
find_program(genhtml genhtml REQUIRED)

SET(CMAKE_C_FLAGS "--coverage")

function(coverage_target build_dir exclude_pattern target_dependency)
	add_custom_target(coverage
		COMMAND ${CMAKE_COMMAND} -E remove_directory ${build_dir}/coverage
		COMMAND ${CMAKE_COMMAND} -E make_directory ${build_dir}/coverage
		COMMAND ${lcov} --directory ${build_dir} --capture --output-file coverage/coverage.info
		COMMAND ${lcov} --directory ${build_dir} --remove coverage/coverage.info ${exclude_pattern} --output-file coverage/coverage.info
		COMMAND ${genhtml} -o ${build_dir}/coverage ${build_dir}/coverage/coverage.info
		DEPENDS ${target_dependency}
		)
endfunction()
