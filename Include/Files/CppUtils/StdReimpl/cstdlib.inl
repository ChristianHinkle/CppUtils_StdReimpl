// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <CppUtils/StdReimpl/cstdlib.h>

#include <type_traits>

namespace StdReimpl
{
    namespace Detail
    {
        template <StdReimpl::signed_integral TInt>
        constexpr TInt abs(TInt j);
    }

    constexpr int abs(int j)
    {
        return Detail::abs(j);
    }
    constexpr long int abs(long int j)
    {
        return Detail::abs(j);
    }
    constexpr long long int abs(long long int j)
    {
        return Detail::abs(j);
    }

#if 0 // Disabled until we have `std::signbit` reimplemented.
    template <StdReimpl::floating_point floating_point_type>
    constexpr floating_point_type abs(floating_point_type x)
    {
        if (std::is_constant_evaluated()) // if consteval
        {
            // A manual implementation, based on a simple branch.

            // Use the sign bit to determine negativity. To support -0 and -inf. Note: NaN propogation will naturally occur.
            if (std::signbit(x)) // TODO: Reimplement `signbit` for constexpr support.
            {
                return -x;
            }

            return x;
        }
        else
        {
            return std::abs(x);
        }
    }
#endif // #if 0

    namespace Detail
    {
        template <StdReimpl::signed_integral TInt>
        constexpr TInt abs(TInt j)
        {
            if (std::is_constant_evaluated()) // if consteval
            {
                // A manual implementation, based on a simple branch.

                if (j < 0)
                {
                    return -j;
                }

                return j;
            }
            else
            {
                return std::abs(j);
            }
        }
    }
}
