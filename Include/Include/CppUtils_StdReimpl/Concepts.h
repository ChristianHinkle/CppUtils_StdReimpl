// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <type_traits>

/**
 * @brief Rewrites of the standard library's concepts library. This is necessary for certain
 *        compilers that may not have the C++20 standard concepts library enabled, e.g., for certain
 *        gaming consoles.
 */
namespace StdReimpl
{
    /**
     * @see https://eel.is/c++draft/concept.derived#concept:derived_from
     */
    template <class Derived, class Base>
    concept derived_from =
        std::is_base_of_v<Base, Derived> &&
        std::is_convertible_v<const volatile Derived*, const volatile Base*>;

    /**
     * @see https://eel.is/c++draft/concepts.arithmetic#concept:floating_point
     */
    template <class T>
    concept floating_point = std::is_floating_point_v<T>;

    /**
     * @see https://eel.is/c++draft/concept.invocable#concept:invocable
     */
    template <class F, class... Args>
    concept invocable = requires(F&& f, Args&&... args) {
        std::invoke(std::forward<F>(f), std::forward<Args>(args)...); // not required to be equality-preserving
    };

    /**
     * @see https://eel.is/c++draft/concept.regularinvocable#concept:regular_invocable
     */
    template <class F, class... Args>
    concept regular_invocable = StdReimpl::invocable<F, Args...>;
}
