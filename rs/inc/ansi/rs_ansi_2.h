#pragma once
#include <array>


#define ESC '\x1b'
#define CSI '['



template<typename T, size_t N>
struct consteval_array {
    using array_type = std::array<T, N>;
    using value_type = T;


    consteval consteval_array (void) = delete;
    consteval consteval_array (const value_type(&c_array)[N]) noexcept {
        for (size_t i = 0; i < N; i++) {
            (*this)[i] = c_array[i];
        }
    }
    consteval consteval_array (value_type(&& c_array)[N]) noexcept {
        for (size_t i = 0; i < N; i++) {
            (*this)[i] = std::move(c_array[i]);
        }
    }


    constexpr const array_type& get(void) const noexcept {
        return this->m_array;
    }
    constexpr array_type& get(void) noexcept {
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


    template<size_t N_RHS, typename _ResultType = consteval_array<T, N + N_RHS>>
    consteval _ResultType operator + (const consteval_array<T, N_RHS>& rhs) const noexcept {
        auto seq = _ResultType({});
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
    constexpr auto a1 = a0;
    constexpr auto a2 = a1 + consteval_array({ 'e','l' });
    constexpr auto a3 = a2 + consteval_array("lo");

    return _private::consteval_memcmp(a3->data(), "hello", 6);
}(), "consteval_array: error");

template<size_t N>
using consteval_char_array = consteval_array<char, N>;



// {'\x1b',...}
template<size_t _N, typename _BaseType = consteval_char_array<1+_N>>
struct esc_sequence : _BaseType {
    using base_type  = _BaseType;
    using value_type = base_type::value_type;

    consteval esc_sequence (const value_type (&&c_array)[_N])
        : base_type ({ESC})
    {
        size_t idx = 1;
        for (size_t i = 0; i < _N; i++) {
            this->m_array[idx++] = std::move(c_array[i]);
        }
    }
};
static_assert(2 == sizeof(esc_sequence({' '})), "esc_sequence: size error");

// {'\x1b','[',...}
template<size_t _N, typename _BaseType = esc_sequence<1+_N>>
struct csi_sequence : _BaseType {
    using base_type = _BaseType;
    using value_type = base_type::value_type;

    consteval csi_sequence (const value_type (&&c_array)[_N]) noexcept
        : base_type ({CSI})
    {
        size_t idx = 2;
        for (size_t i = 0; i < _N; i++) {
            this->m_array[idx++] = std::move(c_array[i]);
        }
    }
};

// {'\x1b','[',...,'m'}
template<size_t _N, typename _BaseType = csi_sequence<_N+1>>
struct sgr_sequence : _BaseType {
    using base_type = _BaseType;

private:
    static consteval auto generate_base_type_param (const char (&&c_array)[_N]) noexcept {
        auto arr = consteval_char_array<_N>({});
    }

public:
    consteval sgr_sequence (const char (&&c_array)[_N]) noexcept
        : base_type ({})
    {
        for (size_t i = 0; i < _N; i++) {
            (*this)[2 + i] = std::move(c_array[i]);
        }

        (*this)[2 + _N] = 'm';
    }

    // Append a string literal.
    template<size_t _N_RHS, typename _ResultType = consteval_char_array<_N+_N_RHS>>
    consteval _ResultType operator + (const char (&rhs)[_N_RHS]) const noexcept {
        auto seq = _ResultType({});
        size_t idx = 0;

        for (size_t i = 0; i < _N; i++) {
            seq[idx++] = this->m_array[i];
        }
        for (size_t i = 0; i < _N_RHS; i++) {
            seq[idx++] = rhs[i];
        }

        return seq;
    }

    // Combine SGR parameters.
    template<size_t _N_RHS, typename _ResultType = sgr_sequence<_N+1+_N_RHS>>
    consteval _ResultType operator + (const sgr_sequence<_N_RHS>& rhs) const noexcept {
        auto seq = _ResultType({});
        size_t idx = 2;

        for (size_t i = 0; i < _N; i++) {
            seq[idx++] = this->m_array[2 + i];
        }
        seq[idx++] = ';';
        for (size_t i = 0; i < _N_RHS; i++) {
            seq[idx++] = rhs[2 + i];
        }
        seq[idx++] = 'm';

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


static constexpr auto FG_RED        = sgr_sequence({'3','1'});
static constexpr auto FG_GREEN      = sgr_sequence({'3','2'});
static constexpr auto FG_DEFAULT    = sgr_sequence({'3','9'});
static constexpr auto BG_RED        = sgr_sequence({'4','1'});
static constexpr auto BG_GREEN      = sgr_sequence({'4','2'});
static constexpr auto BG_DEFAULT    = sgr_sequence({'4','9'});


// Make a string literal red.
template <bool ResetColor = true, size_t N>
consteval auto red (const char (&string_literal)[N]) {

    // using _ResetT    = consteval_char_array<FG_RED.size() + (N - 1) + FG_DEFAULT.size() + 1 /* yes, i know they cancel. */>;
    // using _NonResetT = sgr_sequence<FG_RED.size() + N>;
    // using _BaseSequenceT = std::conditional_t<ResetColor, _ResetT, _NonResetT>;
    // 
    // auto seq = _BaseSequenceT({});
    // size_t idx = 0;
    // 
    // for (size_t i = 0; i < FG_RED.size(); i++) {
    //     seq[idx++] = FG_RED[i];
    // }
    // for (size_t i = 0; i < N - 1; i++) {
    //     seq[idx++] = string_literal[i];
    // }
    // if constexpr (ResetColor) {
    //     for (size_t i = 0; i < FG_DEFAULT.size(); i++) {
    //         seq[idx++] = FG_DEFAULT[i];
    //     }
    // }
    // seq[idx] = '\0';
    // 
    // return seq;

    auto seq = FG_RED + string_literal;

    if constexpr (ResetColor) {
        return seq + FG_DEFAULT;
    }
    else {
        return seq;
    }
}

static constexpr auto FG_RED_BG_GREEN = FG_RED + BG_GREEN;












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
