#ifndef BNIK_TEST_USE_LIBDSTRING_CXX_MODULE
# include <dstring.hpp>
# include "utils/IteratorTestHelpters.h"
# include "utils/utils.h"
#endif

#include <gtest/gtest.h>

#ifndef BNIK_TEST_USE_STD_MODULE
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
import std;
#endif

#ifdef BNIK_TEST_USE_LIBDSTRING_CXX_MODULE
import dstring;
import bnik.Test.Utils;
#endif


constexpr bool conduct01() {
	//using iterator_t = bnik::dstring::iterator;
	//const bnik::dstring str{ "conduct01" };

	//i_o_iterable_t<iterator_t> it{ str.data() };
	//return *it == 'c';
	return false;
}

TEST(DStringTest, Constexprness01_Iterators01) {
	using it_t = bnik::dstring::iterator;

	//constexpr bool test01 = invoke_and_return(bnik::dstring{"Test"}, &)
#if 1
	using bnik::dstring;
	/*constexpr */bool test01 = test::utils::eval_predicate([]() { dstring s{"a"}; i_o_iterable_t<it_t> it{s.data()}; return *it == 'a'; });
	EXPECT_TRUE(test01);

	/*constexpr */bool test02 = test::utils::eval_predicate([]()
		{
			dstring s{ "abcd" };
			i_o_iterable_t<it_t> it{ s.data() };

			const bool p1 = *it == 'a';
			++it;
			return p1 && *it == 'b';
		});
	EXPECT_TRUE(test02);

	/*constexpr */bool test03 = test::utils::eval_predicate([]()
		{
			dstring s{ "abcd" };
			i_o_iterable_t<it_t> it{ s.data() };

			return std::addressof(*it++) == s.data() && std::addressof(*it) == s.data() + 1;
		});
	EXPECT_TRUE(test03);

	/*constexpr */bool test04 = test::utils::eval_predicate([]()
		{
			dstring s{ "abcd" };
			i_o_iterable_t<it_t> it{ s.data() };

			return *it++ == 'a' && *it == 'b';
		});
	EXPECT_TRUE(test04);

	EXPECT_FALSE(test::utils::eval_predicate([]() {dstring s{ "abcd" }; i_o_iterable_t<it_t> it{ s.data() }; return *it == 'b'; }));
	EXPECT_FALSE(test::utils::eval_predicate([]() {dstring s{ "abcd" }; i_o_iterable_t<it_t> it{ s.data() }; return *(++it) == 'a'; }));
	EXPECT_FALSE(test::utils::eval_predicate([]() {dstring s{ "abcd" }; i_o_iterable_t<it_t> it{ s.data() }; return *it++ == 'b'; }));

	EXPECT_TRUE(test::utils::eval_predicate([]() {dstring s{ "abcd" }; const i_iterable_t<it_t> it{ s.data() }; return std::is_const_v<decltype(it)>; }));
	EXPECT_FALSE(test::utils::eval_predicate([]() {dstring s{ "abcd" };      i_iterable_t<it_t> it{ s.data() }; return std::is_const_v<decltype(it)>; }));
	EXPECT_TRUE(test::utils::eval_predicate([]() {dstring s{ "abcd" }; const i_iterable_t<it_t> it{ s.data() }; return *it == 'a'; }));
	EXPECT_FALSE(test::utils::eval_predicate([]() {dstring s{ "abcd" }; const i_iterable_t<it_t> it{ s.data() }; return *it == 'b'; }));

# if 1
	EXPECT_TRUE(test::utils::eval_predicate([]() {dstring s{ "abcd" }; return incr_able_t<it_t>{ s.data() } == it_t{ s.data() }; }));
	EXPECT_FALSE(test::utils::eval_predicate([]() {dstring s{ "abcd" }; return incr_able_t<it_t>{ s.data() } == (++it_t{ s.data() }); }));

	EXPECT_TRUE(test::utils::eval_predicate([]() {dstring s{ "abcd" }; return --bi_it_t<it_t>{ s.data() + 3 } == it_t{ std::find(s.data(), s.data() + s.size(), 'c') }; }));
	EXPECT_TRUE(test::utils::eval_predicate([]() {dstring s{ "abcd" }; return --(--bi_it_t<it_t>{ s.data() + 3 }) == it_t{ std::find(s.data(), s.data() + s.size(), 'b') }; }));
	EXPECT_FALSE(test::utils::eval_predicate([]() {dstring s{ "abcd" }; return --bi_it_t<it_t>{ s.data() + 1 } == it_t{ s.data() + 1 }; }));

	EXPECT_TRUE(test::utils::eval_predicate([]() {dstring s{ "thyme" }; return s.begin() + 0 == s.begin(); }));
    EXPECT_TRUE(test::utils::eval_predicate([]() {dstring s{ "thyme" }; return s.begin() + 5 == s.end(); }));
	EXPECT_TRUE(test::utils::eval_predicate([]() {dstring s{ "thyme" }; r_a_iterable_t<it_t> a = s.begin(); return *(a + 1) == 'h' && *(a + 2) == 'y' && *(a + 3) == 'm' && *(a + 4) == 'e'; }));

	EXPECT_TRUE(test::utils::eval_predicate([]() {dstring s{ "test" }; auto a = s.begin(); auto b = a; return a - b == 0; }));
	EXPECT_TRUE(test::utils::eval_predicate([]() {dstring s{ "test" }; auto a = s.begin(); auto b = a + s.size(); return b - a == s.size(); }));
	EXPECT_TRUE(test::utils::eval_predicate([]() {dstring s{ "test" }; auto a = s.begin(); auto b = a + s.size(); return a - b < 0 && a - b == -4; }));

	EXPECT_TRUE(test::utils::eval_predicate([]() {dstring s{ "thyme" }; auto m = s.begin(); bool a = *m == 't'; m += 0; return a && (*m == 't'); }));

	EXPECT_TRUE(test::utils::eval_predicate([]() {dstring s{ "hydrate" }; auto begin = s.begin(); begin -= 0; return *begin == 'h'; }));
	using differ_t = typename bnik::dstring::difference_type;
	EXPECT_TRUE(test::utils::eval_predicate([]() {dstring s{ "hydrate" }; auto iter = s.end(); differ_t len = std::min<differ_t>(s.size(), std::numeric_limits<differ_t>::max()); iter -= len; return *iter == 'h'; }));
	EXPECT_TRUE(test::utils::eval_predicate([]() {dstring s{ "hydrate" }; auto iter = s.end(); differ_t len = std::min<differ_t>(s.size(), std::numeric_limits<differ_t>::max()); iter -= 1; return *iter == 'e' && *(iter -= 2) == 'a'; }));

#if 1
	EXPECT_TRUE(test::utils::eval_predicate(
	[]()
	{
			dstring s{ "Hello World!" };
			contig_iterable_t<it_t> start = s.begin(), end = contig_iterable_t<it_t>(s.data() + s.size());
			std::sort(start, end);
			return *(s.end() - 1) == 'r';
	})
	);
#endif

#endif

#endif

	bnik::dstring str02{ "testStr" };

#if 1
	bnik::dstring::iterator hi = str02.begin(); //hi{ str02.data() };
	bnik::dstring::iterator foo = str02.begin(); // { str02.data() };

	static_assert(std::is_same_v<decltype(hi), decltype(foo)>);
	EXPECT_TRUE(hi == foo);
#endif

#if 1
	std::string str03{ "testStr" };

	auto it3 = std::cbegin(str03), it4 = std::cend(str03);

	EXPECT_TRUE(it4 - it3 == str03.size());

#endif
}

