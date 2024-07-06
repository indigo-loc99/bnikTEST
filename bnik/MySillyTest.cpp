#define BNIK_CXX23_CONSTEXPR_AVAILABLE 1
#define BNIK_HAS_STD_ALLOCATE_AT_LEAST 1
#include "dstring.hpp"

#include <iostream>
#include <concepts>
#include <ios>

using iter_t = bnik::detail::iter_wrapper<char*>;
#if 0
template<iter_t A, iter_t B>
constexpr bool is_equal() {
    return A == B;
}
#endif

template <std::predicate F>
consteval bool eval_pred(F f) {
    return f();
}

int main() {
    constexpr bool result = eval_pred([]() {bnik::dstring s{"Hello"}; iter_t a = s.begin(), b = s.begin(); return a == b; } );
    std::cout << "Result is: " << std::boolalpha << result << ".\n";
    return 0;
}
