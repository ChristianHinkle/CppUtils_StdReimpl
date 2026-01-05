// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#include <CppUtils_StdReimpl.h>

// Perform the basic sanity checks first.
static_assert(CppUtils::StdReimpl::NonLvalueReference<int>);
static_assert(CppUtils::StdReimpl::NonLvalueReference<int&&>);
static_assert(!CppUtils::StdReimpl::NonLvalueReference<int&>);

namespace
{
    /**
     * @brief This example function only accepts rvalues.
     */
    template <CppUtils::StdReimpl::NonLvalueReference T>
    constexpr void MyNonLvalueReferenceFunc(T&& arg)
    {
    }

    /**
     * @brief Determines whether it's possible to pass in an lvalue to `MyNonLvalueReferenceFunc`.
     */
    constexpr bool CanPassInLvalueToMyNonLvalueReferenceFunc()
    {
        // Wrap the requires expression in a template function (template lambda).
        return []<class T = int>()
        {
            // Passing in an lvalue to `MyNonLvalueReferenceFunc` is ill-formed, so this will return false. Note: It's important to
            // write this ill-formed code based on the template argument, so the requires expression returns false instead of
            // actually giving us a compiler error.
            return requires(T myLvalue)
            {
                MyNonLvalueReferenceFunc(myLvalue);
            };
        }();
    }
}

static_assert(!CanPassInLvalueToMyNonLvalueReferenceFunc(), "It should not be possible to pass in an lvalue to `MyNonLvalueReferenceFunc`.");