TEST(DStringTest, Constexprness02_AllocAssignment) {
	using it_t = bnik::dstring::iterator;
	using bnik::dstring;
	using namespace std::string_view_literals;

	EXPECT_TRUE(test::utils::eval_predicate([]() {
		dstring smove{ "Foo" };
		dstring sa = std::move(smove);
		return "Foo"sv == sa;
	}));

	EXPECT_TRUE(test::utils::eval_predicate([]() {
		dstring sa{ "Foo" };
		dstring sb = dstring{ sa };
		return "Foo"sv == sa && "Foo"sv == sb;
	}));

	EXPECT_TRUE(test::utils::eval_predicate([]() {
		dstring smove{ "This is a long enough string to set off sso's dynamically allocating large-mode!" };
		dstring sa = std::move(smove);
		return "This is a long enough string to set off sso's dynamically allocating large-mode!"sv == sa;
	}));

	EXPECT_TRUE(test::utils::eval_predicate([]() {
		dstring sa{ "This is a long enough string to set off sso's dynamically allocating large-mode!" };
		dstring sb = dstring{ sa };
		return "This is a long enough string to set off sso's dynamically allocating large-mode!"sv == sb;
	}));

	EXPECT_TRUE(test::utils::eval_predicate([]() {
		dstring sa{ "This is a long enough string to set off sso's dynamically allocating large-mode!" };
		dstring sb{"Another long string: the fox jumped over the large hill of hamburgers to catch the chicken bus."};
		const bool pa{ sa != sb };
		sb = dstring{ sa };
		return pa && "This is a long enough string to set off sso's dynamically allocating large-mode!"sv == sb;
	}));

	EXPECT_TRUE(test::utils::eval_predicate([]() {
		dstring sa{ "This is a long enough string to set off sso's dynamically allocating large-mode!" };
		dstring sb{ "Bar" };
		const bool pa{ sa != sb };
		sb = dstring{ sa };
		return pa && "This is a long enough string to set off sso's dynamically allocating large-mode!"sv == sb;
	}));

	EXPECT_TRUE(test::utils::eval_predicate([]() {
		dstring sa{ "Foo" };
		dstring sb{"Another long string: the fox jumped over the large hill of hamburgers to catch the chicken bus."};
		const bool pa{ sa != sb };
		sb = dstring{ sa };
		return pa && "Foo"sv == sb;
	}));

	EXPECT_TRUE(test::utils::eval_predicate([]() {
		dstring sa{ "" };
		dstring sb{"Another long string: the fox jumped over the large hill of hamburgers to catch the chicken bus."};
		const bool pa{ sa != sb };
		sb = dstring{ sa };
		return pa && ""sv == sb;
	}));

	EXPECT_TRUE(test::utils::eval_predicate([]() {
		dstring sa{ "This is a long enough string to set off sso's dynamically allocating large-mode!" };
		dstring sb{ "" };
		const bool pa{ sa != sb };
		sb = dstring{ sa };
		return pa && "This is a long enough string to set off sso's dynamically allocating large-mode!"sv == sb;
	}));

	EXPECT_TRUE(test::utils::eval_predicate([]() {
		dstring sa{ "" };
		dstring sb{ "" };
		const bool pa{ sa == sb };
		sb = dstring{ sa };
		return pa && ""sv == sb;
	}));
}
