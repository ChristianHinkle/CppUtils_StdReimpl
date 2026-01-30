// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once


namespace StdReimpl
{
    // https://eel.is/c++draft/utilities#utility.syn-1

    template <auto V>
    struct constant_arg_t
    {
        explicit constant_arg_t() = default;
    };

    template <auto V>
    constexpr constant_arg_t<V> constant_arg{};
}
