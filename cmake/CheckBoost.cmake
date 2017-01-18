# coding: utf-8

# see https://cmake.org/cmake/help/v3.0/module/FindBoost.html
# on how to set search hints

# search for boost components with compiled libraries
SET(BOOST_WANTED_MAJOR 1)
SET(BOOST_WANTED_MINOR 46)
SET(BOOST_WANTED_PATCH 0)
SET(BOOST_WANTED_VERSION
	"${BOOST_WANTED_MAJOR}.${BOOST_WANTED_MINOR}.${BOOST_WANTED_PATCH}")
find_package(Boost ${BOOST_WANTED_VERSION} REQUIRED 
	COMPONENTS regex filesystem system date_time 
)
math(EXPR BOOST_WANTED_VERSION_NUMERIC
	"${BOOST_WANTED_MAJOR} * 10000 + ${BOOST_WANTED_MINOR} * 100 
	+ ${BOOST_WANTED_PATCH}")

#message(STATUS "BOOST_WANTED_VERSION_NUMERIC='${BOOST_WANTED_VERSION_NUMERIC}'")

# search for boost headers-only modules
SET(${PROJECT_NAME}_BOOST_FILES "boost/rational.hpp" "boost/logic/tribool.hpp"
	"boost/lexical_cast.hpp" "boost/math/common_factor.hpp"
	"boost/range.hpp" "boost/variant.hpp" "boost/format.hpp")
SET(${PROJECT_NAME}_BOOST_HEADERS "")
FOREACH (BOOST_INC_FILE ${${PROJECT_NAME}_BOOST_FILES})
	find_path(BOOST_INC_HEADER ${BOOST_INC_FILE})
	#message(STATUS 
	#	"search for '${BOOST_INC_FILE}'! found in '${BOOST_INC_HEADER}'")
	IF (NOT BOOST_INC_HEADER)
		message(FATAL_ERROR 
		"Could not find boost header file '${BOOST_INC_FILE}'! CMake will exit now.")
	ENDIF(NOT BOOST_INC_HEADER)
ENDFOREACH(BOOST_INC_FILE)

include(CheckCXXSourceCompiles)
set(CMAKE_REQUIRED_INCLUDES ${BOOST_INCLUDE_DIRS})
set(CMAKE_REQUIRED_LIBRARIES ${BOOST_LIBRARIES})
check_cxx_source_compiles(
	"
	#include <boost/version.hpp>
	#if BOOST_VERSION >= ${BOOST_WANTED_VERSION_NUMERIC}
    // Everything is okay
    #else
    #  error Boost version is too old
    #endif
	int main(int argc, char ** argv) { return 0; }
    "
    BOOST_COMPILES)

IF (NOT BOOST_COMPILES)
	message(FATAL_ERROR "Boost version '${BOOST_WANTED_VERSION}' is too old.")
ENDIF (NOT BOOST_COMPILES)
