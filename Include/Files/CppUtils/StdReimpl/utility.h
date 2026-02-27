// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <type_traits>

namespace StdReimpl
{
    /**
     * @see https://eel.is/c++draft/utilities#utility.underlying
     * @see https://cppreference.com/w/cpp/utility/to_underlying.html
     * @note A feature from the C++23 standard.
     */
    template <class T>
    constexpr std::underlying_type_t<T> to_underlying(T value) noexcept
    {
        return static_cast<std::underlying_type_t<T>>(value);
    }

    /**
     * @see https://cppreference.com/w/cpp/utility/nontype.html
     * @see https://eel.is/c++draft/utilities
     * @note A feature from the C++26 standard.
     */
    template <auto V>
    struct constant_arg_t
    {
        explicit constant_arg_t() = default;
    };

    /**
     * @see https://cppreference.com/w/cpp/utility/nontype.html
     * @see https://eel.is/c++draft/utilities
     * @note A feature from the C++26 standard.
     */
    template <auto V>
    constexpr constant_arg_t<V> constant_arg{};
}

#include <CppUtils/StdReimpl/utility.inl>
