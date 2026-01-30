// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <functional>
#include <type_traits>
#include <CppUtils/StdReimpl/Utility.h>
#include <memory>

namespace StdReimpl
{
    /**
     * @brief A feature from C++23.
     * @see https://eel.is/c++draft/func.invoke
     * @see https://en.cppreference.com/w/cpp/utility/functional/invoke.html
     */
    template <class R, class F, class... Args>
        requires (std::is_invocable_r_v<R, F, Args...>)
    constexpr R invoke_r(F&& f, Args&&... args)
        noexcept(std::is_nothrow_invocable_r_v<R, F, Args...>)
    {
        if constexpr (std::is_void_v<R>)
        {
            std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
        }
        else
        {
            return std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
        }
    }

    // Implementation of C++26 feature `std::function_ref`. TODO: Implement it properly and fix all the errors we get when we try using it. So far all that's implemented is what I read from the Working Draft. I wouldn't doubt there are mistakes in the starting point code I wrote below. I was especially unconfident with how I decided to use the "invoke" functions in the thunk lambdas.
#if 0
    // https://eel.is/c++draft/func.wrap.ref

    // Primary template.
    template <class>
    class function_ref;

    template <class R, class... ArgTypes>
    class function_ref<R(ArgTypes...) const noexcept(true)>
    {
    private:
        using BoundEntityType = void*;

        template <class... T>
        static constexpr bool is_invocable_using = std::is_nothrow_invocable_r_v<R, T..., ArgTypes...>;

    public:
        // [func.wrap.ref.ctor], constructors and assignment operators

        template <class F>
            requires (std::is_function_v<F> && is_invocable_using<F>)
        function_ref(F* f) noexcept
            : bound_entity{f}
            , thunk_ptr{
                [](BoundEntityType bound_entity_param, ArgTypes&& call_args)
                {
                    StdReimpl::invoke_r<R>(*bound_entity_param, call_args...);
                }
            }
        {
            // Preconditions: f is not a null pointer.
            assert(f);
        }

        template <class F>
            requires (
                !std::is_same_v<std::remove_cvref_t<F>, function_ref>
                && !is_member_pointer_v<remove_reference_t<F>>
                && is_invocable_using<std::add_lvalue_reference_t<const remove_reference_t<F>>>
            )
        constexpr function_ref(F&& f) noexcept
            : bound_entity{std::addressof(f)}
            , thunk_ptr{
                [](BoundEntityType bound_entity_param, ArgTypes&& call_args)
                {
                    // Let T be remove_reference_t<F>.
                    using T = std::remove_reference_t<F>;

                    StdReimpl::invoke_r<R>(static_cast<const T&>(*bound_entity_param), call_args...);
                }
            }
        {
        }

        template <auto f>
            requires (is_invocable_using<const decltype(f)&>)
        constexpr function_ref(constant_arg_t<f>) noexcept
            : bound_entity{nullptr}
            , thunk_ptr{
                [](BoundEntityType bound_entity_param, ArgTypes&& call_args)
                {
                    StdReimpl::invoke_r<R>(*bound_entity_param, call_args...);
                }
            }
        {
            // Let F be decltype(f).
            using F = decltype(f);

            // Mandates: If is_pointer_v<F> || is_member_pointer_v<F> is true, then f != nullptr is true.
            static_assert(!(std::is_pointer_v<F> || std::is_member_pointer_v<F>) || f != nullptr);
        }

        template <auto f, class U>
            requires (
                !std::is_rvalue_reference_v<U&&>
                && is_invocable_using<const decltype(f)&, std::add_lvalue_reference_t<const std::remove_reference_t<U>>>
            )
        constexpr function_ref(constant_arg_t<f>, U&& obj) noexcept
            : bound_entity{std::addressof(obj)}
            , thunk_ptr{
                [](BoundEntityType bound_entity_param, ArgTypes&& call_args)
                {
                    // Let T be remove_reference_t<U> and F be decltype(f).
                    using T = std::remove_reference_t<U>;

                    StdReimpl::invoke_r<R>(*bound_entity_param, static_cast<const T&>(*bound_entity_param), call_args...);
                }
            }
        {
            // Let T be remove_reference_t<U> and F be decltype(f).
            using F = decltype(f);

            // Mandates: If is_pointer_v<F> || is_member_pointer_v<F> is true, then f != nullptr is true.
            static_assert(!(std::is_pointer_v<F> || std::is_member_pointer_v<F>) || f != nullptr);
        }

