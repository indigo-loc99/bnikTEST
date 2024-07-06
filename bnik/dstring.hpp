#ifndef DSTRING_HPP
#define DSTRING_HPP

#ifndef BNIK_CXX_MODULES
// TODO Using std modules won't be allowed in this case.
//      Put in a safe guard and require use of standard
//      library headers.

# include "config.hpp"
# include "common_defs.hpp"
# include <cassert>


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

# include <iostream>
#endif

#if BNIK_CAN_USE_STD_ALLOCATE_AT_LEAST
#define BNIK_ALLOC_RESULT(A, N, EXPR) EXPR
#else
# define allocate_at_least allocate
# define BNIK_ALLOC_RESULT(A, N, EXPR) std::make_pair( EXPR, std::size_t{N} )
#endif

namespace bnik {

#if 0
    namespace detail {
        BNIK_CXXM_EXPORT template <typename Iterator>
        class iter_wrapper;
    }

    BNIK_CXXM_EXPORT template <typename Iterator>
    BNIK_CXX23_CONSTEXPR bool operator==(::bnik::detail::iter_wrapper<Iterator> const& lhs, ::bnik::detail::iter_wrapper<Iterator> const& rhs) noexcept;
#endif

    namespace detail {

        template<class StringViewish, class T, class TTraits>
        concept string_viewable = std::convertible_to<StringViewish, std::basic_string_view<T, TTraits>> && !std::convertible_to<StringViewish, const T*>;

        template<typename T, typename V>
        constexpr T ones_bitmask(const V) {
            constexpr auto b = sizeof(V);
            if constexpr (b <= 1)
                return 15;
            else if constexpr (b <= 2)
                return 7;
            else if constexpr (b <= 4)
                return 3;
            else if constexpr (b <= 8)
                return 1;
            else
                return 0;
        }

        template <class ValType>
        struct simple_value_type_profile {
            using value_type = ValType;
            using size_type = std::size_t;
            using difference_type = std::ptrdiff_t;
            using pointer = value_type*;
            using const_pointer = const value_type*;
        };

        template<class TypeProfile>
        class string_type_value {
        public:
            using value_type = typename TypeProfile::value_type;
            using size_type = typename TypeProfile::size_type;
            using difference_type = typename TypeProfile::difference_type;
            using pointer = typename TypeProfile::pointer;
            using const_pointer = typename TypeProfile::const_pointer;
            using reference = value_type&;
            using const_reference = const value_type&;

            static constexpr size_type BUFFER_SIZE = 16U / sizeof(value_type);
            static constexpr size_type alloc_mask = ones_bitmask<size_type>(value_type{});
            static constexpr size_type small_string_capacity = BUFFER_SIZE - 1;

            BNIK_CXX23_CONSTEXPR string_type_value() noexcept : data_{} {}

            BNIK_CXX23_CONSTEXPR pointer ptr_() noexcept {
                if (!large_mode_set_off()) {
                    return data_.buf_;
                }
                else {
                    return data_.ptr_;
                }
            }

            BNIK_CXX23_CONSTEXPR const_pointer cptr_() const noexcept {
                if (!large_mode_set_off()) {
                    return data_.buf_;
                }
                else {
                    return data_.ptr_;
                }
            }

            BNIK_CXX23_CONSTEXPR bool large_mode_set_off() const noexcept {
                return size_ > small_string_capacity;
            }

            BNIK_CXX23_CONSTEXPR void start_sso_buffer() noexcept {
                if (std::is_constant_evaluated()) {
                    for (int i = 0; i < BUFFER_SIZE; ++i) {
                        data_.buf_[i] = value_type();
                    }
                }
            }

            size_type size_ = {};
            size_type reserved_ = {};

            union ssbo_data_t {
                BNIK_CXX23_CONSTEXPR ssbo_data_t() noexcept : buf_{} {}
                BNIK_CXX23_CONSTEXPR ~ssbo_data_t() noexcept {}

                BNIK_CXX23_CONSTEXPR void restart_as_buf() noexcept {
                    ptr_.~pointer();

                    if (std::is_constant_evaluated()) {
                        // NOTE don't use range for-loop cause clang deems it non-constant expression
                        for (size_type i = 0; i < BUFFER_SIZE; ++i) {
                            buf_[i] = value_type();
                        }
                    }
                }

                BNIK_CXX23_CONSTEXPR void zero_out_buf() noexcept {
                    for (auto& c : buf_) {
                        c = value_type();
                    }
                }

                value_type buf_[BUFFER_SIZE];
                pointer ptr_;
            };

