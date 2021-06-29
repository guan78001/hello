aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} ALL_SRC)
include_directories(${CMAKE_SOURCE_DIR}/lib1)
include_directories(${CMAKE_SOURCE_DIR}/dll1)

get_filename_component(ProjectId ${CMAKE_CURRENT_LIST_DIR} NAME)
add_executable(${ProjectId} ${ALL_SRC})
target_link_libraries(${ProjectId} Lib1 Dll1)

#output path
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin")