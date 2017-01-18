# coding: utf-8

# check if compiler supports C++14 standard
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
SET(${PROJECT_NAME}_COMPILER_MSG 
"Please consider using at least GNU gcc-4.9.x or LLVM/Clang-3.4. CMake will exit now.")
if(NOT COMPILER_SUPPORTS_CXX14)
    message(FATAL_ERROR
		"Your compiler does not support the C++11/C++14 standard.
		${${PROJECT_NAME}_COMPILER_MSG}")
endif(NOT COMPILER_SUPPORTS_CXX14)

# check manually if compiler supports required features
include(CheckCXXSourceCompiles)

set(CMAKE_REQUIRED_FLAGS "-std=c++14")

# initializer lists
check_cxx_source_compiles(
	"
	#include <string>
	#include <vector>
	std::vector<std::string> listy = { \"asd\", \"qwe\", \"123\" };
	int main(int argc, char ** argv) { return 0; }
    "
    CXX11_INITIALIZER_LISTS)
if (NOT CXX11_INITIALIZER_LISTS)
	message(FATAL_ERROR "Your compiler does not support C++11/C++14 feature "
		"'initializer lists'. ${${PROJECT_NAME}_COMPILER_MSG}")
endif(NOT CXX11_INITIALIZER_LISTS)

# range based "for"
check_cxx_source_compiles(
	"
	#include <string>
	#include <vector>
	std::string total;
	std::vector<std::string> listy = { \"asd\", \"qwe\", \"123\" };
	int main(int argc, char ** argv) {
		for (std::string &s : listy)
			total += s;
		return 0;
	}
    "
    CXX11_RANGE_BASED_FOR)

if (NOT CXX11_RANGE_BASED_FOR)
	message( FATAL_ERROR "Your compiler does not support C++11/C++14 feature "
		"'range-based >>for<<'. ${${PROJECT_NAME}_COMPILER_MSG}")
endif(NOT CXX11_RANGE_BASED_FOR)

# angle-brackets
check_cxx_source_compiles(
	"
	#include <map>
	#include <vector>
	typedef std::map<int, std::vector<int>> unicorn;
	unicorn charlie;
	int main(int argc, char ** argv) { return 0; }
    "
    CXX11_ANGLE_BRACKETS)

if (NOT CXX11_ANGLE_BRACKETS)
	message( FATAL_ERROR "Your compiler does not support C++11/C++14 feature "
		"'angle-brackets'. ${${PROJECT_NAME}_COMPILER_MSG}")
endif(NOT CXX11_ANGLE_BRACKETS)

# auto keyword
check_cxx_source_compiles(
	"
	#include <vector>
	int main(int argc, char ** argv) {
		std::vector<unsigned int> listy;
		unsigned int sum = 0;
		for (auto i = listy.begin(); i < listy.end(); i++)
			sum += *i;
		return 0;
	}
    "
    CXX11_AUTO_KEYWORD)

if (NOT CXX11_AUTO_KEYWORD)
	message( FATAL_ERROR "Your compiler does not support C++11/C++14 feature "
		"'auto keyword'. ${${PROJECT_NAME}_COMPILER_MSG}")
endif(NOT CXX11_AUTO_KEYWORD)

# lambda function
check_cxx_source_compiles(
	"
	#include <algorithm>
	#include <vector>
	int main(int argc, char ** argv) {
		std::vector<unsigned int> listy;
		unsigned int sum = 0;
		std::for_each(listy.begin(), listy.end(), 
			[&](unsigned int i) 
			{ sum += i; }
		);
		return 0;
	}
    "
    CXX11_LAMBDA_FUNCTION)

if (NOT CXX11_LAMBDA_FUNCTION)
	message( FATAL_ERROR "Your compiler does not support C++11/C++14 feature "
		"'lambda function'. ${${PROJECT_NAME}_COMPILER_MSG}")
endif(NOT CXX11_LAMBDA_FUNCTION)

# nullptr
check_cxx_source_compiles(
	"
	int main(int argc, char ** argv) {
		unsigned char const *charlie = nullptr;
		return 0;
	}
    "
    CXX11_NULLPTR)

if (NOT CXX11_NULLPTR)
	message( FATAL_ERROR "Your compiler does not support C++11/C++14 feature "
		"'nullptr'. ${${PROJECT_NAME}_COMPILER_MSG}")
endif(NOT CXX11_NULLPTR)

# tuples
check_cxx_source_compiles(
	"
	#include <tuple>
	int main(int argc, char ** argv) {
		std::tuple<int, int, char> t = std::make_tuple(1, 2, 'c');
		std::get<2>(t) += std::get<0>(t) * std::get<1>(t);
		return 0;
	}
    "
    CXX11_TUPLES)

if (NOT CXX11_TUPLES)
	message( FATAL_ERROR "Your compiler does not support C++11/C++14 feature "
		"'tuples'. ${${PROJECT_NAME}_COMPILER_MSG}")
endif(NOT CXX11_TUPLES)

# alias declarations
check_cxx_source_compiles(
	"
	#include <vector>
	using thingy = std::vector<int>;
	int main(int argc, char ** argv) {
		thingy things;
		things.push_back(42);
		return 0;
	}
    "
    CXX11_ALIAS_DECLARATIONS)

if (NOT CXX11_ALIAS_DECLARATIONS)
	message( FATAL_ERROR "Your compiler does not support C++11/C++14 feature "
		"'alias declarations'. ${${PROJECT_NAME}_COMPILER_MSG}")
endif(NOT CXX11_ALIAS_DECLARATIONS)

# make unique
check_cxx_source_compiles(
	"
	#include <memory>
	int main(int argc, char ** argv) {
		auto i_ptr{std::make_unique<int>(42)};
		return 0;
	}
    "
    CXX14_MAKE_UNIQUE)

if (NOT CXX14_MAKE_UNIQUE)
	message( FATAL_ERROR "Your compiler does not support C++11/C++14 feature "
		"'make unique'. ${${PROJECT_NAME}_COMPILER_MSG}")
endif(NOT CXX14_MAKE_UNIQUE)

# digit separators
check_cxx_source_compiles(
	"
	int main(int argc, char ** argv) {
		auto num = 10'000'000'000ll;
		return 0;
	}
    "
    CXX14_DIGIT_SEPARATORS)

if (NOT CXX14_DIGIT_SEPARATORS)
	message( FATAL_ERROR "Your compiler does not support C++11/C++14 feature "
		"'digit separators'. ${${PROJECT_NAME}_COMPILER_MSG}")
endif(NOT CXX14_DIGIT_SEPARATORS)

# binary literals
check_cxx_source_compiles(
	"
	int main(int argc, char ** argv) {
		auto num1 = 0b00101010;
		auto num2 = 0B00101010;
		auto num3 = 0b0010'1010;
		return 0;
	}
    "
    CXX14_BINARY_LITERALS)

if (NOT CXX14_BINARY_LITERALS)
	message( FATAL_ERROR "Your compiler does not support C++11/C++14 feature "
		"'binary literals'. ${${PROJECT_NAME}_COMPILER_MSG}")
endif(NOT CXX14_BINARY_LITERALS)

# generic lambdas
check_cxx_source_compiles(
	"
	#include <algorithm>
	#include <vector>
	int main(int argc, char ** argv) {
		std::vector<int> vv;
        std::sort(vv.begin(), vv.end(), 
			[](auto &a, auto &b) { return b < a; }
		);
		return 0;
	}
    "
    CXX14_GENERIC_LAMBDAS)

if (NOT CXX14_GENERIC_LAMBDAS)
	message( FATAL_ERROR "Your compiler does not support C++11/C++14 feature "
		"'generic lambdas'. ${${PROJECT_NAME}_COMPILER_MSG}")
endif(NOT CXX14_GENERIC_LAMBDAS)

# TODO: feature 'human': def name
# CXX14_DEF_NAME
