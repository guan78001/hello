cmake_minimum_required(VERSION 3.20)
PROJECT(VTKSamples)
include(VTK_9.0.cmake)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
message(${CMAKE_BINARY_DIR})
macro(VTK_Project target)
add_executable(${target} ${ARGN} )
target_link_libraries(${target} ${VTK_LIB})
endmacro()

add_subdirectory(Src)
