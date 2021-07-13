set(VTK_DIR "C:/csd/git_clone/3rdparty/3rdparty/vtk")
set(VTK_VERSION           9.0$<$<CONFIG:Debug>:d>)
message("VTK_DIR=${VTK_DIR}, VTK_VERSION=${VTK_VERSION}")
#9.0 static library
set(VTK_LIB_DIR ${VTK_DIR}/lib)
include_directories(${VTK_DIR}/include)
link_directories(${VTK_LIB_DIR})

set(VTK_LIB   vtkChartsCore-${VTK_VERSION}
              vtkCommonCore-${VTK_VERSION}
			  vtkCommonColor-${VTK_VERSION}
			  vtkCommonMath-${VTK_VERSION}
			  vtkCommonDataModel-${VTK_VERSION}
			  vtkCommonMisc-${VTK_VERSION}
			  vtkCommonSystem-${VTK_VERSION}
			  vtkCommonTransforms-${VTK_VERSION}
			  vtkCommonComputationalGeometry-${VTK_VERSION}
			  vtkCommonExecutionModel-${VTK_VERSION}
			  vtkDICOMParser-${VTK_VERSION}
			  vtksys-${VTK_VERSION}
			  vtkFiltersSources-${VTK_VERSION}
			  vtkFiltersGeneral-${VTK_VERSION}
			  vtkFiltersCore-${VTK_VERSION}
			  vtkFiltersGeometry-${VTK_VERSION}
			  vtkFiltersExtraction-${VTK_VERSION}
			  vtkFiltersStatistics-${VTK_VERSION}
			  vtkFiltersModeling-${VTK_VERSION}
			  vtkFiltersHybrid-${VTK_VERSION}
			  vtkImagingFourier-${VTK_VERSION}
			  vtkImagingCore-${VTK_VERSION}
			  vtkImagingHybrid-${VTK_VERSION}
			  vtkImagingSources-${VTK_VERSION}
			  vtkImagingGeneral-${VTK_VERSION}
			  vtkImagingColor-${VTK_VERSION}
			  vtkInteractionStyle-${VTK_VERSION}
			  vtkInfovisCore-${VTK_VERSION}
			  vtkInteractionWidgets-${VTK_VERSION}
			  vtkRenderingCore-${VTK_VERSION}
			  vtkRenderingLOD-${VTK_VERSION}
			  #vtkRenderingContextOpenGL2-${VTK_VERSION}
			  vtkRenderingContext2D-${VTK_VERSION}
			  vtkRenderingFreeType-${VTK_VERSION}
			  vtkRenderingOpenGL2-${VTK_VERSION}
			  vtkRenderingVolumeOpenGL2-${VTK_VERSION}
			  vtkRenderingVolume-${VTK_VERSION}
			  vtkRenderingAnnotation-${VTK_VERSION}
			  vtkIOPLY-${VTK_VERSION}
			  vtkIOCore-${VTK_VERSION}
			  vtkIOGeometry-${VTK_VERSION}
			  vtkIOLegacy-${VTK_VERSION}
			  vtkIOImage-${VTK_VERSION}
			  vtkzlib-${VTK_VERSION}
			  vtkjsoncpp-${VTK_VERSION}
			  vtkpng-${VTK_VERSION}
			  vtktiff-${VTK_VERSION}
			  vtkjpeg-${VTK_VERSION}
			  vtkmetaio-${VTK_VERSION}
			  vtkfreetype-${VTK_VERSION}
			  vtkViewsContext2D-${VTK_VERSION}
			  vtkViewsCore-${VTK_VERSION}
			  vtkglew-${VTK_VERSION}
			  
			  vtkloguru-${VTK_VERSION}
			  vtkRenderingUI-${VTK_VERSION}
			  opengl32
			  dbghelp
			  Ws2_32
			  Psapi
			  )

