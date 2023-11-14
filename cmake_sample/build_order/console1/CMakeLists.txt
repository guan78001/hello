file(GLOB ALL_SRC
  "*.[hH]"
  "*.[cC]"
  "*.[hc]pp"
  "*.[hc]xx"
  "*.cc"
)
include_directories(${CMAKE_SOURCE_DIR}/lib1)
include_directories(${CMAKE_SOURCE_DIR}/dll1)

get_filename_component(ProjectId ${CMAKE_CURRENT_LIST_DIR} NAME)
add_executable(${ProjectId} ${ALL_SRC})
target_link_libraries(${ProjectId} Lib1 Dll1)
target_compile_definitions(${ProjectId} PRIVATE -DMACRO_N=1)

add_executable(console1_ext ${ALL_SRC})
target_compile_definitions(console1_ext PRIVATE -DMACRO_N=2)
target_link_libraries(console1_ext Lib1 Dll1)