cmake_minimum_required(VERSION 2.8)
 
PROJECT(TextureMapImageData)
 
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
 
add_executable(TextureMapImageData MACOSX_BUNDLE ShowPlyWithTexture.cxx)
 
if(VTK_LIBRARIES)
  target_link_libraries(TextureMapImageData ${VTK_LIBRARIES})
else()
  target_link_libraries(TextureMapImageData vtkHybrid vtkWidgets)
endif()