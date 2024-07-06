#include "../bnik/config.hpp"
#if BNIK_EXCEPTIONS_ENABLED
# ifndef BNIK_TEST_USE_LIBDSTRING_CXX_MODULE
#  include <dstring.hpp>
# endif
# include <gtest/gtest.h>
# ifndef BNIK_TEST_USE_STD_MODULE
#  include <iostream>
#  include <algorithm>
#  include <memory>
#  include <string>
#  include <type_traits>
#  include <span>
#  include <tuple>
#  include <array>
#  include <limits>
# else
import std;
# endif

#ifdef BNIK_TEST_USE_LIBDSTRING_CXX_MODULE
import dstring;
#endif

template<typename T, std::size_t N=std::numeric_limits<std::ptrdiff_t>::max()>
class DummyAllocator {
public:
	using value_type = T;
	using pointer = T*;
	using size_type = std::size_t;
	static constexpr size_type maximum_storage_supported = N;

	constexpr pointer allocate(const size_type n) const {
		return static_cast<pointer>(::operator new(n));
	}

	constexpr void deallocate(pointer p, const size_type n) const {
		::operator delete(p, n);
	}

	constexpr size_type max_size() const noexcept {
		return N;
	}

	constexpr bool operator==(const DummyAllocator&) const noexcept {
		return true;
	}
	constexpr bool operator!=(const DummyAllocator&) const noexcept {
		return false;
	}

	template <class U, std::size_t M=N>
	struct rebind {
		using other = DummyAllocator<U, M>;
	};

	constexpr DummyAllocator() noexcept = default;
	constexpr DummyAllocator(const DummyAllocator&) noexcept = default;

	template<typename OtherT, std::size_t M>
	constexpr DummyAllocator(const DummyAllocator<OtherT, M>&) noexcept {}

	constexpr ~DummyAllocator() = default;

	constexpr DummyAllocator& operator=(const DummyAllocator&) = default;
};

TEST(DStringTest, CapacityLimits01_Basic)
{
	DummyAllocator<char> a1;
	[[maybe_unused]] DummyAllocator<char> a2;

	DummyAllocator<int*> b1;

	static_assert(std::is_same_v<decltype(a1), typename decltype(b1)::template rebind<char>::other>);

	DummyAllocator<int*> b2(a1);


	using cldstring = bnik::basic_dstring<char, std::char_traits<char>, DummyAllocator<char>>;

	cldstring str01{ "Test" };
	EXPECT_STREQ(str01.c_str(), "Test");

	cldstring str02{ "This is the longest string in the world. It is just a couple of sentences long, but it's big enough for this situation." };
	EXPECT_STREQ(str02.c_str(), "This is the longest string in the world. It is just a couple of sentences long, but it's big enough for this situation.");
}

TEST(DStringTest, CapacityLimits02_ArbitraryLimits)
{
	using cldstring = bnik::basic_dstring<char, std::char_traits<char>, DummyAllocator<char>>;
	using EDstring_t = bnik::basic_dstring<char, std::char_traits<char>, DummyAllocator<char, 0>>;

	EXPECT_NO_THROW(EDstring_t{ "Test" });

	EDstring_t str01{ "Test" };
	EXPECT_STREQ(str01.c_str(), "Test");

	using SingleByteDstring_t = bnik::basic_dstring<char, std::char_traits<char>, DummyAllocator<char, 1>>; 
	EXPECT_NO_THROW(SingleByteDstring_t{ "" });
	EXPECT_NO_THROW(SingleByteDstring_t{ "a" }); // ssbo buffer will always be the smallest maximum size supported

	SingleByteDstring_t str02{ "" };
	EXPECT_STREQ(str02.c_str(), "");

	constexpr std::string_view LongStr = "This is the longest string in the world. It is just a couple of sentences long, but it's big enough for this situation.";
	//std::basic_string<char, std::char_traits<char>, DummyAllocator<char, 0 >> str01{ "This is the longest string in the world. It is just a couple of sentences long, but it's big enough for this situation." };
	//EDstring_t str02{ "This is the longest string in the world. It is just a couple of sentences long, but it's big enough for this situation." };
	//EXPECT_STREQ(str02.c_str(), "This is the longest string in the world. It is just a couple of sentences long, but it's big enough for this situation.");
	EXPECT_THROW(EDstring_t{ LongStr.data() }, std::length_error);

	// Make sure bnik::basic_dstring's maximum_size(...) member function accounts for null byte terminator correctly
	using LongStrTest_t = bnik::basic_dstring<char, std::char_traits<char>, DummyAllocator<char, LongStr.size()>>;

	EXPECT_THROW(LongStrTest_t{ LongStr.data() }, std::length_error);

	EXPECT_NO_THROW(LongStrTest_t{ "Test" });

	LongStrTest_t str03{ "Test" };
	EXPECT_STREQ(str03.c_str(), "Test");

	using LongStrTestExact_t = bnik::basic_dstring<char, std::char_traits<char>, DummyAllocator<char, LongStr.size()+1>>;

	EXPECT_NO_THROW(LongStrTestExact_t{ LongStr.data() });

	std::string_view longEnoughStr3{ LongStr.data(), std::string{}.capacity() };
	EXPECT_NO_THROW(LongStrTest_t{longEnoughStr3});
	EXPECT_NO_THROW(LongStrTestExact_t{longEnoughStr3});
#ifndef _LIBCPP_VERSION
	EXPECT_NO_THROW(EDstring_t{ longEnoughStr3 });
#else
	EXPECT_NO_THROW(EDstring_t{ longEnoughStr3.substr(0, 15) });
#endif
}
#endif // BNIK_EXCEPTIONS_ENABLED != 0
