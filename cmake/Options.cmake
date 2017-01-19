# coding: utf-8 

# TODO: --with-tools???
option(BUILD_TOOLS
	"build the tools in the src/tools sub-directory (useful mainly for development)"
	false)

option(BUILD_STATIC
	"make a static build of the applications"
	false)
	# TODO: LINK_STATICALLY=" -static "

option(WITH_WORDS_BIG
	"set big-endianness"
	false)

option(WITH_WORDS_LITTLE
	"set little-endianness"
	false)
