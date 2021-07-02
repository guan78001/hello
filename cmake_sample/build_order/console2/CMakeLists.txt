file(GLOB ALL_SRC
  "*.[hH]"
  "*.[cC]"
  "*.[hc]pp"
  "*.[hc]xx"
  "*.cc"
)
include_directories(${CMAKE_SOURCE_DIR}/lib1)

get_filename_component(ProjectId ${CMAKE_CURRENT_LIST_DIR} NAME)
add_executable(${ProjectId} ${ALL_SRC})
target_link_libraries(${ProjectId} Lib1)
