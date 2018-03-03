cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(pcl-matching_cube)

# find_package(PCL 1.5 REQUIRED)

# include_directories(${PCL_INCLUDE_DIRS})
# link_directories(${PCL_LIBRARY_DIRS})
# add_definitions(${PCL_DEFINITIONS})
set(3RDPARTY_DIR "D:/git_clone/D3DCamera_v2/D3DCamera/3rdparty")
set(SHARE_ROOT "${3RDPARTY_DIR}/share-msvc-vc12-x64_dll")

if(CMAKE_GENERATOR MATCHES "Win64")
  set(COMPILER_PLATFORM "x64")
else()
  set(COMPILER_PLATFORM "x86")
endif()

set(BOOST_VERSION              1_57_0)
set(PCL_VERSION             1.7)
set(VTK_VERSION             6.2)
set(BOOST_INC_DIR         "${3RDPARTY_DIR}/boost_${BOOST_VERSION}")
message(${COMPILER_PLATFORM})
set(BOOST_LIB_DIR         "${BOOST_INC_DIR}/stage-${COMPILER_PLATFORM}/lib")

set(FLANN_LIB             flann_cpp_s)

set(PCL_BIN_DIR           "${SHARE_ROOT}/bin")
set(PCL_INCLUDE_DIRS      "${SHARE_ROOT}/include"
                          "${SHARE_ROOT}/include/eigen3"
                          "${SHARE_ROOT}/include/vtk-${VTK_VERSION}"
                          "${SHARE_ROOT}/include/pcl-${PCL_VERSION}"
						  "${BOOST_INC_DIR}"
)
set(PCL_LIBRARY_DIRS      "${SHARE_ROOT}/lib"
						  "${BOOST_LIB_DIR}"
)

set(PCL_LIBRARIES		  debug pcl_common_debug optimized pcl_common_release
                          debug pcl_features_debug optimized pcl_features_release
                          debug pcl_filters_debug optimized pcl_filters_release
                          debug pcl_io_debug optimized pcl_io_release
                          debug pcl_kdtree_debug optimized pcl_kdtree_release
                          debug pcl_keypoints_debug optimized pcl_keypoints_release
                          debug pcl_octree_debug optimized pcl_octree_release
                          debug pcl_sample_consensus_debug optimized pcl_sample_consensus_release
                          debug pcl_search_debug optimized pcl_search_release
                          debug pcl_surface_debug optimized pcl_surface_release
                          debug pcl_visualization_debug optimized pcl_visualization_release
)
set(VTK_LIBRARIES

vtkalglib-6.2
vtkChartsCore-6.2
vtkCommonColor-6.2
vtkCommonComputationalGeometry-6.2
vtkCommonCore-6.2
vtkCommonDataModel-6.2
vtkCommonExecutionModel-6.2
vtkCommonMath-6.2
vtkCommonMisc-6.2
vtkCommonSystem-6.2
vtkCommonTransforms-6.2
vtkDICOMParser-6.2
vtkDomainsChemistry-6.2
vtkexoIIc-6.2
vtkexpat-6.2
vtkFiltersAMR-6.2
vtkFiltersCore-6.2
vtkFiltersExtraction-6.2
vtkFiltersFlowPaths-6.2
vtkFiltersGeneral-6.2
vtkFiltersGeneric-6.2
vtkFiltersGeometry-6.2
vtkFiltersHybrid-6.2
vtkFiltersHyperTree-6.2
vtkFiltersImaging-6.2
vtkFiltersModeling-6.2
vtkFiltersParallel-6.2
vtkFiltersParallelImaging-6.2
vtkFiltersProgrammable-6.2
vtkFiltersSelection-6.2
vtkFiltersSMP-6.2
vtkFiltersSources-6.2
vtkFiltersStatistics-6.2
vtkFiltersTexture-6.2
vtkFiltersVerdict-6.2
vtkfreetype-6.2
vtkftgl-6.2
vtkGeovisCore-6.2
vtkgl2ps-6.2
vtkhdf5-6.2
vtkhdf5_hl-6.2
vtkImagingColor-6.2
vtkImagingCore-6.2
vtkImagingFourier-6.2
vtkImagingGeneral-6.2
vtkImagingHybrid-6.2
vtkImagingMath-6.2
vtkImagingMorphological-6.2
vtkImagingSources-6.2
vtkImagingStatistics-6.2
vtkImagingStencil-6.2
vtkInfovisCore-6.2
vtkInfovisLayout-6.2
vtkInteractionImage-6.2
vtkInteractionStyle-6.2
vtkInteractionWidgets-6.2
vtkIOAMR-6.2
vtkIOCore-6.2
vtkIOEnSight-6.2
vtkIOExodus-6.2
vtkIOExport-6.2
vtkIOGeometry-6.2
vtkIOImage-6.2
vtkIOImport-6.2
vtkIOInfovis-6.2
vtkIOLegacy-6.2
vtkIOLSDyna-6.2
vtkIOMINC-6.2
vtkIOMovie-6.2
vtkIONetCDF-6.2
vtkIOParallel-6.2
vtkIOParallelXML-6.2
vtkIOPLY-6.2
vtkIOSQL-6.2
vtkIOVideo-6.2
vtkIOXML-6.2
vtkIOXMLParser-6.2
vtkjpeg-6.2
vtkjsoncpp-6.2
vtklibxml2-6.2
vtkmetaio-6.2
vtkNetCDF-6.2
vtkNetCDF_cxx-6.2
vtkoggtheora-6.2
vtkParallelCore-6.2
vtkpng-6.2
vtkproj4-6.2
vtkRenderingAnnotation-6.2
vtkRenderingContext2D-6.2
vtkRenderingContextOpenGL-6.2
vtkRenderingCore-6.2
vtkRenderingFreeType-6.2
vtkRenderingFreeTypeOpenGL-6.2
vtkRenderingGL2PS-6.2
vtkRenderingImage-6.2
vtkRenderingLabel-6.2
vtkRenderingLIC-6.2
vtkRenderingLOD-6.2
vtkRenderingOpenGL-6.2
vtkRenderingVolume-6.2
vtkRenderingVolumeOpenGL-6.2
vtksqlite-6.2
vtksys-6.2
vtktiff-6.2
vtkverdict-6.2
vtkViewsContext2D-6.2
vtkViewsCore-6.2
vtkViewsInfovis-6.2
vtkzlib-6.2
)
include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})
add_executable (ConcatenatePointClouds ConcatenatePointClouds.cpp)
target_link_libraries (ConcatenatePointClouds ${PCL_LIBRARIES} ${VTK_LIBRARIES})