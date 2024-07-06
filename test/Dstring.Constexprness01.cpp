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



template<auto STR, class StringType=bnik::basic_dstring<typename decltype(STR)::value_type>>
constexpr auto make_from_dstring() {
	using c_t = typename decltype(STR)::value_type;
	const StringType bigEnoughStr{ std::basic_string_view<c_t>{STR.data(), STR.size()}.data()};
	constexpr auto bigEnoughStrSize = StringType{ std::basic_string_view<c_t>{STR.data(), STR.size()}.data()}.size();
	//constexpr std::basic_string<c_t> bigEnoughStr{ STR.data() };

	std::array<typename decltype(bigEnoughStr)::value_type, bigEnoughStrSize+1> aBuf{};

	std::span<const char, bigEnoughStrSize> aView{ bigEnoughStr.c_str(), bigEnoughStr.size() };

	for (int i = 0; auto c: aView) {
		aBuf[i] = c;
		++i;
	}
	aBuf.back() = char{};

	return aBuf;
}

constexpr std::string short_string() {
	//bnik::dstring a_str{ "hello" };
	return std::string{ "hello" }.c_str();
}

#if 0
constexpr auto short_stdstring_size() {
	constexpr auto thesize = std::string{ "small" }.size();
	return thesize;
}

constexpr auto larger_stdstring_size() {
	constexpr auto thesize = std::string{ "TheBiggestStringYouWillEverWitnessInThisPlebianLifetimeHahaha" }.size();
	return thesize;
}
#endif

// Focus primarily on testing the operations in a constexpr context.
//
// To make it easier to spot for UB, try to evaluate bnik::dstring's supported operations
// at compile time. Not necessarily concerned with the correctness of the operations, as
// those will be tested for in other tests.

TEST(DStringTest, Constexprness01_SSOBuffer) {
	auto emptyStr = make_from_dstring<std::to_array("")>();
	EXPECT_STREQ(emptyStr.data(), "");

	auto oneCharacterStr = make_from_dstring<std::to_array("a")>();
	EXPECT_STREQ(oneCharacterStr.data(), "a");

	auto shortStr = make_from_dstring<std::to_array("test")>();
	EXPECT_STREQ(shortStr.data(), "test");

	auto bigEnoughStr = make_from_dstring<std::to_array("0123456789abcd")>();
	EXPECT_STREQ(bigEnoughStr.data(), "0123456789abcd");

	using namespace std::string_view_literals;
	std::unique_ptr<bnik::dstring> funny = std::make_unique<bnik::dstring>("cool"sv.data());

}

TEST(DStringTest, Constexprness02_DynamicBuffer) {

	auto barelyBiggerStr = make_from_dstring<std::to_array("0123456789abcde")>();
	EXPECT_STREQ(barelyBiggerStr.data(), "0123456789abcde");

	auto bigStr = make_from_dstring<std::to_array("This is the longest string in the world. It is just a couple of sentences long, but it's big enough for this situation.")>();
	EXPECT_STREQ(bigStr.data(), "This is the longest string in the world. It is just a couple of sentences long, but it's big enough for this situation.");

}

TEST(DStringTest, Constexprness03_AllocatorMaxLimits) {
	using cooldstring_t = bnik::basic_dstring<char, std::char_traits<char>, DummyAllocator<char>>;
	auto bigEnoughStr = make_from_dstring<std::to_array("0123456789abcd"), cooldstring_t>();
	EXPECT_STREQ(bigEnoughStr.data(), "0123456789abcd");

	auto barelyBiggerStr = make_from_dstring<std::to_array("0123456789abcde"), cooldstring_t>();
	EXPECT_STREQ(barelyBiggerStr.data(), "0123456789abcde");
}

