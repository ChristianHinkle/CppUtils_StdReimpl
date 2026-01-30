// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#ifndef CPPUTILS_STDREIMPL_EXPORT_H
#define CPPUTILS_STDREIMPL_EXPORT_H

// Include the appropriate "export header" file depending on which type of this library is being compiled.

#if __has_include(<cpputils_stdreimpl_shared_export.h>)
#   include <cpputils_stdreimpl_shared_export.h>
#elif __has_include(<cpputils_stdreimpl_module_export.h>)
#   include <cpputils_stdreimpl_module_export.h>
#elif __has_include(<cpputils_stdreimpl_object_export.h>)
#   include <cpputils_stdreimpl_object_export.h>
#elif __has_include(<cpputils_stdreimpl_static_export.h>)
#   include <cpputils_stdreimpl_static_export.h>
#else
    // Since no generated header files exist, it must mean we are an interface library (being used as a header-only library).
#   include <cpputils_stdreimpl_include_export.h>
#endif

// Undef the generated macros that we've decided not to use.

#ifdef CPPUTILS_STDREIMPL_DEPRECATED
#   undef CPPUTILS_STDREIMPL_DEPRECATED
#endif

#ifdef CPPUTILS_STDREIMPL_DEPRECATED_EXPORT
#   undef CPPUTILS_STDREIMPL_DEPRECATED_EXPORT
#endif

#ifdef CPPUTILS_STDREIMPL_DEPRECATED_NO_EXPORT
#   undef CPPUTILS_STDREIMPL_DEPRECATED_NO_EXPORT
#endif

#ifdef CPPUTILS_STDREIMPL_NO_DEPRECATED
#   undef CPPUTILS_STDREIMPL_NO_DEPRECATED
#endif

#endif // #ifndef CPPUTILS_STDREIMPL_EXPORT_H
