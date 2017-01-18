# coding: utf-8

# TODO: implement properly

# test if the 64bit integer types are available, and if not if they
# can be typedef'ed manually.
include(CheckCXXSourceCompiles) 


check_cxx_source_compiles(
	"
	#if HAVE_INTTYPES_H
	# include <inttypes.h>
	#endif
	#if HAVE_STDINT_H
	# include <stdint.h>
	#endif
	#if HAVE_SYS_TYPES_H
	# include <sys/types.h>
	#endif
	int main(int argc, char ** argv) { 
		int64_t foo;
		return 0;
	}
	"
	HAS_INT_64_t)
if (HAS_INT_64_t)
	set(TYPE64 "int64_t")
	set(TYPEU64 "int64_t")
else (HAS_INT_64_t)
	message(FATAL_ERROR 
		"No 64 bit type found on this platform! CMake will exit now.")
endif(HAS_INT_64_t)
