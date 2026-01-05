// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <type_traits>

/**
 * @brief Rewrites of the standard library's concepts library. This is necessary for certain
 *        compilers that don't have C++20 standard concepts library enabled, e.g., for PS5.
 */
namespace CppUtils::StdReimpl::Std
{
    template <class TDerived, class TBase>
    concept derived_from =
        std::is_base_of_v<TBase, TDerived>
        && std::is_convertible_v<const volatile TDerived*, const volatile TBase*>;

    template <class T>
    concept floating_point =
        std::is_floating_point_v<T>;
}
