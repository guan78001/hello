if(MSVC_VERSION EQUAL 1700)
  add_definitions(-D_VARIADIC_MAX=10)
endif()

################################
# GTest
################################
# Enable ExternalProject CMake module
include(ExternalProject)

# Set default ExternalProject root directory
set_directory_properties(PROPERTIES EP_PREFIX ${CMAKE_BINARY_DIR}/ThirdParty)

# Add gtest
ExternalProject_Add(
    googletest
    GIT_REPOSITORY https://github.com/svn2github/googletest.git
    TIMEOUT 30
    # Force separate output paths for debug and release builds to allow easy
    # identification of correct lib in subsequent TARGET_LINK_LIBRARIES commands
    CMAKE_ARGS -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:PATH=DebugLibs
               -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE:PATH=ReleaseLibs
               -Dgtest_force_shared_crt=ON
    # Disable install step
    INSTALL_COMMAND ""
    # Wrap download, configure and build steps in a script to log output
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON
)

# Specify include dir for googlemock and googletest
ExternalProject_Get_Property(googletest source_dir)
SET(GTEST_INCLUDE_DIR 
    ${source_dir}/include
    ${source_dir}/gtest/include
)

ExternalProject_Get_Property(googletest binary_dir)
SET(GTEST_DEBUG_LIBRARY
    debug ${binary_dir}/DebugLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${CMAKE_FIND_LIBRARY_SUFFIXES}
)
SET(GTEST_MAIN_DEBUG_LIBRARY
    debug ${binary_dir}/DebugLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main${CMAKE_FIND_LIBRARY_SUFFIXES}
)
SET(GTEST_RELEASE_LIBRARY
    optimized ${binary_dir}/ReleaseLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${CMAKE_FIND_LIBRARY_SUFFIXES}
)
SET(GTEST_MAIN_RELEASE_LIBRARY
    optimized ${binary_dir}/ReleaseLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main${CMAKE_FIND_LIBRARY_SUFFIXES}
)

file(GLOB MAIN_SRCS
  "*.[hH]"
  "*.[cC]"
  "*.[hc]pp"
  "*.[hc]xx"
  "*.cc"
)

set(RESRCS
)

set(SRCS
   ${RESRCS}
   ${MAIN_SRCS}
)

set(LIBS
  ${GTEST_DEBUG_LIBRARY}
  ${GTEST_RELEASE_LIBRARY}
)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${GTEST_INCLUDE_DIR}
)


add_executable(UnitTests ${SRCS})
target_link_libraries(UnitTests ${LIBS})
set_target_properties(UnitTests PROPERTIES FOLDER "UnitTests")