            ssbo_data_t data_;
        };

        template<class T, class... Args>
        BNIK_CXX23_CONSTEXPR void construct_in_place(T& obj, Args&&...args) {
            if (std::is_constant_evaluated()) {
                std::construct_at(std::addressof(obj), std::forward<Args>(args)...);
            }
            else {
                ::new (static_cast<void*>(std::addressof(obj))) T(std::forward<Args>(args)...);
            }
        }

        template<typename T>
        BNIK_CXX23_CONSTEXPR void destroy_in_place(T& obj) {
            if (std::is_array_v<T>) {
            }
            else {

            }
        }

        template<typename PtrT>
        BNIK_CXX23_CONSTEXPR auto unfancy(PtrT fancy_ptr) {
            return std::addressof(*fancy_ptr);
        }

        template<typename T>
        BNIK_CXX23_CONSTEXPR T* unfancy(T* ptr) {
            return ptr;
        }

        [[noreturn]] inline void std_length_error(const char* mssg) {
            BNIK_THROW(std::length_error, mssg);
        }

        BNIK_CXXM_EXPORT template<typename Iterator>
        class iter_wrapper;

#if 0
        BNIK_CXXM_EXPORT template <typename Iterator>
        BNIK_CXX23_CONSTEXPR bool operator==(iter_wrapper<Iterator> const& lhs, iter_wrapper<Iterator> const& rhs) noexcept;
#endif

        BNIK_CXXM_EXPORT template <typename Iterator>
        BNIK_CXX23_CONSTEXPR auto operator-(typename std::iterator_traits<Iterator>::difference_type, iter_wrapper<Iterator> const&)
        -> typename std::iterator_traits<Iterator>::difference_type = delete;

        BNIK_CXXM_EXPORT template <typename Iterator>
        BNIK_CXX23_CONSTEXPR auto operator+(const typename std::iterator_traits<Iterator>::difference_type addend, iter_wrapper<Iterator> const& augend) noexcept
        ->  iter_wrapper<Iterator>;



        // makeshift wrapper for iterators; could be a fancy iterator or a pointer
        // TODO pointer types are mainly tested against
        BNIK_CXXM_EXPORT template<typename Iterator>
        class iter_wrapper {
        public:
            using iterator_type = Iterator;
            using iterator_category = std::contiguous_iterator_tag;
            using difference_type = std::iterator_traits<Iterator>::difference_type;
            using value_type = std::iterator_traits<Iterator>::value_type;
            using pointer = std::iterator_traits<Iterator>::pointer;
            using reference = value_type&;

            BNIK_CXX23_CONSTEXPR iter_wrapper() noexcept = default;

            BNIK_CXX23_CONSTEXPR iter_wrapper(pointer val) noexcept : m_val{ val } {}

            //BNIK_CXX23_CONSTEXPR iterator(const iterator& previousCopy) noexcept = default;

            //BNIK_CXX23_CONSTEXPR iterator& operator=(const iterator& rhs) noexcept = default;

            BNIK_CXX23_CONSTEXPR reference operator*() noexcept;

            BNIK_CXX23_CONSTEXPR const reference operator*() const noexcept;

            BNIK_CXX23_CONSTEXPR iter_wrapper& operator++() noexcept;
            BNIK_CXX23_CONSTEXPR iter_wrapper& operator--() noexcept;
            BNIK_CXX23_CONSTEXPR iter_wrapper operator++(int) noexcept;
            BNIK_CXX23_CONSTEXPR iter_wrapper operator--(int) noexcept;

            //BNIK_CXX23_CONSTEXPR bool operator==(iter_wrapper const& rhs) const noexcept;
            BNIK_CXX23_CONSTEXPR std::strong_ordering operator<=>(iter_wrapper const& rhs) const noexcept = default;


            BNIK_CXX23_CONSTEXPR iter_wrapper&  operator-=(const difference_type addend) noexcept;
            BNIK_CXX23_CONSTEXPR iter_wrapper    operator-(const difference_type subtrahend) const noexcept;
            BNIK_CXX23_CONSTEXPR difference_type operator-(iter_wrapper<iterator_type> const& rhs) const noexcept;

            BNIK_CXX23_CONSTEXPR iter_wrapper& operator+=(const difference_type addend) noexcept;
            BNIK_CXX23_CONSTEXPR iter_wrapper   operator+(const difference_type) const noexcept;

            BNIK_CXX23_CONSTEXPR reference operator[](const difference_type) const noexcept;
            BNIK_CXX23_CONSTEXPR pointer operator->() const noexcept;

