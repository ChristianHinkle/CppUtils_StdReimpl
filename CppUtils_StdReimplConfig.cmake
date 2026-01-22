# Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

# Write out the main project name here, in one place, to make renaming easier. This is mainly used by our
# boilerplate code. There may be other mentions of the project's name mixed in certain places of this file, so
# don't rely on these variables alone for renaming.
SET(MY_BASE_PROJECT_NAME_FULL "CppUtils_StdReimpl")
string(TOLOWER ${MY_BASE_PROJECT_NAME_FULL} MY_BASE_PROJECT_NAME_FULL_LOWERCASE)
string(TOUPPER ${MY_BASE_PROJECT_NAME_FULL} MY_BASE_PROJECT_NAME_FULL_UPPERCASE)
SET(MY_BASE_PROJECT_NAME_NAMESPACE "CppUtils")
SET(MY_BASE_PROJECT_NAME_LEAFNAME "StdReimpl")

include(CMakeFindDependencyMacro)

# Include our export. This imports all of our targets.
include("${CMAKE_CURRENT_LIST_DIR}/${MY_BASE_PROJECT_NAME_FULL}Export.cmake")

#
# Add alias targets whose names match those from the project file.
#
# It's important to make sure the names are the same for consistency. Namely, so that build
# interface users can use `OVERRIDE_FIND_PACKAGE` with `FetchContent_Declare()`.
#

add_library(${MY_BASE_PROJECT_NAME_NAMESPACE}::${MY_BASE_PROJECT_NAME_LEAFNAME}::Include ALIAS ${MY_BASE_PROJECT_NAME_FULL}_Include)
add_library(${MY_BASE_PROJECT_NAME_NAMESPACE}::${MY_BASE_PROJECT_NAME_LEAFNAME}::Source ALIAS ${MY_BASE_PROJECT_NAME_FULL}_Source)
add_library(${MY_BASE_PROJECT_NAME_NAMESPACE}::${MY_BASE_PROJECT_NAME_LEAFNAME}::Static ALIAS ${MY_BASE_PROJECT_NAME_FULL}_Static)
add_library(${MY_BASE_PROJECT_NAME_NAMESPACE}::${MY_BASE_PROJECT_NAME_LEAFNAME}::Shared ALIAS ${MY_BASE_PROJECT_NAME_FULL}_Shared)
add_library(${MY_BASE_PROJECT_NAME_NAMESPACE}::${MY_BASE_PROJECT_NAME_LEAFNAME}::Module ALIAS ${MY_BASE_PROJECT_NAME_FULL}_Module)
add_library(${MY_BASE_PROJECT_NAME_NAMESPACE}::${MY_BASE_PROJECT_NAME_LEAFNAME}::Object ALIAS ${MY_BASE_PROJECT_NAME_FULL}_Object)
