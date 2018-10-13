cmake_minimum_required(VERSION 2.8)
PROJECT(OpenCVSamples)
include(OpenCV_Config.cmake)
include(VLD.cmake)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

macro(OpenCV_Project target)
file(GLOB MAIN_SRCS
  "*.[hH]"
  "*.[cC]"
  "*.[hc]pp"
  "*.[hc]xx"
  "*.cc"
)
add_executable(${target} ${MAIN_SRCS} )
target_link_libraries(${target} ${OPENCV_LIB})

CopyOpenCV(${target})
CopyVLD(${target})

endmacro()

add_subdirectory(src)
