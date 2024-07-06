module;
#define BNIK_CXX_MODULES
#include "config.hpp"
#include "common_defs.hpp"
#include <assert.h>
#if !BNIK_USE_STD_MODULES
# include <bit>
# include <concepts>
# include <cstddef>
# include <vector>
# include <cstdint>
# include <string>
# include <string_view>
# include <span>
# include <type_traits>
# include <stdexcept>
#endif

//#define BNIK_USING_STD_MODULES
export module dstring;

#if BNIK_USE_STD_MODULES
import std;
#endif

#include "dstring.hpp"

#if 0

namespace bnik {
	template<std::integral T, class Traits>
	bool operator==(const bnik::basic_dstring<T, Traits>& lhs, const bnik::basic_dstring<T, Traits>& rhs) noexcept
	{
		return lhs.privEquals(rhs);
	}

#if 1
	export template<std::integral T, class Traits>
	inline auto operator<=>(const basic_dstring<T, Traits>& lhs, const basic_dstring<T, Traits>& rhs) noexcept -> typename basic_dstring<T,Traits>::traits_type::comparison_category
	{
		return std::basic_string_view<T, Traits>(lhs) <=> std::basic_string_view<T, Traits>(rhs);
	}
#endif
}


template<std::integral T, typename Traits>
bnik::basic_dstring<T, Traits>::basic_dstring(const T* str)
	: m_data{}
{
	std::basic_string_view<T> src{ str };

	m_data = std::vector<T>{ src.cbegin(), src.cend() };
	m_data.emplace_back(T{ 0 });
}

template<std::integral T, typename Traits>
bnik::basic_dstring<T, Traits>::operator std::basic_string_view<T>() const noexcept {
	return std::basic_string_view<T>{m_data.data(), m_data.size()-1};
}

template<std::integral T, typename Traits>
const T* bnik::basic_dstring<T, Traits>::c_str() const noexcept
{
	return m_data.data();
}

template<std::integral T, typename Traits>
int bnik::basic_dstring<T, Traits>::compare(const bnik::basic_dstring<T, Traits>& str) const noexcept {

	const auto len01 = m_data.size() - 1;
	const auto len02 = str.m_data.size() - 1;

	const auto rlen = std::min(len01, len02);
	const int cmp = traits_type::compare(m_data.data(), str.m_data.data(), rlen);

	if (cmp == 0) {
		return len01 - len02;
	}
	else {
		return cmp;
	}
}

template<std::integral T, typename Traits>
int bnik::basic_dstring<T, Traits>::compare(const std::basic_string<T>& str) const noexcept
{
	const auto len01 = m_data.size() - 1;
	const auto len02 = str.size();

	const auto rlen = std::min(len01, len02);
	const int cmp = traits_type::compare(m_data.data(), str.data(), rlen);

	if (cmp == 0) {
		return len01 - len02;
	}
	else {
		return cmp;
	}
}

namespace bnik {
	namespace detail {

		template<class ChTraits> using ChTraitsChT = typename ChTraits::char_type;

		template<class ChTraits> using ChTraitsPtrT = const typename ChTraits::char_type*;

		template<class Traits>
		inline bool ChTraitsEqual(ChTraitsPtrT<Traits> srcL, std::size_t lenL, ChTraitsPtrT<Traits> srcR, std::size_t lenR) noexcept {
			return lenL == lenR && Traits::compare(srcL, srcR, lenL) == 0;
		}
	}
}

template<std::integral T, typename Traits>
bool bnik::basic_dstring<T, Traits>::privEquals(const basic_dstring& lhs) const noexcept
{
	return detail::ChTraitsEqual<Traits>(m_data.data(), m_data.size() - 1, lhs.m_data.data(), lhs.m_data.size() - 1);
}

#endif
