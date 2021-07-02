cmake_minimum_required(VERSION 2.8)
PROJECT (BuildOrder)

#add_compile_definitions(-DUNICODE -D_UNICODE -D_CRT_NONSTDC_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -D_SCL_SECURE_NO_WARNINGS)

#add 
message(${CMAKE_CXX_FLAGS})
set(Disabled_Warnings "/wd4018 /wd4267 /wd4290 /wd4996")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Disabled_Warnings}")
message(${CMAKE_CXX_FLAGS})
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${Disabled_Warnings}")

#Output all EXEs and DLLs to one folder
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_subdirectory(dll1)
add_subdirectory(lib1)
add_subdirectory(console1)
add_subdirectory(console2)



