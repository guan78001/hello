aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src ALL_SRC)
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(GoogleTest ${D3D_LIBRARY_TYPE} ${ALL_SRC})
set(LIBRARY_OUTPUT_PATH  "${CMAKE_SOURCE_DIR}/bin")