module;
#ifndef BNIK_TEST_USE_STD_MODULE
# include <iterator>
#else
#endif

#define BNIK_TEST_CXX_MODULE

export module bnik.Test.Utils:IteratorTestHelpers;

#ifdef BNIK_TEST_USE_STD_MODULE
import std;
#endif


#include "IteratorTestHelpters.h"

