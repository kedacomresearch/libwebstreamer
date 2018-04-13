project(lib)
file(GLOB_RECURSE  LWS_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.[ch] ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)
project_group(LWS_SOURCES ${CMAKE_CURRENT_SOURCE_DIR})

foreach( filename ${LWS_SOURCES} )
message(${filename})
endforeach()





include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_library(libwebstreamer  SHARED ${LWS_SOURCES} )
target_link_libraries(libwebstreamer ${GST_MODULES_LIBRARIES} )
