# coding: utf-8

# - Try to find utf8cpp
# Once done this will define
#
# UTF8CPP_FOUND - system has utf8cpp
# UTF8CPP_INCLUDE_DIRS - the utf8cpp include directory

if(NOT UTF8CPP_FOUND)
  find_path(UTF8CPP_INCLUDE_DIRS utf8.h)
endif(NOT UTF8CPP_FOUND)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Utf8cpp DEFAULT_MSG UTF8CPP_INCLUDE_DIRS)

mark_as_advanced(UTF8CPP_INCLUDE_DIRS)
