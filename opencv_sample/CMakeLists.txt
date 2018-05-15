cmake_minimum_required(VERSION 2.8)
PROJECT(OpenCVSamples)
include(OpenCV_Config.cmake)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
message(${CMAKE_BINARY_DIR})
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

set(BUILD_TYPE $<$<CONFIG:debug>:debug>$<$<CONFIG:release>:release>)
set(OPENCV_BUILD_TYPE $<$<CONFIG:debug>:d>$<$<CONFIG:release>:>)
SET(OPENCV_MODULES "calib3d" "contrib" "core" "features2d" "flann" "gpu" "highgui" "imgproc" "legacy" "ml" "nonfree" "objdetect" "ocl" "photo" "stitching" "superres" "video" "videostab")
FOREACH(MODULE ${OPENCV_MODULES})
	add_custom_command(TARGET DisplayImage POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OPENCV_BIN_DIR}/opencv_${MODULE}${OPENCV_VERSION}${OPENCV_BUILD_TYPE}.dll" ${PROJECT_BINARY_DIR}/$<CONFIGURATION>
	)
ENDFOREACH(MODULE)

endmacro()

add_subdirectory(Src)
