#if 0
#include <string>
#include <array>
#include <iostream>
//#include <print>
#else
//#include <cstddef>
//#include <iostream>
//import std;
//import dstring;
# include <string>
# include <iostream>
#endif

std::string aSimpleFunc() {
	std::string s1 = "I love you!";
	std::string s2 = " You love me!";
	return std::move(std::string{ s1 + s2 });
}

int main()
{
	using string_t = std::string;
	string_t* ptr = nullptr;
	string_t ba{ aSimpleFunc() };//{"I am the longest string in the entire world! Rawr!"};
	{
		string_t foo{ "A fox is SO red, you are blinded by loss of L-cones in your eyes." };
		ptr = &foo;
		std::cout << "Current string is '" << foo.data() << "'\n";

		ba = std::move(foo);
	}

	string_t bar{ "A blue is fox." };
	//using funny_t = typename std::string::uwu_type_t;
	std::cout << "New string is '" << bar.data() << "'\n";
	return 0;


}
