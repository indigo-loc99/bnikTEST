module;
#ifndef BNIK_TEST_USE_STD_MODULE
# include <concepts>
#else
#endif

#define BNIK_TEST_CXX_MODULE
export module bnik.Test.Utils;

export import :IteratorTestHelpers;

#ifdef BNIK_TEST_USE_STD_MODULE
import std;
#endif

#include "utils.h"
