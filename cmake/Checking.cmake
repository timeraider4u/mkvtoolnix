# coding: utf-8

#include(CheckCXXFeatures) 
#include(CheckSystemHeaders)
# TODO: implement properly include(CheckIntTypes)

find_package(PkgConfig)
if(NOT PKG_CONFIG_FOUND)
	message(FATAL_ERROR "Could not find PkgConfig. Cmake will exit now.")
endif(NOT PKG_CONFIG_FOUND)

include(CheckZlib)

find_package(Ogg REQUIRED)
find_package(Vorbis REQUIRED)

include(CheckUtf8Cpp)
include(CheckBoost)
