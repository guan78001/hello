add_library(AddDLL SHARED AddDLL.h AddDLL.cpp)
#target_link_libraries(AddDLL PUBLIC AddLib PRIVATE AddLib2)
target_link_libraries(AddDLL PUBLIC AddLib PUBLIC AddLib2)
add_definitions(-DADDDLL_EXPORTS)