            //BNIK_CXX23_CONSTEXPR friend bool operator== <>(typename basic_dstring<T,Traits,Alloc>::iterator const& lhs, typename basic_dstring<T,Traits,Alloc>::iterator const& rhs) noexcept;

            //friend BNIK_CXX23_CONSTEXPR bool operator==<>(iter_wrapper const& lhs, iter_wrapper const& rhs) noexcept;

        private:
            pointer m_val;
        };

#if 0
        BNIK_CXXM_EXPORT template <typename Iterator>
        BNIK_CXX23_CONSTEXPR bool operator==(iter_wrapper<Iterator> const& lhs, iter_wrapper<Iterator> const& rhs) noexcept {
            return lhs.m_val == rhs.m_val;
        }
#endif
#define BNIK_COMMUTATIVE_ITERATOR_ADDITION
#ifdef BNIK_COMMUTATIVE_ITERATOR_ADDITION

        BNIK_CXXM_EXPORT template<typename Iterator>
        BNIK_CXX23_CONSTEXPR auto operator+(const typename std::iterator_traits<Iterator>::difference_type addend, iter_wrapper<Iterator> const& augend) noexcept
        -> iter_wrapper<Iterator>;

#endif // BNIK_COMMUTATIVE_ITERATOR_ADDITION

        template <typename Alloc>
        BNIK_CXX23_CONSTEXPR void pocma_assignment(Alloc& assignee, Alloc& source) noexcept {
            if constexpr (std::allocator_traits<Alloc>::propagate_on_container_move_assignment::value) {
                assignee = std::move(source);
            }
        }
    }


    BNIK_CXXM_EXPORT
    template<std::integral T, typename Traits, typename Allocator>
    class basic_dstring;

    BNIK_CXXM_EXPORT template<std::integral T, class Traits, class Allocator>
    BNIK_CXX23_CONSTEXPR inline bool operator==(const basic_dstring<T, Traits, Allocator>& lhs, const basic_dstring<T, Traits, Allocator>& rhs) noexcept;

    BNIK_CXXM_EXPORT struct MyAlloc { };

    BNIK_CXXM_EXPORT
    template<std::integral T=char, typename Traits=std::char_traits<T>, class Allocator=std::allocator<T>>
    class basic_dstring {
    private:
        using str_val_t = detail::string_type_value<detail::simple_value_type_profile<T>>;
        static constexpr auto BUFFER_SIZE_ = str_val_t::BUFFER_SIZE;
        static constexpr auto alloc_mask = str_val_t::alloc_mask;
        static constexpr auto small_string_capacity_ = str_val_t::small_string_capacity;

        using alloc_t_traits = std::allocator_traits<Allocator>;
    public:

        using value_type = T;
        using size_type = typename alloc_t_traits::size_type;
        using difference_type = typename alloc_t_traits::difference_type;
        using pointer = typename alloc_t_traits::pointer;
        using const_pointer = typename alloc_t_traits::const_pointer;
        using traits_type = Traits;
        using allocator_type = Allocator;
        using iterator = detail::iter_wrapper<pointer>;

        //BNIK_CXX23_CONSTEXPR basic_dstring(const std::basic_string_view<T> str);

#if 1
        template<detail::string_viewable<value_type, traits_type> StringViewable>
        BNIK_CXX23_CONSTEXPR basic_dstring(const StringViewable str) {
            construct<construct_method_e_::ViaPtr>(str.data(), str.size());
        }
#endif
        BNIK_CXX23_CONSTEXPR basic_dstring(const value_type* src);

        BNIK_CXX23_CONSTEXPR explicit basic_dstring(const basic_dstring& other);

        BNIK_CXX23_CONSTEXPR basic_dstring(basic_dstring&& other) noexcept;
        BNIK_CXX23_CONSTEXPR basic_dstring& operator=(basic_dstring&& rhs) 
                                        noexcept(std::allocator_traits<allocator_type>::propagate_on_container_move_assignment::value != false);

        // we don't allow this but allow this, respectively: basic_dstring str_b = str_a; basic_dstring str_b = basic_dstring{str_a};
        BNIK_CXX23_CONSTEXPR basic_dstring& operator=(const basic_dstring&) = delete;

        BNIK_CXX23_CONSTEXPR ~basic_dstring() noexcept;

        BNIK_CXX23_CONSTEXPR operator std::basic_string_view<T>() const noexcept;

        BNIK_CXX23_CONSTEXPR const_pointer c_str() const noexcept;

        BNIK_CXX23_CONSTEXPR const_pointer data() const noexcept;
        BNIK_CXX23_CONSTEXPR pointer data() noexcept;

