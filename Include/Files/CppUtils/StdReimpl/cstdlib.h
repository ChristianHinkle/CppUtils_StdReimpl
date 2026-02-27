// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <CppUtils_StdReimpl_Export.h>
#include <CppUtils/StdReimpl/concepts.h>

namespace StdReimpl
{
    /**
     * @see https://eel.is/c++draft/c.math.abs
     * @see https://cppreference.com/w/cpp/numeric/math/abs
     * @note Constexpr support is a feature from the C++23 standard.
     */
    constexpr int abs(int j);
    constexpr long int abs(long int j);
    constexpr long long int abs(long long int j);

#if 0 // Disabled until we have `std::signbit` reimplemented.
    /**
     * @see https://eel.is/c++draft/c.math.abs
     * @see https://cppreference.com/w/cpp/numeric/math/fabs
     * @note Templated and constexpr support is a feature from the C++23 standard.
     */
    template <StdReimpl::floating_point floating_point_type>
    constexpr floating_point_type abs(floating_point_type x);
#endif // #if 0
}

#include <CppUtils/StdReimpl/cstdlib.inl>
