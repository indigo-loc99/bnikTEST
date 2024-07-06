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
# include <array>
#else
//#error "lol"
import std;
#endif

#ifdef BNIK_TEST_USE_LIBDSTRING_CXX_MODULE
//#error "lol"
import dstring;
#endif

#if 1
TEST(DstringTest, Basics) {
	using namespace std::string_view_literals;
	bnik::dstring testStr("string"sv.data());
	using MyDString = bnik::basic_dstring<wchar_t>;


	constexpr auto sillyVar = sizeof(bnik::dstring);


	MyDString boi(L"Cool"sv.data());
	MyDString ma(L"Cool"sv.data());
	EXPECT_TRUE(boi == ma);
	EXPECT_STREQ("string", testStr.c_str());

	using namespace bnik;

	dstring empty01(""sv.data());
	dstring empty02(""sv.data());

	EXPECT_STREQ(empty01.c_str(), empty02.c_str());
	EXPECT_TRUE(empty01 == empty02);
	EXPECT_FALSE(empty01 != empty02);

	dstring substr01("abcd"sv.data());
	dstring substr02("abc"sv.data());

	std::string std_substr01(substr01.c_str());
	std::string std_substr02(substr02.c_str());

	EXPECT_TRUE(substr01 > substr02);
	EXPECT_EQ(substr01 > substr02, std_substr01 > std_substr02);

	EXPECT_TRUE(substr02 < substr01);
	EXPECT_EQ(substr02 < substr01, std_substr02 < std_substr01);

	EXPECT_TRUE(substr01 != substr02);
	EXPECT_EQ(substr01 != substr02, std_substr01 != std_substr02);

	dstring eq01("hello"sv.data());
	dstring eq02("hello"sv.data());

	EXPECT_TRUE(eq01 == eq02);
	EXPECT_FALSE(eq01 != eq02);

	EXPECT_FALSE(eq01 > eq02);
	EXPECT_FALSE(eq01 < eq02);

	EXPECT_TRUE(eq01 >= eq02);
	EXPECT_TRUE(eq01 <= eq02);

	using namespace std::string_literals;

	std::string test_str = "0123456789abcde"s;
	std::string short_str;

	{
		std::string shorter_str{ "123" };
		//std::println("{}", shorter_str);
		short_str = std::string{ std::move(shorter_str) };
	}

	short_str.push_back('a');
	short_str.push_back('b');
	short_str.push_back('c');
#if 0
	short_str.push_back('f');
	short_str.push_back('g');

	short_str.pop_back();
	short_str.pop_back();
	short_str.pop_back();
	short_str.pop_back();

	short_str.push_back('d');
#endif

	//std::println("We have size of {} bytes", sillyVar);
	std::cout << "We have size of " << sillyVar << " bytes\n";

}
#else
#include <iostream>

int main()
{
	std::string str{ "0123456789abcdefghijklmnop" };
	//str.push_back('h');
	std::cout << str << "\n";
}
#endif


bool IsBufPtrUsed(const bnik::dstring& str) {
	return static_cast<void const*>(str.c_str()) >= &str && static_cast<void const*>(str.c_str()) <= (&str+ 1);
}

template<auto STR>
constexpr auto get_big_enough_str_buff() {
	using c_t = typename decltype(STR)::value_type;
	const bnik::dstring bigEnoughStr{ std::basic_string_view<c_t>{STR.data(), STR.size()}.data()};
	constexpr auto bigEnoughStrSize = bnik::dstring{ std::basic_string_view<c_t>{STR.data(), STR.size()}.data()}.size();

	std::array<typename decltype(bigEnoughStr)::value_type, bigEnoughStrSize+1> aBuf{};

	std::span<const char, bigEnoughStrSize> aView{ bigEnoughStr.c_str(), bigEnoughStr.size() };

	for (int i = 0; auto c: aView) {
		aBuf[i] = c;
		++i;
	}
	aBuf.back() = char{};

	return aBuf;
}

TEST(DstringTest, ShortStringBufferBasicsA) {
	using namespace std::string_view_literals;
	constexpr char bigEnoughStr[] = "0123456789abcde";
	constexpr char biggerStr[] = "Hello there from the world of bnik!";

	bnik::dstring empty01(""sv.data());

	EXPECT_STREQ(empty01.c_str(), "");
	EXPECT_TRUE(IsBufPtrUsed(empty01));

	bnik::dstring single01{ "a"sv.data()};

	EXPECT_STREQ(single01.c_str(), "a");
	EXPECT_TRUE(IsBufPtrUsed(single01));

	//bnik::dstring smallStr01{ bigEnoughStr };
	//EXPECT_STREQ(smallStr01.c_str(), "0123456789abcde");
	//EXPECT_TRUE(IsBufPtrUsed(smallStr01));

	constexpr auto aarray = std::to_array("0123456789abcde");
	constexpr auto smallStr01Arr = get_big_enough_str_buff<aarray>();
	EXPECT_STREQ(smallStr01Arr.data(), "0123456789abcde");

	bnik::dstring bigStr01{ std::string_view{biggerStr}.data()};
	std::string_view bigSv01{ bigStr01.c_str() };
#ifdef _MSC_VER
	EXPECT_TRUE(std::ranges::contains_subrange(biggerStr, bigSv01));
#else
	EXPECT_TRUE(std::string_view{biggerStr}.starts_with(bigSv01));
#endif

	EXPECT_FALSE(IsBufPtrUsed(bigStr01));

	//using silly_t = typename std::string::funny_type_t;
}
