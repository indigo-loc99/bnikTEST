#ifndef BNIK_TEST_USE_LIBDSTRING_CXX_MODULE
# include <dstring.hpp>
#endif

#include <gtest/gtest.h>

#ifndef BNIK_TEST_USE_STD_MODULE
# include <cassert>
# include <iostream>
# include <algorithm>
# include <memory>
# include <string>
# include <type_traits>
# include <span>
# include <tuple>
# include <array>
# include <concepts>
#else
# include <assert.h>
import std;
//import bnik.Test.Helpers;
#endif

#ifdef BNIK_TEST_USE_LIBDSTRING_CXX_MODULE
import dstring;
#endif



TEST(DStringTest, DestructorBehavior01_Clearance) {
	std::string astr;
	std::allocator<char> alloc = astr.get_allocator();
}
