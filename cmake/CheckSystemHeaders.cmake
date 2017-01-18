# coding: utf-8

# check that certain system headers exist
INCLUDE(CheckIncludeFiles)

CHECK_INCLUDE_FILES("inttypes.h;stdint.h;sys/types.h;sys/syscall.h;stropts.h"
	HAVE_SYS_HEADER)
IF (NOT HAVE_SYS_HEADER)
	message(FATAL_ERROR 
	"Could not find system header '${SYS_HEADER}'! CMake will exit now.")
ENDIF (NOT HAVE_SYS_HEADER)

# check that certain system functions exist
INCLUDE (CheckFunctionExists)

SET(${PROJECT_NAME}_SYS_FUNCS "vsscanf;syscall")
FOREACH (SYS_FUNC ${${PROJECT_NAME}_SYS_FUNCS})
	CHECK_FUNCTION_EXISTS(${SYS_FUNC} HAVE_SYS_FUNC_${SYS_FUNC})
	IF (NOT HAVE_SYS_FUNC_${SYS_FUNC})
		message(FATAL_ERROR 
		"Could not find system function '${SYS_FUNC}'! CMake will exit now.")
	ENDIF(NOT HAVE_SYS_FUNC_${SYS_FUNC})
ENDFOREACH(SYS_FUNC)