        template <auto f, class T>
            requires (is_invocable_using<const decltype(f)&, const T*>)
        constexpr function_ref(constant_arg_t<f>, const T* obj) noexcept
            : bound_entity{obj}
            , thunk_ptr{
                [](BoundEntityType bound_entity_param, ArgTypes&& call_args)
                {
                    StdReimpl::invoke_r<R>(*bound_entity_param, bound_entity_param, call_args...);
                }
            }
        {
            // Let F be decltype(f).
            using F = decltype(f);

            // Mandates: If is_pointer_v<F> || is_member_pointer_v<F> is true, then f != nullptr is true.
            static_assert(!(std::is_pointer_v<F> || std::is_member_pointer_v<F>) || f != nullptr);

            // Preconditions: If is_member_pointer_v<F> is true, obj is not a null pointer.
            assert(!std::is_member_pointer_v<F> || obj);
        }

        constexpr function_ref(const function_ref&) noexcept = default;

        constexpr function_ref& operator=(const function_ref&) noexcept = default;

        template <class T>
            requires (!std::is_same_v<function_ref> && !std::is_pointer_v<T>) // TODO: Implement constraint: "T is not a specialization of constant_arg_t."
        function_ref& operator=(T) = delete;

        // [func.wrap.ref.inv], invocation
        R operator()(ArgTypes... args) const noexcept(true)
        {
            return thunk_ptr(bound_entity, std::forward<ArgTypes>(args)...);
        }

    private:
        R (*thunk_ptr)(BoundEntityType, ArgTypes&&...) noexcept(true) = nullptr;
        BoundEntityType bound_entity = nullptr;
    };

    template <class R, class... ArgTypes>
    class function_ref<R(ArgTypes...) const noexcept(false)>
    {
    private:
        using BoundEntityType = void*;

        template <class... T>
        static constexpr bool is_invocable_using = std::is_invocable_r_v<R, T..., ArgTypes...>;

    public:
        // [func.wrap.ref.ctor], constructors and assignment operators

        template <class F>
            requires (std::is_function_v<F> && is_invocable_using<F>)
        function_ref(F* f) noexcept
            : bound_entity{f}
            , thunk_ptr{
                [](BoundEntityType bound_entity_param, ArgTypes&& call_args)
                {
                    StdReimpl::invoke_r<R>(*bound_entity_param, call_args...);
                }
            }
        {
            // Preconditions: f is not a null pointer.
            assert(f);
        }

        template <class F>
            requires (
                !std::is_same_v<std::remove_cvref_t<F>, function_ref>
                && !is_member_pointer_v<remove_reference_t<F>>
                && is_invocable_using<std::add_lvalue_reference_t<const remove_reference_t<F>>>
            )
        constexpr function_ref(F&& f) noexcept
            : bound_entity{std::addressof(f)}
            , thunk_ptr{
                [](BoundEntityType bound_entity_param, ArgTypes&& call_args)
                {
                    // Let T be remove_reference_t<F>.
                    using T = std::remove_reference_t<F>;

                    StdReimpl::invoke_r<R>(static_cast<const T&>(*bound_entity_param), call_args...);
                }
            }
        {
        }

        template <auto f>
            requires (is_invocable_using<const decltype(f)&>)
        constexpr function_ref(constant_arg_t<f>) noexcept
            : bound_entity{nullptr}
            , thunk_ptr{
                [](BoundEntityType bound_entity_param, ArgTypes&& call_args)
                {
                    StdReimpl::invoke_r<R>(*bound_entity_param, call_args...);
                }
            }
        {
            // Let F be decltype(f).
            using F = decltype(f);

            // Mandates: If is_pointer_v<F> || is_member_pointer_v<F> is true, then f != nullptr is true.
            static_assert(!(std::is_pointer_v<F> || std::is_member_pointer_v<F>) || f != nullptr);
        }

