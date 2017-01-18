# coding: utf-8

find_package(UTF8CPP QUIET)
set(${PROJECT_NAME}_COMPILER_MSG
	"Could not find utf8cpp. CMake will exit now.")
if (NOT UTF8CPP_FOUND)
	message(FATAL_ERROR ${${PROJECT_NAME}_COMPILER_MSG})
endif (NOT UTF8CPP_FOUND)

include(CheckCXXSourceCompiles)
set(CMAKE_REQUIRED_INCLUDES ${UTF8CPP_INCLUDE_DIRS})
set(CMAKE_REQUIRED_LIBRARIES ${UTF8CPP_LIBRARIES})
check_cxx_source_compiles(
	"
	#include <utf8.h>
	#include <vector>
	int main(int argc, char ** argv) {
		std::string s(\"ÁÂÃÄÅÆ\");
		std::vector<int> utf32result;
		std::vector<unsigned char> utf8result;
		utf8::utf8to32(s.begin(),s.end(),std::back_inserter(utf32result));
		utf8::utf32to8(utf32result.begin(),utf32result.end(),
			std::back_inserter(utf8result));
		return 0;
	}
    "
    UTF8CPP_COMPILES)
if (UTF8CPP_COMPILES)
	message(INFO "Using the system version of UTF8-CPP")
else(UTF8CPP_COMPILES)
	message(INFO "Using the internal version of UTF8-CPP")
endif(NOT ZLIB_COMPILES)

