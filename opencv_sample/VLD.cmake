#VLD
set(VLD_DIR "${PROJECT_SOURCE_DIR}/../VLD")
set(VLD_INC_DIR ${VLD_DIR}/include) 
set(VLD_LIB_DIR ${VLD_DIR}/lib/win64)
set(VLD_BIN_DIR ${VLD_DIR}/bin/win64)
include_directories(${VLD_INC_DIR})
link_directories(${VLD_LIB_DIR})
message("VLD Dir: ${VLD_BIN_DIR}")

macro (CopyVLD target)
SET(VLD_MODULES "${VLD_BIN_DIR}/dbghelp.dll" "${VLD_BIN_DIR}/vld_x64.dll" "${VLD_BIN_DIR}/microsoft.dtfw.dhl.manifest")
FOREACH(MODULE ${VLD_MODULES})
	add_custom_command(TARGET ${target} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different ${MODULE} ${PROJECT_BINARY_DIR}/$<CONFIGURATION>
	)
ENDFOREACH(MODULE)
endmacro()