        template <auto f, class U>
            requires (
                !std::is_rvalue_reference_v<U&&>
                && is_invocable_using<const decltype(f)&, std::add_lvalue_reference_t<const std::remove_reference_t<U>>>
            )
        constexpr function_ref(constant_arg_t<f>, U&& obj) noexcept
            : bound_entity{std::addressof(obj)}
            , thunk_ptr{
                [](BoundEntityType bound_entity_param, ArgTypes&& call_args)
                {
                    // Let T be remove_reference_t<U> and F be decltype(f).
                    using T = std::remove_reference_t<U>;

                    StdReimpl::invoke_r<R>(*bound_entity_param, static_cast<const T&>(*bound_entity_param), call_args...);
                }
            }
        {
            // Let T be remove_reference_t<U> and F be decltype(f).
            using F = decltype(f);

            // Mandates: If is_pointer_v<F> || is_member_pointer_v<F> is true, then f != nullptr is true.
            static_assert(!(std::is_pointer_v<F> || std::is_member_pointer_v<F>) || f != nullptr);
        }

        template <auto f, class T>
            requires (is_invocable_using<const decltype(f)&, const T*>)
        constexpr function_ref(constant_arg_t<f>, const T* obj) noexcept
            : bound_entity{obj}
            , thunk_ptr{
                [](BoundEntityType bound_entity_param, ArgTypes&& call_args)
                {
                    StdReimpl::invoke_r<R>(*bound_entity_param, bound_entity_param, call_args...);
                }
            }
        {
            // Let F be decltype(f).
            using F = decltype(f);

            // Mandates: If is_pointer_v<F> || is_member_pointer_v<F> is true, then f != nullptr is true.
            static_assert(!(std::is_pointer_v<F> || std::is_member_pointer_v<F>) || f != nullptr);

            // Preconditions: If is_member_pointer_v<F> is true, obj is not a null pointer.
            assert(!std::is_member_pointer_v<F> || obj);
        }

        constexpr function_ref(const function_ref&) noexcept = default;

        constexpr function_ref& operator=(const function_ref&) noexcept = default;

        template <class T>
            requires (!std::is_same_v<function_ref> && !std::is_pointer_v<T>) // TODO: Implement constraint: "T is not a specialization of constant_arg_t."
        function_ref& operator=(T) = delete;

        // [func.wrap.ref.inv], invocation
        R operator()(ArgTypes... args) const noexcept(false)
        {
            return thunk_ptr(bound_entity, std::forward<ArgTypes>(args)...);
        }

    private:
        R (*thunk_ptr)(BoundEntityType, ArgTypes&&...) noexcept(false) = nullptr;
        BoundEntityType bound_entity = nullptr;
    };

    template <class R, class... ArgTypes>
    class function_ref<R(ArgTypes...) noexcept(true)>
    {
    private:
        using BoundEntityType = void*;

        template <class... T>
        static constexpr bool is_invocable_using = std::is_nothrow_invocable_r_v<R, T..., ArgTypes...>;

    public:
        // [func.wrap.ref.ctor], constructors and assignment operators

        template <class F>
            requires (std::is_function_v<F> && is_invocable_using<F>)
        function_ref(F* f) noexcept
            : bound_entity{f}
            , thunk_ptr{
                [](BoundEntityType bound_entity_param, ArgTypes&& call_args)
                {
                    StdReimpl::invoke_r<R>(*bound_entity_param, call_args...);
                }
            }
        {
            // Preconditions: f is not a null pointer.
            assert(f);
        }

        template <class F>
            requires (
                !std::is_same_v<std::remove_cvref_t<F>, function_ref>
                && !is_member_pointer_v<remove_reference_t<F>>
                && is_invocable_using<std::add_lvalue_reference_t<remove_reference_t<F>>>
            )
        constexpr function_ref(F&& f) noexcept
            : bound_entity{std::addressof(f)}
            , thunk_ptr{
                [](BoundEntityType bound_entity_param, ArgTypes&& call_args)
                {
                    // Let T be remove_reference_t<F>.
                    using T = std::remove_reference_t<F>;

                    StdReimpl::invoke_r<R>(static_cast<T&>(*bound_entity_param), call_args...);
                }
            }
        {
        }

        template <auto f>
            requires (is_invocable_using<const decltype(f)&>)
        constexpr function_ref(constant_arg_t<f>) noexcept
            : bound_entity{nullptr}
            , thunk_ptr{
                [](BoundEntityType bound_entity_param, ArgTypes&& call_args)
                {
                    StdReimpl::invoke_r<R>(*bound_entity_param, call_args...);
                }
            }
        {
            // Let F be decltype(f).
            using F = decltype(f);

            // Mandates: If is_pointer_v<F> || is_member_pointer_v<F> is true, then f != nullptr is true.
            static_assert(!(std::is_pointer_v<F> || std::is_member_pointer_v<F>) || f != nullptr);
        }

