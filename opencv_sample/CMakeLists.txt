cmake_minimum_required(VERSION 2.8)
PROJECT(OpenCVSamples)
include(OpenCV_Config.cmake)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

#VLD
set(VLD_DIR "D:/github/hello/VLD")
message(${PROJECT_SOURCE_DIR})
set(VLD_INC_DIR ${VLD_DIR}/include) 
set(VLD_LIB_DIR ${VLD_DIR}/lib/win64)
set(VLD_BIN_DIR ${VLD_DIR}/bin/win64)
include_directories(${VLD_INC_DIR})
link_directories(${VLD_LIB_DIR})
message(${VLD_BIN_DIR})

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
	add_custom_command(TARGET ${target} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OPENCV_BIN_DIR}/opencv_${MODULE}${OPENCV_VERSION}${OPENCV_BUILD_TYPE}.dll" ${PROJECT_BINARY_DIR}/$<CONFIGURATION>
	)
ENDFOREACH(MODULE)


SET(EXTRA_MODULES "${VLD_BIN_DIR}/dbghelp.dll" "${VLD_BIN_DIR}/vld_x64.dll" "${VLD_BIN_DIR}/microsoft.dtfw.dhl.manifest")
FOREACH(MODULE ${EXTRA_MODULES})
	add_custom_command(TARGET ${target} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different ${MODULE} ${PROJECT_BINARY_DIR}/$<CONFIGURATION>
	)
ENDFOREACH(MODULE)

endmacro()

add_subdirectory(Src)
