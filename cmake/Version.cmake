# coding: utf-8

set(${PROJECT_NAME}_MAJOR_VERSION "9")
set(${PROJECT_NAME}_MINOR_VERSION "7")
set(${PROJECT_NAME}_PATCH_VERSION "1")

# Constuct version info
set(${PROJECT_NAME}_VERSION
	"${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION}.${${PROJECT_NAME}_PATCH_VERSION}"
	CACHE INTERNAL "${PROJECT_NAME} version number")
