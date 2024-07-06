#ifndef BNIK_TEST_USE_LIBDSTRING_CXX_MODULE
# include <dstring.hpp>
#endif
#if 1
#include <gtest/gtest.h>
#endif
#ifndef BNIK_TEST_USE_STD_MODULE
# include <iostream>
# include <algorithm>
# include <memory>
# include <string>
# include <type_traits>
# include <span>
# include <tuple>
# include <array>
#else
import std;
#endif

#ifdef BNIK_TEST_USE_LIBDSTRING_CXX_MODULE
import dstring;
#endif


TEST(DStringTest, Iterators01_Basic)
{
	using it_t = bnik::dstring::iterator;
	bnik::dstring str01{ "Hello, World!" };

#if 1
	it_t str01_it01a{ str01.data() };

	it_t str01_it01b{ str01.data() };

	EXPECT_EQ(std::addressof(*str01_it01a), std::addressof(*str01_it01b));

	++str01_it01b;

	EXPECT_NE(std::addressof(*str01_it01a), std::addressof(*str01_it01b));

	EXPECT_EQ(*str01_it01a, 'H');
#endif

#if 1
	std::string a;
	a.reserve(str01.size());
	auto ait = a.cbegin();

	for (it_t it = it_t{ str01.data() }, end = it_t{ str01.data() + str01.size() }; it != end; ++it) {
		ait = a.insert(ait, *it);
	}

	EXPECT_STREQ(a.data(), "!dlroW ,olleH");
#endif

}

#if 1
template <std::random_access_iterator I>
using iterable = I;

TEST(DStringTest, Iterators01_ConceptsConformance)
{
	iterable<bnik::dstring::iterator> it;
}
#endif

TEST(DStringTest, Iterators01_IncrementOp)
{
	using bnik::dstring;
	using it_t = bnik::dstring::iterator;
	{
		dstring s{ "abcd" };
		it_t a = s.begin();
		it_t b = s.begin();
		EXPECT_EQ(*b, 'a');
		EXPECT_EQ(std::to_address(a), std::to_address(b));
		EXPECT_EQ(b - a, 0);
		++b;
		EXPECT_EQ(*b, 'b');
		EXPECT_EQ(b - a, 1);

		EXPECT_EQ(*(b++), 'b');
		EXPECT_EQ(*b, 'c');
		EXPECT_EQ(b - a, 2);
		std::string substr(a, b);
		EXPECT_STREQ(substr.data(), "ab");
		const auto curr_offset = b - a;
		--b;
		--b;
		EXPECT_EQ(std::to_address(a), std::to_address(b));
		EXPECT_EQ(*(b + curr_offset), 'c');
	}
}
