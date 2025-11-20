#pragma once
#include <array>

#define ENABLE_CLASS_STATIC_ASSERTS





/* A compile-mutable array */

template<typename T, size_t N>
struct ce_array
{
    using array_type = std::array<T, N>;

    consteval ce_array (void) noexcept
        : m_array {}
    {}

    consteval ce_array (const T (&c_array)[N]) noexcept
    {
        for (size_t i = 0; i < N; i++)
        {
            m_array[i] = c_array[i];
        }
    }
    consteval ce_array (T (&&c_array)[N]) noexcept
    {
        for (size_t i = 0; i < N; i++)
        {
            m_array[i] = std::move(c_array[i]);
        }
    }


    constexpr const array_type& get (void) const noexcept
    {
        return m_array;
    }
    constexpr array_type& get (void) noexcept
    {
        return m_array;
    }

    constexpr const array_type* const operator -> (void) const noexcept
    {
        return &m_array;
    }
    constexpr array_type* const operator -> (void) noexcept
    {
        return &m_array;
    }

    constexpr const T& operator [] (size_t index) const noexcept
    {
        return m_array[index];
    }
    constexpr T& operator [] (size_t index) noexcept
    {
        return m_array[index];
    }


    template<size_t N_OTHER>
    consteval auto operator + (const ce_array<T, N_OTHER>& other) const noexcept
    {
        auto arr = ce_array<T, N + N_OTHER>();
        size_t idx = 0;

        for (size_t i = 0; i < N; i++)
        {
            arr[idx++] = (*this)[i];
        }
        for (size_t i = 0; i < N_OTHER; i++)
        {
            arr[idx++] = other[i];
        }

        return arr;
    }

private:
    array_type m_array;
};
#ifdef ENABLE_CLASS_STATIC_ASSERTS
static_assert([]{
    using namespace rs::util;

    constexpr auto a0 = ce_array({ 'h' });
    if (sizeof(a0) != 1) {
        return false;
    }

    constexpr auto a1 = ce_array({ 'e','l' });
    if (sizeof(a1) != 2) {
        return false;
    }

    constexpr auto a2 = a0 + a1;
    if (sizeof(a2) != 3) {
        return false;
    }

    constexpr auto a3 = a2 + ce_array("lo");
    if (sizeof(a3) != 6) {
        return false;
    }

    constexpr const char EXPECTED [] = "hello";
    return _private::consteval_memcmp(a3->data(), EXPECTED, sizeof(EXPECTED));
}(), "ce_array: error");
#endif

template<size_t N>
using ce_char_array = ce_array<char, N>;





/* A compile-mutable string literal */

template<size_t N>
struct ce_cstring : ce_char_array<N>
{
    using base_type = ce_char_array<N>;

    consteval ce_cstring (void)
        : base_type ()
    {}

    consteval ce_cstring (const char (&string_literal)[N])
        : ce_cstring ()
    {
        for (size_t i = 0; i < N; i++)
        {
            (*this)[i] = string_literal[i];
        }

        static_assert(string_literal[N - 1] == '\0');
        static_assert((*this)[N - 1] == '\0');
    }


    template<size_t N_RHS>
    consteval auto operator + (const ce_char_array<N_RHS>& rhs) const noexcept
    {
        auto str = ce_cstring<N+N_RHS>();

        size_t idx = 0;
        for (size_t i = 0; i < N - 1; i++)  // add self, skipping null-terminator
        {
            str[idx++] = (*this)[i];
        }
        for (size_t i = 0; i < N_RHS; i++)  // add rhs
        {
            str[idx++] = rhs[i];
        }
        str[idx] = '\0';                    // place null-terminator at end

        return str;
    }

    template<size_t N_RHS>
    consteval auto operator + (const char (&string_literal)[N_RHS]) const noexcept
    {
        return (*this) + ce_cstring(string_literal);
    }

    template<size_t N_RHS>
    consteval auto operator + (const ce_cstring<N_RHS>& other) const noexcept
    {
        auto str = ce_cstring<N+N_RHS-1>();

        size_t idx = 0;
        for (size_t i = 0; i < N - 1; i++)  // add self, skipping null-terminator
        {
            str[idx++] = (*this)[i];
        }
        for (size_t i = 0; i < N_RHS; i++)  // add other, including its null-terminator
        {
            str[idx++] = other[i];
        }

        return str;
    }
};





/* ANSI-sequence building blocks */

template<size_t N>
struct esc_sequence : ce_char_array<1+N>
{
    using base_type = ce_char_array<1+N>;

    consteval esc_sequence (void) noexcept
        : base_type ({'\x1b'})
    {}

    consteval esc_sequence (const char (&&params)[N])
        : esc_sequence ()
    {
        size_t idx = 1;
        for (size_t i = 0; i < N; i++)
        {
            (*this)[idx++] = std::move(params[i]);
        }
    }
};

template<size_t N>
struct csi_sequence : esc_sequence<1+N>
{
    using base_type = esc_sequence<1+N>;

