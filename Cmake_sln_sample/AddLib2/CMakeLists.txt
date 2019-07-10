#add_library(AddLib2 AddLib2.h AddLib2.cpp)
get_filename_component(FOLDER_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
add_library(${FOLDER_NAME} AddLib2.h AddLib2.cpp)
message("${FOLDER_NAME}")