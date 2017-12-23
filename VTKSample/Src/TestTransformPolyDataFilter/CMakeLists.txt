#VTK_Project(CylinderRenderingProperties CylinderRenderingProperties.cpp)
file(GLOB MAIN_SRCS
  "*.[hH]"
  "*.[cC]"
  "*.[hc]pp"
  "*.[hc]xx"
  "*.cc"
)

add_executable(TestTransformPolyDataFilter ${MAIN_SRCS})
target_link_libraries(TestTransformPolyDataFilter ${VTK_LIB})