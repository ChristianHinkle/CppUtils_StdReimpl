// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <functional>
#include <type_traits>
#include <CppUtils/StdReimpl/Utility.h>

// Currently disabled entirely, as it is not at all usable rn.
#if 0

namespace StdReimpl
{
    // https://eel.is/c++draft/func.wrap.ref

    // Implementation of `std::function_ref`. TODO: Actually implement it.
    // - All of the member functions are currently declarations only.

    // Primary template.
    template <class>
    class function_ref;

    template <class R, class... ArgTypes>
    class function_ref<R(ArgTypes...) const noexcept(true)>
    {
    private:
        using BoundEntityType = void*;

    public:
        // [func.wrap.ref.ctor], constructors and assignment operators

        template <class F>
        function_ref(F*) noexcept;

        template <class F>
        constexpr function_ref(F&&) noexcept;

        template <auto f>
        constexpr function_ref(constant_arg_t<f>) noexcept;

        template <auto f, class U>
        constexpr function_ref(constant_arg_t<f>, U&&) noexcept;

        template <auto f, class T>
        constexpr function_ref(constant_arg_t<f>, const T*) noexcept;

        constexpr function_ref(const function_ref&) noexcept = default;

        constexpr function_ref& operator=(const function_ref&) noexcept = default;

        template <class T>
        function_ref& operator=(T) = delete;

        // [func.wrap.ref.inv], invocation
        R operator()(ArgTypes...) const noexcept(true);

    private:
        template <class... T>
        static constexpr bool is_invocable_using = std::is_nothrow_invocable_r_v<R, T..., ArgTypes...>;

        R (*thunk_ptr)(BoundEntityType, ArgTypes&&...) noexcept(true) = nullptr;
        BoundEntityType bound_entity = nullptr;
    };

    template <class R, class... ArgTypes>
    class function_ref<R(ArgTypes...) const noexcept(false)>
    {
    private:
        using BoundEntityType = void*;

    public:
        // [func.wrap.ref.ctor], constructors and assignment operators

        template <class F>
        function_ref(F*) noexcept;

        template <class F>
        constexpr function_ref(F&&) noexcept;

        template <auto f>
        constexpr function_ref(constant_arg_t<f>) noexcept;

        template <auto f, class U>
        constexpr function_ref(constant_arg_t<f>, U&&) noexcept;

        template <auto f, class T>
        constexpr function_ref(constant_arg_t<f>, const T*) noexcept;

        constexpr function_ref(const function_ref&) noexcept = default;

        constexpr function_ref& operator=(const function_ref&) noexcept = default;

        template <class T>
        function_ref& operator=(T) = delete;

        // [func.wrap.ref.inv], invocation
        R operator()(ArgTypes...) const noexcept(false);

    private:
        template <class... T>
        static constexpr bool is_invocable_using = std::is_invocable_r_v<R, T..., ArgTypes...>;

        R (*thunk_ptr)(BoundEntityType, ArgTypes&&...) noexcept(false) = nullptr;
        BoundEntityType bound_entity = nullptr;
    };

    template <class R, class... ArgTypes>
    class function_ref<R(ArgTypes...) noexcept(true)>
    {
    private:
        using BoundEntityType = void*;

    public:
        // [func.wrap.ref.ctor], constructors and assignment operators

        template <class F>
        function_ref(F*) noexcept;

        template <class F>
        constexpr function_ref(F&&) noexcept;

        template <auto f>
        constexpr function_ref(constant_arg_t<f>) noexcept;

        template <auto f, class U>
        constexpr function_ref(constant_arg_t<f>, U&&) noexcept;

        template <auto f, class T>
        constexpr function_ref(constant_arg_t<f>, T*) noexcept;

        constexpr function_ref(const function_ref&) noexcept = default;

        constexpr function_ref& operator=(const function_ref&) noexcept = default;

        template <class T>
        function_ref& operator=(T) = delete;

        // [func.wrap.ref.inv], invocation
        R operator()(ArgTypes...) const noexcept(true);

    private:
        template <class... T>
        static constexpr bool is_invocable_using = std::is_nothrow_invocable_r_v<R, T..., ArgTypes...>;

        R (*thunk_ptr)(BoundEntityType, ArgTypes&&...) noexcept(true) = nullptr;
        BoundEntityType bound_entity = nullptr;
    };

    template <class R, class... ArgTypes>
    class function_ref<R(ArgTypes...) noexcept(false)>
    {
    private:
        using BoundEntityType = void*;

    public:
        // [func.wrap.ref.ctor], constructors and assignment operators

        template <class F>
        function_ref(F*) noexcept;

        template <class F>
        constexpr function_ref(F&&) noexcept;

        template <auto f>
        constexpr function_ref(constant_arg_t<f>) noexcept;

        template <auto f, class U>
        constexpr function_ref(constant_arg_t<f>, U&&) noexcept;

        template <auto f, class T>
        constexpr function_ref(constant_arg_t<f>, T*) noexcept;

        constexpr function_ref(const function_ref&) noexcept = default;

        constexpr function_ref& operator=(const function_ref&) noexcept = default;

        template <class T>
        function_ref& operator=(T) = delete;

        // [func.wrap.ref.inv], invocation
        R operator()(ArgTypes...) const noexcept(false);

    private:
        template <class... T>
        static constexpr bool is_invocable_using = std::is_invocable_r_v<R, T..., ArgTypes...>;

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
}

#endif // #if 0

#include <CppUtils/StdReimpl/Functional.inl>
