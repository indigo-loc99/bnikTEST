#ifndef BNIK_TEST_CXX_MODULE
# include <concepts>
# define BNIK_TEST_HELPERS_CXXM_EXPORT
#else
# define BNIK_TEST_HELPERS_CXXM_EXPORT export
#endif

BNIK_TEST_HELPERS_CXXM_EXPORT namespace test::utils {
	template<std::predicate F>
	consteval bool eval_predicate(F f) {
		return f();
	}
}