        template <auto f, class U>
            requires (
                !std::is_rvalue_reference_v<U&&>
                && is_invocable_using<const decltype(f)&, std::add_lvalue_reference_t<std::remove_reference_t<U>>>
            )
        constexpr function_ref(constant_arg_t<f>, U&& obj) noexcept
            : bound_entity{std::addressof(obj)}
            , thunk_ptr{
                [](BoundEntityType bound_entity_param, ArgTypes&& call_args)
                {
                    // Let T be remove_reference_t<U> and F be decltype(f).
                    using T = std::remove_reference_t<U>;

                    StdReimpl::invoke_r<R>(*bound_entity_param, static_cast<T&>(*bound_entity_param), call_args...);
                }
            }
        {
            // Let T be remove_reference_t<U> and F be decltype(f).
            using F = decltype(f);

            // Mandates: If is_pointer_v<F> || is_member_pointer_v<F> is true, then f != nullptr is true.
            static_assert(!(std::is_pointer_v<F> || std::is_member_pointer_v<F>) || f != nullptr);
        }

        template <auto f, class T>
            requires (is_invocable_using<const decltype(f)&, T*>)
        constexpr function_ref(constant_arg_t<f>, T* obj) noexcept
            : bound_entity{obj}
            , thunk_ptr{
                [](BoundEntityType bound_entity_param, ArgTypes&& call_args)
                {
                    StdReimpl::invoke_r<R>(*bound_entity_param, bound_entity_param, call_args...);
                }
            }
        {
            // Let F be decltype(f).
            using F = decltype(f);

            // Mandates: If is_pointer_v<F> || is_member_pointer_v<F> is true, then f != nullptr is true.
            static_assert(!(std::is_pointer_v<F> || std::is_member_pointer_v<F>) || f != nullptr);

            // Preconditions: If is_member_pointer_v<F> is true, obj is not a null pointer.
            assert(!std::is_member_pointer_v<F> || obj);
        }

        constexpr function_ref(const function_ref&) noexcept = default;

        constexpr function_ref& operator=(const function_ref&) noexcept = default;

        template <class T>
            requires (!std::is_same_v<function_ref> && !std::is_pointer_v<T>) // TODO: Implement constraint: "T is not a specialization of constant_arg_t."
        function_ref& operator=(T) = delete;

        // [func.wrap.ref.inv], invocation
        R operator()(ArgTypes... args) const noexcept(true)
        {
            return thunk_ptr(bound_entity, std::forward<ArgTypes>(args)...);
        }

    private:
        R (*thunk_ptr)(BoundEntityType, ArgTypes&&...) noexcept(true) = nullptr;
        BoundEntityType bound_entity = nullptr;
    };

    template <class R, class... ArgTypes>
    class function_ref<R(ArgTypes...) noexcept(false)>
    {
    private:
        using BoundEntityType = void*;

        template <class... T>
        static constexpr bool is_invocable_using = std::is_invocable_r_v<R, T..., ArgTypes...>;

    public:
        // [func.wrap.ref.ctor], constructors and assignment operators

        template <class F>
            requires (std::is_function_v<F> && is_invocable_using<F>)
        function_ref(F* f) noexcept
            : bound_entity{f}
            , thunk_ptr{
                [](BoundEntityType bound_entity_param, ArgTypes&& call_args)
                {
                    StdReimpl::invoke_r<R>(*bound_entity_param, call_args...);
                }
            }
        {
            // Preconditions: f is not a null pointer.
            assert(f);
        }

        template <class F>
            requires (
                !std::is_same_v<std::remove_cvref_t<F>, function_ref>
                && !is_member_pointer_v<remove_reference_t<F>>
                && is_invocable_using<std::add_lvalue_reference_t<remove_reference_t<F>>>
            )
        constexpr function_ref(F&& f) noexcept
            : bound_entity{std::addressof(f)}
            , thunk_ptr{
                [](BoundEntityType bound_entity_param, ArgTypes&& call_args)
                {
                    // Let T be remove_reference_t<F>.
                    using T = std::remove_reference_t<F>;

                    StdReimpl::invoke_r<R>(static_cast<T&>(*bound_entity_param), call_args...);
                }
            }
        {
        }

