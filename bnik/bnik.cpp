// bnik.cpp : Defines the entry point for the application.
//

//#include "bnik.h"
import std;
import dstring;

using namespace std;

int main()
{
	bnik::dstring fun("Hello from bnik::dstring, World!");

	std::println("{}", static_cast<std::string_view>(fun));
	//cout << "Hello CMake." << endl;
	return 0;
}
