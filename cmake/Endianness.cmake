# coding: utf-8  

IF (WITH_WORDS_BIG AND WITH_WORDS_LITTLE)
	message(FATAL_ERROR "Can not set big and little-endianness at the same time.")
ENDIF(WITH_WORDS_BIG AND WITH_WORDS_LITTLE)

IF (WITH_WORDS_BIG)
	SET(WORDS_BIGENDIAN 1)
ELSEIF(WITH_WORDS_LITTLE)
	SET(WORDS_BIGENDIAN 0)
ELSE(WITH_WORDS_BIG)
	include(TestBigEndian)
	TEST_BIG_ENDIAN(WORDS_BIGENDIAN)
ENDIF(WITH_WORDS_BIG)