        int compare(const basic_dstring& str) const noexcept;
        int compare(const std::basic_string<T>& str) const noexcept;

        BNIK_CXX23_CONSTEXPR inline size_type size() const noexcept { return m_store.size_; }

        BNIK_CXX23_CONSTEXPR iterator begin() noexcept {
            return iterator(data());
        }

        BNIK_CXX23_CONSTEXPR iterator end() noexcept {
            return iterator(data() + size());
        }

        friend BNIK_CXX23_CONSTEXPR bool ::bnik::operator==<>(const basic_dstring<T, Traits, Allocator>& lhs, const basic_dstring<T, Traits, Allocator>& rhs) noexcept;
    private:
        BNIK_CXX23_CONSTEXPR size_type maximum_size() const noexcept;

        BNIK_CXX23_CONSTEXPR void empty_construct() noexcept;

        BNIK_CXX23_CONSTEXPR void take_contents(basic_dstring& source);

        using alloc_t_ = typename std::allocator_traits<Allocator>::template rebind_alloc<T>;
        BNIK_CXX23_CONSTEXPR       alloc_t_& get_al_() noexcept { return m_alloc; }
        BNIK_CXX23_CONSTEXPR const alloc_t_& get_al_() const noexcept { return m_alloc; }

        BNIK_CXX23_CONSTEXPR void             dellocate_for_capacity (      alloc_t_  &al, const pointer oldPtr, const size_type capacity) noexcept;
        BNIK_CXX23_CONSTEXPR size_type        calculate_capacity     (const size_type newSize) noexcept;
        static BNIK_CXX23_CONSTEXPR size_type calculate_capacity_perf(const size_type newSize, const size_type oldSize, const size_type maxSizePossible) noexcept;

        BNIK_CXX23_CONSTEXPR void zero_out(pointer startPtr, const size_type size) noexcept;

        BNIK_CXX23_CONSTEXPR void tidy_deallocate() noexcept;
        //BNIK_CXX23_CONSTEXPR void tidy_deallocate_for_move(basic_dstring& source) noexcept;

        enum class construct_method_e_ : std::uint8_t {
            ViaSingleChar,
            ViaPtr,
            ViaString
        };

        template<construct_method_e_ METHOD, class CharOrPtr> requires std::is_scalar_v<CharOrPtr>
        BNIK_CXX23_CONSTEXPR void construct(const CharOrPtr obj, const size_type count);

        BNIK_CXX23_CONSTEXPR bool privEquals(const basic_dstring& lhs) const noexcept;


        BNIK_NO_UNIQUE_ADDRESS Allocator m_alloc;
        detail::string_type_value<detail::simple_value_type_profile<T>> m_store;
        static constexpr float GROWTH_FACTOR = 1.4f;
    };

    BNIK_CXXM_EXPORT using dstring = basic_dstring<char>;


}

#if 1

namespace bnik {
    //
    //template<std::integral T, typename Traits, class Alloc>
    //BNIK_CXX23_CONSTEXPR bool operator==(typename ::bnik::basic_dstring<T, Traits, Alloc>::iterator const& lhs, typename ::bnik::basic_dstring<T, Traits, Alloc>::iterator const& rhs) noexcept
    //{
        //return lhs.m_val == rhs.m_val;
    //    return true;
    //}

    template<std::integral T, class Traits, class Alloc>
    BNIK_CXX23_CONSTEXPR bool operator==(const bnik::basic_dstring<T, Traits, Alloc>& lhs, const bnik::basic_dstring<T, Traits, Alloc>& rhs) noexcept
    {
        return lhs.privEquals(rhs);
    }

#if 1
    BNIK_CXXM_EXPORT template<std::integral T, class Traits, class Alloc>
    inline auto operator<=>(const basic_dstring<T, Traits, Alloc>& lhs, const basic_dstring<T, Traits, Alloc>& rhs) noexcept -> typename basic_dstring<T,Traits,Alloc>::traits_type::comparison_category
    {
        return std::basic_string_view<T, Traits>(lhs) <=> std::basic_string_view<T, Traits>(rhs);
    }
#endif
}


template<typename Iterator>
BNIK_CXX23_CONSTEXPR auto bnik::detail::iter_wrapper<Iterator>::operator*() noexcept
-> bnik::detail::iter_wrapper<Iterator>::reference
{
    return *m_val;
}

template<typename Iterator>
BNIK_CXX23_CONSTEXPR auto bnik::detail::iter_wrapper<Iterator>::operator*() const noexcept
-> const bnik::detail::iter_wrapper<Iterator>::reference
{
    return *m_val;
}

