# coding: utf-8

set(${PROJECT_NAME}_COMPILER_MSG
	"Could not find zlib library. CMake will exit now.")
include(FindZLIB)
if (NOT ZLIB_FOUND)
	message(FATAL_ERROR ${${PROJECT_NAME}_COMPILER_MSG})
endif (NOT ZLIB_FOUND)
include(CheckCXXSourceCompiles)
set(CMAKE_REQUIRED_INCLUDES ${ZLIB_INCLUDE_DIRS})
set(CMAKE_REQUIRED_LIBRARIES ${ZLIB_LIBRARIES})
check_cxx_source_compiles(
	"
	#include <zlib.h>
	int main(int argc, char ** argv) {
		inflate(0, 0);
		return 0;
	}
    "
    ZLIB_COMPILES)
if (NOT ZLIB_COMPILES)
	message(FATAL_ERROR ${${PROJECT_NAME}_COMPILER_MSG})
endif(NOT ZLIB_COMPILES)
