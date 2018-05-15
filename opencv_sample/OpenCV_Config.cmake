set(OPENCV_DIR "D:/github/hello/opencv_sample/opencv")
set(OPENCV_INC_DIR ${OPENCV_DIR}/include) 
set(OPENCV_LIB_DIR ${OPENCV_DIR}/x64/vc12/lib)
set(OPENCV_BIN_DIR ${OPENCV_DIR}/x64/vc12/bin)
set(OPENCV_VERSION "2413")
set(OPENCV_LIB		        debug opencv_calib3d${OPENCV_VERSION}d.lib optimized opencv_calib3d${OPENCV_VERSION}.lib 
							debug opencv_contrib${OPENCV_VERSION}d.lib optimized opencv_contrib${OPENCV_VERSION}.lib 
							debug opencv_core${OPENCV_VERSION}d.lib optimized opencv_core${OPENCV_VERSION}.lib 
							debug opencv_features2d${OPENCV_VERSION}d.lib optimized opencv_features2d${OPENCV_VERSION}.lib 
							debug opencv_flann${OPENCV_VERSION}d.lib optimized opencv_flann${OPENCV_VERSION}.lib 
							debug opencv_gpu${OPENCV_VERSION}d.lib optimized opencv_gpu${OPENCV_VERSION}.lib 
							debug opencv_highgui${OPENCV_VERSION}d.lib optimized opencv_highgui${OPENCV_VERSION}.lib 
							debug opencv_imgproc${OPENCV_VERSION}d.lib optimized opencv_imgproc${OPENCV_VERSION}.lib 
							debug opencv_legacy${OPENCV_VERSION}d.lib optimized opencv_legacy${OPENCV_VERSION}.lib 
							debug opencv_ml${OPENCV_VERSION}d.lib optimized opencv_ml${OPENCV_VERSION}.lib 
							debug opencv_nonfree${OPENCV_VERSION}d.lib optimized opencv_nonfree${OPENCV_VERSION}.lib 
							debug opencv_objdetect${OPENCV_VERSION}d.lib optimized opencv_objdetect${OPENCV_VERSION}.lib 
							debug opencv_ocl${OPENCV_VERSION}d.lib optimized opencv_ocl${OPENCV_VERSION}.lib 
							debug opencv_photo${OPENCV_VERSION}d.lib optimized opencv_photo${OPENCV_VERSION}.lib 
							debug opencv_stitching${OPENCV_VERSION}d.lib optimized opencv_stitching${OPENCV_VERSION}.lib 
							debug opencv_superres${OPENCV_VERSION}d.lib optimized opencv_superres${OPENCV_VERSION}.lib 
							debug opencv_ts${OPENCV_VERSION}d.lib optimized opencv_ts${OPENCV_VERSION}.lib 
							debug opencv_video${OPENCV_VERSION}d.lib optimized opencv_video${OPENCV_VERSION}.lib 
							debug opencv_videostab${OPENCV_VERSION}d.lib optimized opencv_videostab${OPENCV_VERSION}.lib 
)

message("OpenCV_DIR: ${OPENCV_DIR}")
include_directories(${OPENCV_INC_DIR})
link_directories(${OPENCV_LIB_DIR})
