cmake_minimum_required(VERSION 2.8)
PROJECT (CTestSample)

add_subdirectory(lib1)
add_subdirectory(console1)
add_subdirectory(console2)
add_subdirectory(googletest)
add_subdirectory(test)

enable_testing()
include(CTest)
add_test(NAME test COMMAND Test)
add_test(NAME console1 COMMAND console1)


  