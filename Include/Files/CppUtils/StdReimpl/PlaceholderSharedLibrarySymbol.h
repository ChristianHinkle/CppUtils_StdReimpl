// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <CppUtils_StdReimpl_Export.h>

namespace StdReimpl
{
    /**
     * @brief A dummy symbol to export when building the shared library, because some compilers only generate an output artifact for
     *        the shared import library if there is at least one symbol to export/import.
     */
    CPPUTILS_STDREIMPL_EXPORT void PlaceholderSharedLibrarySymbol();
}