template<typename Iterator>
BNIK_CXX23_CONSTEXPR auto bnik::detail::iter_wrapper<Iterator>::operator++() noexcept
-> bnik::detail::iter_wrapper<Iterator>&
{
    ++m_val;
    return *this;
}

template<typename Iterator>
BNIK_CXX23_CONSTEXPR auto bnik::detail::iter_wrapper<Iterator>::operator++(int) noexcept
-> bnik::detail::iter_wrapper<Iterator>
{
    iter_wrapper it_copy{ *this };
    ++m_val;
    return it_copy;
}

template<typename Iterator>
BNIK_CXX23_CONSTEXPR auto bnik::detail::iter_wrapper<Iterator>::operator--() noexcept
-> bnik::detail::iter_wrapper<Iterator>&
{
    --m_val;
    return *this;
}

#if 1
template<typename Iterator>
BNIK_CXX23_CONSTEXPR auto bnik::detail::iter_wrapper<Iterator>::operator--(int) noexcept
-> bnik::detail::iter_wrapper<Iterator>
{
    iter_wrapper it_copy{ *this };
    --m_val;
    return it_copy;
}
#endif

template<typename Iterator>
BNIK_CXX23_CONSTEXPR typename bnik::detail::iter_wrapper<Iterator>::reference bnik::detail::iter_wrapper<Iterator>::operator[](const difference_type i) const noexcept
{
    return m_val[i];
}

template<typename Iterator>
BNIK_CXX23_CONSTEXPR typename bnik::detail::iter_wrapper<Iterator>::pointer bnik::detail::iter_wrapper<Iterator>::operator->() const noexcept
{
    return m_val;
}

template <typename Iterator>
BNIK_CXX23_CONSTEXPR typename bnik::detail::iter_wrapper<Iterator>::difference_type bnik::detail::iter_wrapper<Iterator>::operator-(iter_wrapper const& rhs) const noexcept
{
    return m_val - rhs.m_val;
}

template <typename Iterator>
BNIK_CXX23_CONSTEXPR bnik::detail::iter_wrapper<Iterator> bnik::detail::iter_wrapper<Iterator>::operator-(const difference_type subtrahend) const noexcept
{
    return iter_wrapper(m_val - subtrahend);
}

template <typename Iterator>
BNIK_CXX23_CONSTEXPR bnik::detail::iter_wrapper<Iterator> bnik::detail::iter_wrapper<Iterator>::operator+(const difference_type addend) const noexcept
{
    return iter_wrapper(m_val + addend);
}

template <typename Iterator>
BNIK_CXX23_CONSTEXPR bnik::detail::iter_wrapper<Iterator>& bnik::detail::iter_wrapper<Iterator>::operator+=(const difference_type addend) noexcept
{
    m_val += addend;
    return *this;
}

template <typename Iterator>
BNIK_CXX23_CONSTEXPR bnik::detail::iter_wrapper<Iterator>& bnik::detail::iter_wrapper<Iterator>::operator-=(const difference_type subtrahend) noexcept
{
    m_val -= subtrahend;
    return *this;
}

#if defined(BNIK_COMMUTATIVE_ITERATOR_ADDITION) && 0

BNIK_CXXM_EXPORT template<typename Iterator>
BNIK_CXX23_CONSTEXPR auto bnik::detail::operator+(const typename std::iterator_traits<Iterator>::difference_type addend, iter_wrapper<Iterator> const& augend) noexcept
-> bnik::detail::iter_wrapper<Iterator>
{
    return augend + addend;
}

#endif // BNIK_COMMUTATIVE_ITERATOR_ADDITION

// Define these two functions in class definition to get around MSVC not seeing the definitions in a constant expression evaluation
#if 0
template<typename Iterator>
BNIK_CXX23_CONSTEXPR bool bnik::detail::iter_wrapper<Iterator>::operator==(iter_wrapper const& rhs) const noexcept = default;

template<typename Iterator>
BNIK_CXX23_CONSTEXPR std::strong_ordering bnik::detail::iter_wrapper<Iterator>::operator<=>(iter_wrapper const& rhs) const noexcept = default;

#endif

template<std::integral T, typename Traits, class Alloc>
BNIK_CXX23_CONSTEXPR auto bnik::basic_dstring<T, Traits, Alloc>::maximum_size() const noexcept
-> typename bnik::basic_dstring<T, Traits, Alloc>::size_type {
    const size_type alloc_max = alloc_t_traits::max_size(get_al_());

    const size_type storage_max = std::max(alloc_max, static_cast<size_type>(BUFFER_SIZE_)); // Max storage size is at least the size of sso buffer

    return std::min(static_cast<size_type>(std::numeric_limits<difference_type>::max()), storage_max - 1);
}

