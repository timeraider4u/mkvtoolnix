# coding: utf-8

## version configuration
## 
## Versioning scheme: M.m.b (major.minor.patch)
##
## Copyright (C) 2017 - Harald Weiner (harald.weiner@jku.at)
## This file is distributed under the same license as the mkvtoolnix package.
## 
## https://github.com/mbunkus/mkvtoolnix
######################################################################

set(${PROJECT_NAME}_MAJOR_VERSION "9")
set(${PROJECT_NAME}_MINOR_VERSION "7")
set(${PROJECT_NAME}_PATCH_VERSION "1")

# Constuct version info
set(${PROJECT_NAME}_VERSION
	"${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION}.${${PROJECT_NAME}_PATCH_VERSION}"
	CACHE INTERNAL "${PROJECT_NAME} version number")