        template <auto f>
            requires (is_invocable_using<const decltype(f)&>)
        constexpr function_ref(constant_arg_t<f>) noexcept
            : bound_entity{nullptr}
            , thunk_ptr{
                [](BoundEntityType bound_entity_param, ArgTypes&& call_args)
                {
                    StdReimpl::invoke_r<R>(*bound_entity_param, call_args...);
                }
            }
        {
            // Let F be decltype(f).
            using F = decltype(f);

            // Mandates: If is_pointer_v<F> || is_member_pointer_v<F> is true, then f != nullptr is true.
            static_assert(!(std::is_pointer_v<F> || std::is_member_pointer_v<F>) || f != nullptr);
        }

        template <auto f, class U>
            requires (
                !std::is_rvalue_reference_v<U&&>
                && is_invocable_using<const decltype(f)&, std::add_lvalue_reference_t<std::remove_reference_t<U>>>
            )
        constexpr function_ref(constant_arg_t<f>, U&& obj) noexcept
            : bound_entity{std::addressof(obj)}
            , thunk_ptr{
                [](BoundEntityType bound_entity_param, ArgTypes&& call_args)
                {
                    // Let T be remove_reference_t<U> and F be decltype(f).
                    using T = std::remove_reference_t<U>;

                    StdReimpl::invoke_r<R>(*bound_entity_param, static_cast<T&>(*bound_entity_param), call_args...);
                }
            }
        {
            // Let T be remove_reference_t<U> and F be decltype(f).
            using F = decltype(f);

            // Mandates: If is_pointer_v<F> || is_member_pointer_v<F> is true, then f != nullptr is true.
            static_assert(!(std::is_pointer_v<F> || std::is_member_pointer_v<F>) || f != nullptr);
        }

        template <auto f, class T>
            requires (is_invocable_using<const decltype(f)&, T*>)
        constexpr function_ref(constant_arg_t<f>, T* obj) noexcept
            : bound_entity{obj}
            , thunk_ptr{
                [](BoundEntityType bound_entity_param, ArgTypes&& call_args)
                {
                    StdReimpl::invoke_r<R>(*bound_entity_param, bound_entity_param, call_args...);
                }
            }
        {
            // Let F be decltype(f).
            using F = decltype(f);

            // Mandates: If is_pointer_v<F> || is_member_pointer_v<F> is true, then f != nullptr is true.
            static_assert(!(std::is_pointer_v<F> || std::is_member_pointer_v<F>) || f != nullptr);

            // Preconditions: If is_member_pointer_v<F> is true, obj is not a null pointer.
            assert(!std::is_member_pointer_v<F> || obj);
        }

        constexpr function_ref(const function_ref&) noexcept = default;

        constexpr function_ref& operator=(const function_ref&) noexcept = default;

        template <class T>
            requires (!std::is_same_v<function_ref> && !std::is_pointer_v<T>) // TODO: Implement constraint: "T is not a specialization of constant_arg_t."
        function_ref& operator=(T) = delete;

        // [func.wrap.ref.inv], invocation
        R operator()(ArgTypes... args) const noexcept(false)
        {
            return thunk_ptr(bound_entity, std::forward<ArgTypes>(args)...);
        }

    private:
        R (*thunk_ptr)(BoundEntityType, ArgTypes&&...) noexcept(false) = nullptr;
        BoundEntityType bound_entity = nullptr;
    };

    // [func.wrap.ref.deduct], deduction guides

    // TODO: [todo] Implement https://eel.is/c++draft/func.wrap.ref.deduct.
    // - Need to add template parameter constraints.
    // - Need to implement the last deduction guide `function_ref(StdReimpl::constant_arg_t<f>, T&&)` which is a little complicated.

    template <class F>
    function_ref(F*) -> function_ref<F>;

    template <auto f>
    function_ref(StdReimpl::constant_arg_t<f>) -> function_ref<std::remove_pointer_t<decltype(f)>>;

#if 0
    template <auto f, class T>
    function_ref(StdReimpl::constant_arg_t<f>, T&&) -> function_ref<R(A...) noexcept(E)>;
#endif // #if 0
#endif // #if 0
}

#include <CppUtils/StdReimpl/Functional.inl>
