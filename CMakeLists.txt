cmake_minimum_required(VERSION 3.10)

project(c-template)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

set(CMAKE_C_FLAGS
    "-DDEBUG -g -O0"
    CACHE STRING "Flags used by the C compiler during Debug builds."
    FORCE)

include(CTest)
include(GNUInstallDirs)
include(CMakeOptions.txt)
include_directories(lib)

if(${COVERAGE})
	include(CodeCoverage)
endif(${COVERAGE})

add_subdirectory(lib)
add_subdirectory(server)
add_subdirectory(client)
if(${TESTS})
add_subdirectory(tests)
	if(${COVERAGE})
		coverage_target(. */tests/* check)
	endif(${COVERAGE})
endif(${TESTS})

message(STATUS "TESTS " ${TESTS})
message(STATUS "COVERAGE " ${COVERAGE})