    consteval csi_sequence (void) noexcept
        : base_type ({'['})
    {}

    consteval csi_sequence (const char (&string_literal)[N]) noexcept
        : csi_sequence ()
    {
        size_t idx = 2;
        for (size_t i = 0; i < N; i++)
        {
            (*this)[idx++] = string_literal[i];
        }
    }
};

template<size_t N>
struct sgr_sequence : csi_sequence<N+1>
{
    using base_type = csi_sequence<N+1>;

    consteval sgr_sequence (void) noexcept
        : base_type ()
    {}

    consteval sgr_sequence (const char (&&sgr_params)[N]) noexcept
        : base_type ()
    {
        size_t idx = 2;

        for (size_t i = 0; i < N; i++)
        {
            (*this)[idx++] = std::move(sgr_params[i]);
        }
        (*this)[idx] = 'm';
    }


    // Append a string literal.
    template<size_t N_RHS>
    consteval auto operator + (const char (&string_literal)[N_RHS]) const noexcept
    {
        auto str = ce_cstring<2+N+1+N_RHS>();
        size_t idx = 0;

        for (size_t i = 0; i < 2+N+1; i++)
        {
            str[idx++] = (*this)[i];
        }
        for (size_t i = 0; i < N_RHS; i++)
        {
            str[idx++] = string_literal[i];
        }

        return str;
    }

    // Combine SGR parameters.
    template<size_t N_RHS>
    consteval auto operator + (const sgr_sequence<N_RHS>& other) const noexcept
    {
        auto seq = sgr_sequence<N+1+N_RHS>();
        size_t idx = 2;

        for (size_t i = 0; i < N; i++)
        {
            seq[idx++] = (*this)[2 + i];
        }
        seq[idx++] = ';';
        for (size_t i = 0; i < N_RHS; i++)
        {
            seq[idx++] = other[2 + i];
        }
        seq[idx] = 'm';

        return seq;
    }
};
#ifdef ENABLE_CLASS_STATIC_ASSERTS
static_assert([]{
    using namespace rs::util;

    constexpr const char EXPECTED0 [] = "\x1b[31m";
    constexpr const char EXPECTED1 [] = "\x1b[42m";
    constexpr const char EXPECTED2 [] = "\x1b[31;42m";

    constexpr auto a0 = sgr_sequence({'3','1'});
    static_assert(sizeof(a0) == 5);
    if (!_private::consteval_memcmp(a0->data(), EXPECTED0, sizeof(EXPECTED0) - 1))
    {
        return false;
    }

    constexpr auto a1 = sgr_sequence({'4','2'});
    static_assert(sizeof(a1) == 5);
    if (!_private::consteval_memcmp(a1->data(), EXPECTED1, sizeof(EXPECTED1) - 1))
    {
        return false;
    }

    constexpr auto a2 = a0 + a1;
    static_assert(sizeof(a2) == 8);
    return _private::consteval_memcmp(a2->data(), EXPECTED2, sizeof(EXPECTED2) - 1);
}(), "sgr_sequence: logic error");
#endif





/* Common SGR sequences */

static constexpr auto SGR_RESET         = sgr_sequence({'0'});

static constexpr auto SGR_FG_BLACK      = sgr_sequence({'3','0'});
static constexpr auto SGR_FG_RED        = sgr_sequence({'3','1'});
static constexpr auto SGR_FG_GREEN      = sgr_sequence({'3','2'});
static constexpr auto SGR_FG_YELLOW     = sgr_sequence({'3','3'});
static constexpr auto SGR_FG_BLUE       = sgr_sequence({'3','4'});
static constexpr auto SGR_FG_MAGENTA    = sgr_sequence({'3','5'});
static constexpr auto SGR_FG_CYAN       = sgr_sequence({'3','6'});
static constexpr auto SGR_FG_DEFAULT    = sgr_sequence({'3','9'});

static constexpr auto SGR_BG_BLACK      = sgr_sequence({'4','0'});
static constexpr auto SGR_BG_RED        = sgr_sequence({'4','1'});
static constexpr auto SGR_BG_GREEN      = sgr_sequence({'4','2'});
static constexpr auto SGR_BG_YELLOW     = sgr_sequence({'4','3'});
static constexpr auto SGR_BG_BLUE       = sgr_sequence({'4','4'});
static constexpr auto SGR_BG_MAGENTA    = sgr_sequence({'4','5'});
static constexpr auto SGR_BG_CYAN       = sgr_sequence({'4','6'});
static constexpr auto SGR_BG_DEFAULT    = sgr_sequence({'4','9'});





/* Common */

template <bool ResetColor, size_t N1, size_t N2>
static consteval auto _make_color (const char (&string_literal)[N1], const sgr_sequence<N2>& color)
{
    auto str = color + string_literal;

    if constexpr (ResetColor) {
        return str + SGR_RESET;
    }
    else {
        return str;
    }
}

template <bool ResetColor = true, size_t N>
consteval auto red (const char (&string_literal)[N])
{
    return _make_color<ResetColor>(string_literal, SGR_FG_RED);
}
