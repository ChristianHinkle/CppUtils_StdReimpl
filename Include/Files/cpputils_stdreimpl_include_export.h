// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#ifndef CPPUTILS_STDREIMPL_INCLUDE_EXPORT_H
#define CPPUTILS_STDREIMPL_INCLUDE_EXPORT_H

// Since interface libraries cannot generate header files during their cmake configuration, we author our
// own "export header" file here, which defines the export macros as a noop.

#define CPPUTILS_STDREIMPL_EXPORT
#define CPPUTILS_STDREIMPL_NO_EXPORT

#endif // #ifndef CPPUTILS_STDREIMPL_INCLUDE_EXPORT_H
