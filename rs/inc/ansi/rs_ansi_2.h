#pragma once
#include <array>


#define ESC '\x1b'
#define CSI '['



template<typename T, size_t N>
struct consteval_array {
    using array_type = std::array<T, N>;
    using value_type = T;

    consteval consteval_array (void) noexcept
        : m_array {}
    {};
    consteval consteval_array (const value_type (&c_array)[N]) noexcept {
        for (size_t i = 0; i < N; i++) {
            this->m_array[i] = c_array[i];
        }
    }
    consteval consteval_array (value_type (&&c_array)[N]) noexcept {
        for (size_t i = 0; i < N; i++) {
            this->m_array[i] = std::move(c_array[i]);
        }
    }


    constexpr const array_type& get (void) const noexcept {
        return this->m_array;
    }
    constexpr array_type& get (void) noexcept {
        return this->m_array;
    }

    constexpr const array_type* const operator -> (void) const noexcept {
        return &this->m_array;
    }
    constexpr array_type* const operator -> (void) noexcept {
        return &this->m_array;
    }

    constexpr const value_type& operator [] (size_t index) const noexcept {
        return this->m_array[index];
    }
    constexpr value_type& operator [] (size_t index) noexcept {
        return this->m_array[index];
    }


    template<size_t N_RHS>
    consteval auto operator + (const consteval_array<T, N_RHS>& rhs) const noexcept {
        auto seq = consteval_array<T,N+N_RHS>({});
        size_t idx = 0;

        for (size_t i = 0; i < N; i++) {
            seq[idx++] = this->m_array[i];
        }
        for (size_t i = 0; i < N_RHS; i++) {
            seq[idx++] = rhs[i];
        }

        return seq;
    }

protected:
    array_type m_array;
};
static_assert([]{
    using namespace rs::util;

    constexpr auto a0 = consteval_array({ 'h' });
    if (sizeof(a0) != 1) {
        return false;
    }

    constexpr auto a1 = consteval_array({ 'e','l' });
    if (sizeof(a1) != 2) {
        return false;
    }

    constexpr auto a2 = a0 + a1;
    if (sizeof(a2) != 3) {
        return false;
    }

    constexpr auto a3 = a2 + consteval_array("lo");
    if (sizeof(a3) != 6) {
        return false;
    }

    return _private::consteval_memcmp(a3->data(), "hello", 6);
}(), "consteval_array: error");

template<size_t N>
using consteval_char_array = consteval_array<char, N>;


// Special consteval_char_array which is always null-terminated.
template<size_t _N, typename _BaseType = consteval_char_array<_N>>
struct consteval_cstring : _BaseType {
    using base_type  = _BaseType;
    using value_type = base_type::value_type;

    consteval consteval_cstring (void)
        : base_type ()
    {}

    consteval consteval_cstring (const value_type (&string_literal)[_N])
        : consteval_cstring ()
    {
        for (size_t i = 0; i < _N; i++) {
            this->m_array[i] = string_literal[i];
        }

        static_assert(string_literal[_N - 1] == '\0');
        static_assert(this->m_array[_N - 1] == '\0');
    }


    template<size_t N_RHS>
    consteval auto operator + (const consteval_char_array<N_RHS>& rhs) const noexcept {
        auto str = consteval_cstring<_N+N_RHS>{};

        size_t idx = 0;
        for (size_t i = 0; i < _N - 1; i++) {   // add self, skipping null-terminator
            str[idx++] = this->m_array[i];
        }
        for (size_t i = 0; i < N_RHS; i++) {    // add rhs
            str[idx++] = rhs[i];
        }
        str[idx] = '\0';                        // place null-terminator at end

        return str;
    }

    template<size_t N_RHS>
    consteval auto operator + (const value_type (&string_literal)[N_RHS]) const noexcept {
        return (*this) + consteval_cstring(string_literal);
    }

    template<size_t N_RHS>
    consteval auto operator + (const consteval_cstring<N_RHS>& other) const noexcept {
        auto str = consteval_cstring<_N+N_RHS-1>{};

        size_t idx = 0;
        for (size_t i = 0; i < _N - 1; i++) {   // add self, skipping null-terminator
            str[idx++] = this->m_array[i];
        }
        for (size_t i = 0; i < N_RHS; i++) {    // add other, including its null-terminator
            str[idx++] = other[i];
        }

        return str;
    }
};



// {'\x1b',...}
template<size_t _N, typename _BaseType = consteval_char_array<1+_N>>
struct esc_sequence : _BaseType {
    using base_type  = _BaseType;
    using value_type = base_type::value_type;

    consteval esc_sequence (void) noexcept
        : base_type ({ESC})
    {}

    consteval esc_sequence (const value_type (&&c_array)[_N])
        : esc_sequence ()
    {
        size_t idx = 1;
        for (size_t i = 0; i < _N; i++) {
            this->m_array[idx++] = std::move(c_array[i]);
        }
    }
};

// {'\x1b','[',...}
template<size_t _N, typename _BaseType = esc_sequence<1+_N>>
struct csi_sequence : _BaseType {
    using base_type = _BaseType;
    using value_type = base_type::value_type;

