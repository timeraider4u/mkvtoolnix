# coding: utf-8

# - Try to find utf8cpp
# Once done this will define
#
# UTF8CPP_FOUND - system has utf8cpp
# UTF8CPP_INCLUDE_DIRS - the utf8cpp include directory
# UTF8CPP_LIBRARIES - The libvorbis libraries

pkg_check_modules (UTF8CPP utf8cpp)
list(APPEND UTF8CPP_INCLUDE_DIRS ${UTF8CPP_INCLUDEDIR})

if(NOT UTF8CPP_FOUND)
  find_path(UTF8CPP_INCLUDE_DIRS utf8.h)
  find_library(UTF8CPP_LIBRARIES utf8cpp)
endif(UTF8CPP_FOUND)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Vorbis DEFAULT_MSG VORBIS_INCLUDE_DIRS VORBIS_LIBRARIES)

mark_as_advanced(VORBIS_INCLUDE_DIRS VORBIS_LIBRARIES)