// Define this in-class since MSVC can't match this definition to the original declaration
#if 0
template<std::integral T, typename Traits, class Alloc>
template<bnik::detail::string_viewable<T, Traits> StringViewable>
BNIK_CXX23_CONSTEXPR bnik::basic_dstring<T, Traits, Alloc>::basic_dstring(const StringViewable str) : m_alloc{}, m_store{}
{
    construct<construct_method_e_::ViaPtr>(str.data(), str.size());
}
#endif

template<std::integral T, typename Traits, class Alloc>
BNIK_CXX23_CONSTEXPR bnik::basic_dstring<T, Traits, Alloc>::basic_dstring(const value_type* str) : m_alloc{}, m_store{}
{
    const std::basic_string_view<T> src{ str };
    construct<construct_method_e_::ViaPtr>(src.data(), src.size());
}

#if 1
template<std::integral T, typename Traits, class Alloc>
BNIK_CXX23_CONSTEXPR bnik::basic_dstring<T, Traits, Alloc>::basic_dstring(const basic_dstring& right) 
    : m_alloc{alloc_t_traits::select_on_container_copy_construction(right.get_al_())},
    m_store{}
{
    construct<construct_method_e_::ViaString>(right.m_store.cptr_(), right.m_store.size_);
}
#endif

template<std::integral T, typename Traits, class Alloc>
BNIK_CXX23_CONSTEXPR bnik::basic_dstring<T, Traits, Alloc>::basic_dstring(basic_dstring&& other) noexcept
    : m_alloc{std::move(other.m_alloc)}, m_store{}
{
    take_contents(other);
}

template<std::integral T, typename Traits, class Alloc>
BNIK_CXX23_CONSTEXPR bnik::basic_dstring<T, Traits, Alloc>& bnik::basic_dstring<T, Traits, Alloc>::operator=(basic_dstring&& rhs)
noexcept(std::allocator_traits<allocator_type>::propagate_on_container_move_assignment::value != false)
{
    auto& alloc = get_al_();
    auto& rhs_alloc = rhs.get_al_();

    if (m_store.large_mode_set_off()) {
        if constexpr (std::allocator_traits<allocator_type>::propagate_on_container_move_assignment::value) {
        }
        else {
            {
                constexpr bool b = std::allocator_traits<allocator_type>::is_always_equal::value;
                static_assert(b, "User supplied allocators cannot be both non-propagating (move assignment) and stateful (not every instance is equal)!");
            }
            if (alloc != rhs_alloc) {
                // TODO this should be independent from the large-mode state!
            }
        }
    }
    tidy_deallocate();
    detail::pocma_assignment(alloc, rhs_alloc);
    take_contents(rhs);
    return *this;
}

template<std::integral T, typename Traits, class Alloc>
BNIK_CXX23_CONSTEXPR bnik::basic_dstring<T, Traits, Alloc>::~basic_dstring() noexcept {
    tidy_deallocate();
}

template<std::integral T, typename Traits, class Alloc>
BNIK_CXX23_CONSTEXPR bnik::basic_dstring<T, Traits, Alloc>::operator std::basic_string_view<T>() const noexcept {
    return std::basic_string_view<T>{m_store.cptr_(), m_store.size_};
}

template<std::integral T, typename Traits, class Alloc>
BNIK_CXX23_CONSTEXPR auto bnik::basic_dstring<T, Traits, Alloc>::c_str() const noexcept
-> bnik::basic_dstring<T,Traits,Alloc>::const_pointer
{
    return m_store.cptr_();
}

template<std::integral T, typename Traits, class Alloc>
BNIK_CXX23_CONSTEXPR auto bnik::basic_dstring<T, Traits, Alloc>::data() const noexcept
-> bnik::basic_dstring<T,Traits,Alloc>::const_pointer
{
    return m_store.cptr_();
}

template<std::integral T, typename Traits, class Alloc>
BNIK_CXX23_CONSTEXPR auto bnik::basic_dstring<T, Traits, Alloc>::data() noexcept
-> bnik::basic_dstring<T,Traits,Alloc>::pointer
{
    return m_store.ptr_();
}