    consteval csi_sequence (void) noexcept
        : base_type ({CSI})
    {}

    consteval csi_sequence (const value_type (&string_literal)[_N]) noexcept
        : csi_sequence ()
    {
        size_t idx = 2;
        for (size_t i = 0; i < _N; i++) {
            this->m_array[idx++] = string_literal[i];
        }
    }
};

// {'\x1b','[',...,'m'}
template<size_t _N, typename _BaseType = csi_sequence<_N+1>>
struct sgr_sequence : _BaseType {
    using base_type = _BaseType;
    using value_type = base_type::value_type;

    consteval sgr_sequence (void) = delete;
    consteval sgr_sequence (const value_type (&string_literal)[_N]) = delete;
    consteval sgr_sequence (const value_type (&&sgr_params)[_N]) noexcept
        : base_type ({})
    {
        size_t idx = 2;

        for (size_t i = 0; i < _N; i++) {
            this->m_array[idx++] = std::move(sgr_params[i]);
        }
        this->m_array[idx] = 'm';
    }


    // Append a string literal.
    template<size_t N_RHS>
    consteval auto operator + (const value_type (&string_literal)[N_RHS]) const noexcept {
        auto seq = consteval_cstring<2+_N+1+N_RHS>();
        size_t idx = 0;

        for (size_t i = 0; i < 2+_N+1; i++) {
            seq[idx++] = this->m_array[i];
        }
        for (size_t i = 0; i < N_RHS; i++) {
            seq[idx++] = string_literal[i];
        }

        return seq;
    }

    // Combine SGR parameters.
    template<size_t N_RHS>
    consteval auto operator + (const sgr_sequence<N_RHS>& other) const noexcept {
        auto seq = sgr_sequence<_N+1+N_RHS>({});
        size_t idx = 2;

        for (size_t i = 0; i < _N; i++) {
            seq[idx++] = this->m_array[2 + i];
        }
        seq[idx++] = ';';
        for (size_t i = 0; i < N_RHS; i++) {
            seq[idx++] = other[2 + i];
        }
        seq[idx] = 'm';

        return seq;
    }
};
static_assert([]{
    constexpr auto a0 = sgr_sequence({'3','1'});
    if (sizeof(a0) != 5) {
        return false;
    }

    constexpr auto a1 = sgr_sequence({'4','2'});
    if (sizeof(a1) != 5) {
        return false;
    }

    constexpr auto a2 = a0 + a1;
    return sizeof(a2) == 8;
}(), "sgr_sequence: logic error");


static constexpr auto SGR_RESET         = sgr_sequence({'0'});
static constexpr auto SGR_FG_RED        = sgr_sequence({'3','1'});
static constexpr auto SGR_FG_GREEN      = sgr_sequence({'3','2'});
static constexpr auto SGR_FG_DEFAULT    = sgr_sequence({'3','9'});
static constexpr auto SGR_BG_RED        = sgr_sequence({'4','1'});
static constexpr auto SGR_BG_GREEN      = sgr_sequence({'4','2'});
static constexpr auto SGR_BG_DEFAULT    = sgr_sequence({'4','9'});


// Make a string literal red.
template <bool ResetColor = true, size_t N>
consteval auto red (const char (&string_literal)[N]) {
    auto str = SGR_FG_RED + string_literal;

    if constexpr (ResetColor) {
        return str + SGR_RESET;
    }
    else {
        return str;
    }
}

template<size_t N>
void debug_print (const consteval_char_array<N>& char_array) {
    size_t i = 0;
    for (const char c : char_array.get()) {
        printf("%u) %d\n", i++, c);
    }
}










template<typename T, size_t N>
struct my_arr : std::array<T, N> {
    
    // Standard ctors.
    consteval my_arr (void) = delete;

    consteval my_arr (const my_arr<T,N>& other) noexcept 
        : std::array<T,N>(other)
    {}

    consteval my_arr (my_arr<T,N>&& other) noexcept
        : std::array<T,N>(std::move(other))
    {}


    // Custom ctors.
    consteval my_arr (const T (&other)[N]) noexcept {
        for (size_t i = 0; i < N; i++) {
            this->_Elems[i] = other[i];
        }
    }

    consteval my_arr (T (&&other)[N]) noexcept {
        for (size_t i = 0; i < N; i++) {
            this->_Elems[i] = std::move(other[i]);
        }
    }


    // Operators.
    template<size_t N_OTHER>
    consteval my_arr<T, N + N_OTHER> append (const my_arr<T, N_OTHER>& other) const noexcept {
        my_arr<T, N + N_OTHER> arr = {};
        size_t idx = 0;

        for (size_t i = 0; i < N; i++) {
            arr[idx++] = this->_Elems[i];
        }
        for (size_t i = 0; i < N_OTHER; i++) {
            arr[idx++] = other[i];
        }

        return arr;
    }
};

template<size_t N>
using my_char_arr = my_arr<char, N>;

//static constexpr auto a = my_char_arr({'H','e','l','l','o'}).append(my_char_arr({ 'H','e','l','l','o' }));
