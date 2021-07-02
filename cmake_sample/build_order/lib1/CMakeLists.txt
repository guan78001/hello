#当前目录下的所有cpp文件
file(GLOB ALL_SRC
  "*.[hH]"
  "*.[cC]"
  "*.[hc]pp"
  "*.[hc]xx"
  "*.cc"
)
add_library(Lib1 ${ALL_SRC})
