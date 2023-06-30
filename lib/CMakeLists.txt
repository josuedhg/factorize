
file(GLOB c_sources ${CMAKE_SOURCE_DIR}/lib/*.c)
file(GLOB c_headers ${CMAKE_SOURCE_DIR}/lib/*.h)

add_library(libfactors SHARED ${c_sources})

set_target_properties(libfactors PROPERTIES CLEAN_DIRECT_OUTPUT 1)

install(TARGETS libfactors LIBRARY DESTINATION lib${LIB_SUFFIX} COMPONENT sharedlibs)
install( FILES ${c_headers} DESTINATION include/${PROJECT_NAME} COMPONENT headers)
