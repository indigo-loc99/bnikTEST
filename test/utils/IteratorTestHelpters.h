#ifndef BNIK_TEST_CXX_MODULE
# include <iterator>
# define BNIK_TEST_HELPERS_CXXM_EXPORT
#else
# define BNIK_TEST_HELPERS_CXXM_EXPORT export
#endif


#ifdef BNIK_TEST_CXX_MODULE
export {
#endif // BNIK_TEST_CXX_MODULE

template <std::random_access_iterator I>
using r_a_iterable_t = I;

template <std::contiguous_iterator I>
using contig_iterable_t = I;

template <std::input_or_output_iterator I>
using i_o_iterable_t = I;

template <std::input_iterator I>
using i_iterable_t = I;

template <std::incrementable I>
using incr_able_t = I;

template <std::bidirectional_iterator I>
using bi_it_t = I;

#ifdef BNIK_TEST_CXX_MODULE
}
#endif // BNIK_TEST_CXX_MODULE
