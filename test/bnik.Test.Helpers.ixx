module;
#ifndef BNIK_TEST_USE_STD_MODULE
# include <cassert>
# include <expected>
# include <memory>
#else
# include <assert.h>
#endif

#define BNIK_TEST_HELPERS_CXX_MODULE

export module bnik.Test.Helpers;

#ifdef BNIK_TEST_USE_STD_MODULE
import std;
#endif

#include "bnik.Test.Helpers.hpp"

module : private;

#include "TestHelpers.inl"
