#VTK_Project(CylinderRenderingProperties CylinderRenderingProperties.cpp)
file(GLOB MAIN_SRCS
  "*.[hH]"
  "*.[cC]"
  "*.[hc]pp"
  "*.[hc]xx"
  "*.cc"
)

add_executable(WritePLY ${MAIN_SRCS})
target_link_libraries(WritePLY ${VTK_LIB})