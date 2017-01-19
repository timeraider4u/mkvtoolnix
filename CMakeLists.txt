# coding: utf-8

### CMakeLists.txt ---
## 
## Copyright (C) 2017 - Harald Weiner (harald.weiner@jku.at)
## This file is distributed under the same license as the mkvtoolnix package.
## 
## https://github.com/mbunkus/mkvtoolnix
######################################################################

project(MKVToolNix)

cmake_minimum_required(VERSION 3.1)

# enable C++11 standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# set build type
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
   # Set default build type to Release
   set(CMAKE_BUILD_TYPE Release CACHE STRING
   "Type of build (None Debug Release)" FORCE)
   set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

# Where to look for cmake files
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(Config)

include(Checking)
include(Options)

include(Endianness)

message("")
message("${${PROJECT_NAME}_NAME} v${${PROJECT_NAME}_VERSION}")
message("Copyright (C) ${${PROJECT_NAME}_YEAR} ${${PROJECT_NAME}_AUTHOR}")
message("${${PROJECT_NAME}_URL}")
message("")
message("Please report any issues under ${${PROJECT_NAME}_BUGREPORT}")
message("")

include(OptionsOutput)
message("")