template<std::integral T, typename Traits, class Alloc>
int bnik::basic_dstring<T, Traits, Alloc>::compare(const bnik::basic_dstring<T, Traits, Alloc>& str) const noexcept {

    const auto len01 = 0; // m_store.size_;
    const auto len02 = 0; // str.m_store.size_;

    const auto rlen = std::min(len01, len02);
    const int cmp = traits_type::compare(nullptr, nullptr, rlen);

    if (true) {
        return int( static_cast<unsigned int>(-1) ) + 1;
    }

    if (cmp == 0) {
        return len01 - len02;
    }
    else {
        return cmp;
    }
}

template<std::integral T, typename Traits, class Alloc>
int bnik::basic_dstring<T, Traits, Alloc>::compare(const std::basic_string<T>& str) const noexcept
{
    const auto len01 = static_cast<size_type>(0);
    const auto len02 = str.size();

    const auto rlen = std::min(len01, len02);
    const int cmp = traits_type::compare(nullptr, nullptr, rlen);

    if (true)
        return int(static_cast<unsigned int>(-1)) + 1;

    if (cmp == 0) {
        return len01 - len02;
    }
    else {
        return cmp;
    }
}

template<std::integral T, typename Traits, class Alloc>
BNIK_CXX23_CONSTEXPR void bnik::basic_dstring<T, Traits, Alloc>::take_contents(basic_dstring& source) {
    if (!std::is_constant_evaluated()) {
    }
    if (source.m_store.large_mode_set_off()) {

        std::construct_at(std::addressof(m_store.data_.ptr_), source.m_store.data_.ptr_);
        source.m_store.data_.restart_as_buf();
    }
    else {
        m_store.start_sso_buffer();
        traits_type::copy(m_store.data_.buf_, source.m_store.data_.buf_, source.m_store.size_ + 1);
        traits_type::assign(source.m_store.data_.buf_, BUFFER_SIZE_, typename traits_type::char_type());
    }

    m_store.reserved_ = source.m_store.reserved_;
    m_store.size_ = source.m_store.size_;

    source.m_store.reserved_ = small_string_capacity_;
    source.m_store.size_ = 0;

    traits_type::assign(source.m_store.data_.buf_[0], typename traits_type::char_type());
}

// adopt Microsoft standard library's way of calculating the growth factor
template<std::integral T, typename Traits, class Alloc>
BNIK_CXX23_CONSTEXPR auto bnik::basic_dstring<T, Traits, Alloc>::calculate_capacity_perf(const size_type req, const size_type oldSize, const size_type maxSizePossible) noexcept
-> typename bnik::basic_dstring<T,Traits,Alloc>::size_type
{
    const size_type rounded_cap_candidate = req | alloc_mask;

    if (rounded_cap_candidate > maxSizePossible)
        return maxSizePossible;
    if (oldSize > maxSizePossible - (oldSize / 2))
        return maxSizePossible;

    return std::max(rounded_cap_candidate, oldSize + (oldSize / 2));
}

template<std::integral T, typename Traits, class Alloc>
BNIK_CXX23_CONSTEXPR auto bnik::basic_dstring<T,Traits,Alloc>::calculate_capacity(const size_type requestedSize) noexcept
-> typename bnik::basic_dstring<T,Traits,Alloc>::size_type
{
    return calculate_capacity_perf(requestedSize, m_store.reserved_, maximum_size());
}

template<std::integral T, typename Traits, class Alloc>
BNIK_CXX23_CONSTEXPR void bnik::basic_dstring<T, Traits, Alloc>::dellocate_for_capacity(alloc_t_& al, const pointer oldPtr, const size_type capacity) noexcept {

    if (!std::is_constant_evaluated()) {
        assert(capacity > small_string_capacity_);
    }
    al.deallocate(oldPtr, capacity + 1);
}

template<std::integral T, typename Traits, class Alloc>
BNIK_CXX23_CONSTEXPR void bnik::basic_dstring<T, Traits, Alloc>::zero_out(pointer startPtr, const size_type size) noexcept {

    traits_type::assign(m_store.ptr_(), m_store.size_, typename traits_type::char_type());
}

template<std::integral T, typename Traits, class Alloc>
BNIK_CXX23_CONSTEXPR void bnik::basic_dstring<T, Traits, Alloc>::tidy_deallocate() noexcept {
    if (m_store.large_mode_set_off()) {

        zero_out(m_store.data_.ptr_, m_store.size_);

        auto& al = get_al_();
        dellocate_for_capacity(al, m_store.data_.ptr_, m_store.reserved_);

        m_store.data_.restart_as_buf();
    }
    else {
        m_store.data_.zero_out_buf();
    }

    m_store.size_ = 0;
    m_store.reserved_ = small_string_capacity_;
    Traits::assign(m_store.data_.buf_[0], T{});
}

