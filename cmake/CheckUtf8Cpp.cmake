# coding: utf-8

find_package(Utf8cpp QUIET)
set(${PROJECT_NAME}_COMPILER_MSG
	"Could not find utf8cpp. CMake will exit now.")
if (UTF8CPP_FOUND)

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
endif (UTF8CPP_FOUND)

if (UTF8CPP_COMPILES)
	set(UTF8CPP_INTERNAL OFF)
	message(STATUS "Using the system version of UTF8-CPP")
else(UTF8CPP_COMPILES)
	set(UTF8CPP_INTERNAL ON)
	message(STATUS "Using the internal version of UTF8-CPP")
endif(UTF8CPP_COMPILES)
