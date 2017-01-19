# coding: utf-8

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
	include(CheckTypeSize)
	check_type_size("int" SIZE_OF_INT)
	check_type_size("long" SIZE_OF_LONG)
	check_type_size("long long" SIZE_OF_LONG_LONG)
	if (${SIZE_OF_INT} EQUAL 8)
		set(TYPE64 "unsigned int")
		set(TYPEU64 "unsigned int")
	elseif (${SIZE_OF_LONG} EQUAL 8)
		set(TYPE64 "unsigned long")
		set(TYPEU64 "unsigned long")
	elseif (${SIZE_OF_LONG_LONG} EQUAL 8)
		set(TYPE64 "unsigned long long")
		set(TYPEU64 "unsigned long long")
	else ()
		message(FATAL_ERROR 
			"No 64 bit type found on this platform! CMake will exit now.")
	endif ()
endif(HAS_INT_64_t)