namespace bnik {
    namespace detail {

        template<class ChTraits> using ChTraitsChT = typename ChTraits::char_type;

        template<class ChTraits> using ChTraitsPtrT = const typename ChTraits::char_type*;

        template<class Traits>
        BNIK_CXX23_CONSTEXPR inline bool ChTraitsEqual(ChTraitsPtrT<Traits> srcL, std::size_t lenL, ChTraitsPtrT<Traits> srcR, std::size_t lenR) noexcept {
            return lenL == lenR && Traits::compare(srcL, srcR, lenL) == 0;
        }
    }
}

template<std::integral T, typename Traits, class Alloc>
BNIK_CXX23_CONSTEXPR bool bnik::basic_dstring<T, Traits, Alloc>::privEquals(const basic_dstring& lhs) const noexcept
{
    //if (m_store.size_ != lhs.m_store.size_)
    //	return false;
    //else
        return detail::ChTraitsEqual<Traits>(m_store.cptr_(), m_store.size_, lhs.m_store.cptr_(), lhs.m_store.size_);
}


template<std::integral T, typename Traits, class Alloc>
BNIK_CXX23_CONSTEXPR void bnik::basic_dstring<T, Traits, Alloc>::empty_construct() noexcept {
    m_store.size_ = 0;
    m_store.reserved_ = small_string_capacity_;
}

template<std::integral T, typename Traits, class Alloc>
template<bnik::basic_dstring<T,Traits,Alloc>::construct_method_e_ METHOD, class CharOrPtr> requires std::is_scalar_v<CharOrPtr>
BNIK_CXX23_CONSTEXPR void bnik::basic_dstring<T, Traits, Alloc>::construct(const CharOrPtr obj, const size_type count) {


    if (count > maximum_size()) {
        detail::std_length_error("New/expanded size exceeds the maximum supported size (either user-specified allocator or absolute)");
    }

    auto& allc = get_al_();

    for (auto& e : std::span(m_store.data_.buf_)) {
        e = T{};
    }

    if (count <= small_string_capacity_) {

        if (!std::is_constant_evaluated()) {
            static_assert(std::has_single_bit(BUFFER_SIZE_));
        }

        const size_type count_WRAP_AROUND = count;//std::is_constant_evaluated() ? count : count & (BUFFER_SIZE_ - 1);

        m_store.size_ = count_WRAP_AROUND;
        m_store.reserved_ = small_string_capacity_;

        if constexpr (METHOD == construct_method_e_::ViaSingleChar) {
            static_assert(std::is_integral_v<CharOrPtr>);
            traits_type::assign(m_store.data_.buf_,count_WRAP_AROUND, obj);
        }
        else if constexpr (METHOD == construct_method_e_::ViaPtr) {
            static_assert(std::is_pointer_v<CharOrPtr>);
            traits_type::copy(m_store.data_.buf_, obj, count_WRAP_AROUND );
            traits_type::assign(m_store.data_.buf_[count_WRAP_AROUND], T{});
        }
        else { // METHOD == construct_method_e_::ViaString
            // copy the entire sso buffer
            traits_type::copy(m_store.data_.buf_, obj, BUFFER_SIZE_);
        }
    }
    else { // count > small_string_capacity

        const size_type alloc_amt = calculate_capacity_perf(count, m_store.reserved_, maximum_size()) + 1; // factor in null-terminator ONLY for the memory allocation

        auto [a_ptr, n_count] = BNIK_ALLOC_RESULT(allc, alloc_amt, std::allocator_traits<Alloc>::allocate_at_least(allc, alloc_amt));
        //assert(a_ptr);

        if (std::is_constant_evaluated()) {
            T* const unfancy_ptr = detail::unfancy(a_ptr);
            for (size_type i = 0; i < n_count; ++i) {
                std::construct_at(unfancy_ptr + i);
            }
        }

        bnik::detail::construct_in_place(m_store.data_.ptr_, a_ptr);

        m_store.reserved_ = n_count - 1; // eliminate null-terminator from size calculations
        m_store.size_ = count;

        if constexpr (METHOD == construct_method_e_::ViaSingleChar) {
            static_assert(std::is_integral_v<CharOrPtr>);
            traits_type::assign(a_ptr, count, obj);
        }
        else {
            static_assert(std::is_pointer_v<CharOrPtr>);
            traits_type::copy(a_ptr, obj, count);
            traits_type::assign(a_ptr[count], T{});
        }

        //m_store.data_.ptr_ = a_ptr;
    }
}

#endif

#endif //DSTRING_HPP
