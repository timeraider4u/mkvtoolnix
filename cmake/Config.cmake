# coding: utf-8

set (${PROJECT_NAME}_NAME "MKVToolNix")
set (${PROJECT_NAME}_AUTHOR "Moritz Bunkus <moritz@bunkus.org>")
set (${PROJECT_NAME}_URL "https://mkvtoolnix.download/")
set (${PROJECT_NAME}_TARNAME "mkvtoolnix")
set (${PROJECT_NAME}_BUGREPORT "https://github.com/mbunkus/mkvtoolnix/issues/")

# current year
string(TIMESTAMP ${PROJECT_NAME}_YEAR "%Y" UTC)

include(Version)

# configure a header file to pass some of the CMake settings
# to the source code
#configure_file (
#  "${PROJECT_SOURCE_DIR}/config.h.in"
#  "${PROJECT_BINARY_DIR}/config.h"
#)
