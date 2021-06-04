#当前目录下的所有cpp文件
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} ALL_SRC)
add_library(Lib1 ${ALL_SRC})

#output path
set(LIBRARY_OUTPUT_PATH  "${CMAKE_SOURCE_DIR}/bin")