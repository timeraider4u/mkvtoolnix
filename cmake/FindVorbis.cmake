# coding: utf-8

# - Try to find vorbis
# Once done this will define
#
# VORBIS_FOUND - system has libvorbis
# VORBIS_INCLUDE_DIRS - the libvorbis include directory
# VORBIS_LIBRARIES - The libvorbis libraries

pkg_check_modules (VORBIS vorbis)
list(APPEND VORBIS_INCLUDE_DIRS ${VORBIS_INCLUDEDIR})

if(NOT VORBIS_FOUND)
  find_path(VORBIS_INCLUDE_DIRS vorbis/codec.h vorbis/vorbisenc.h)
  find_library(VORBIS_LIBRARIES vorbis)
endif(NOT VORBIS_FOUND)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Vorbis DEFAULT_MSG VORBIS_INCLUDE_DIRS VORBIS_LIBRARIES)

mark_as_advanced(VORBIS_INCLUDE_DIRS VORBIS_LIBRARIES)
