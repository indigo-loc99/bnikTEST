#ifndef BNIK_TEST_HELPERS_CXX_MODULE
# include <memory>
# include <expected>
# define BNIK_TEST_HELPERS_CXXM_EXPORT
#else
# define BNIK_TEST_HELPERS_CXXM_EXPORT export
#endif

namespace bnik_test::impl {
	auto annotate(const void* p) -> std::expected<const void*, const char*>;
	auto remove(const void* p) -> std::expected<bool, const char*>;

	[[nodiscard]]
	auto get(const void* p) -> const void*;

	class AllocBufferInspection {
		std::vector<std::string> m_oldContents;
	};
}

enum class AllocCheckTestTypeE {
	EndedLifetime,
	MovedFromObject
};

BNIK_TEST_HELPERS_CXXM_EXPORT template <typename T, AllocCheckTestTypeE E>
class CheckingAlloc {
public:
	using allocator_type = std::allocator<T>;
	T* allocate(typename std::allocator_traits<allocator_type>::size_type n) {
		auto ret = std::allocator_traits<allocator_type>::allocate(allocator, n);
		return ret;
	}
	void deallocate(typename std::allocator_traits<allocator_type>::pointer p, const typename std::allocator_traits<allocator_type>::size_type n) {
		std::allocator_traits<allocator_type>::deallocate(allocator, p, n);
	}

#if defined(_MSC_VER) && !defined(__clang__)
	[[msvc::no_unique_address]]
#else
	[[no_unique_address]]
#endif
	allocator_type allocator;

private:
};

template class CheckingAlloc<int, AllocCheckTestTypeE::EndedLifetime>;
