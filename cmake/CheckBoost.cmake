# coding: utf-8

# search for boost components with compiled libraries
find_package(Boost 1.46.0 REQUIRED 
	COMPONENTS regex filesystem system date_time 
)

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
