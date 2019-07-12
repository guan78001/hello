cmake_minimum_required(VERSION 2.8)
if(C22MAKE_VERSION VERSION_GREATER "2.8.11")
  cmake_policy(SET CMP0022 OLD)
endif()
set(EXECUTABLE_OUTPUT_PATH  "${PROJECT_BINARY_DIR}/bin")
set(LIBRARY_OUTPUT_PATH     "${PROJECT_BINARY_DIR}/bin")

#project (Tutorial)
add_executable(Tutorial tutorial.cxx)
#target_link_libraries(Tutorial PRIVATE AddDLL AddLib)
target_link_libraries(Tutorial PRIVATE AddDLL)

add_subdirectory(AddDLL)
add_subdirectory(AddLib)
add_subdirectory(AddLib2)


ADD_DEFINITIONS(-DMACRO)
option(FOO_ENABLE "Enable Foo" ON)
if(FOO_ENABLE)
  set(FOO_STRING "foo")
endif()
message(${FOO_STRING})
ADD_DEFINITIONS(-DVERSION=1)
configure_file(${PROJECT_SOURCE_DIR}/foo.h.in ${PROJECT_SOURCE_DIR}/foo.h @ONLY)

#string(REGEX MATCHALL "d+.d+.d+.d+" GETARCH_RESULT_LIST "1.2.3.4")

set(LIB_PROTOBUF /usr/local/protobuf/lib/libprotobuf.so)
string(REGEX REPLACE "(.+)\\libprotobuf.*" "\\1" LIB_PROTOBUF_PATH ${LIB_PROTOBUF})
message("LIB_PROTOBUF_PATH=${LIB_PROTOBUF_PATH}")

set(VERSION "1.2.3.4")
string(REGEX REPLACE "(.+)\\libprotobuf.*" "\\1" LIB_PROTOBUF_PATH ${LIB_PROTOBUF})

set(MY_PROGRAM_VERSION "" CACHE STRING "1.2.3.4")
string(REPLACE "." ";" VERSION_LIST ${MY_PROGRAM_VERSION})
message("VERSION_LIST=${VERSION_LIST}")
list(GET VERSION_LIST 0 MODULE_MAJOR_VERSION)
list(GET VERSION_LIST 1 MODULE_MINOR_VERSION)
list(GET VERSION_LIST 2 MODULE_REVISION_VERSION)
list(GET VERSION_LIST 3 MODULE_BUILD_NUMBER)

list(APPEND mylist 1 2 3 4)
#set(A "11")
list(GET mylist 0 A)
list(GET mylist 1 B)
list(GET mylist 2 C)
list(GET mylist 3 D)
message("mylist=${mylist}")

configure_file(${PROJECT_SOURCE_DIR}/versioninfo.h.in  ${PROJECT_SOURCE_DIR}/versioninfo.h @ONLY)