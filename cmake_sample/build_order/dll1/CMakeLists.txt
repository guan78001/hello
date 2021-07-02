#当前目录下的所有cpp文件
file(GLOB ALL_SRC
  "*.[hH]"
  "*.[cC]"
  "*.[hc]pp"
  "*.[hc]xx"
  "*.cc"
)
add_library(Dll1 SHARED ${ALL_SRC})
add_definitions(-DDLL1_EXPORTS)
