# Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

cmake_minimum_required(VERSION 4.0)

# Write out the main project name here, in one place, to make renaming easier. This is mainly used by our
# boilerplate code. There may be other mentions of the project's name mixed in certain places of this file, so
# don't rely on these variables alone for renaming.
SET(MY_BASE_PROJECT_NAME_FULL "CppUtils_StdReimpl")
string(TOLOWER ${MY_BASE_PROJECT_NAME_FULL} MY_BASE_PROJECT_NAME_FULL_LOWERCASE)
string(TOUPPER ${MY_BASE_PROJECT_NAME_FULL} MY_BASE_PROJECT_NAME_FULL_UPPERCASE)
SET(MY_BASE_PROJECT_NAME_NAMESPACE "CppUtils")
SET(MY_BASE_PROJECT_NAME_LEAFNAME "StdReimpl")

project(${MY_BASE_PROJECT_NAME_FULL}_Include
  VERSION 0.1.0
  LANGUAGES CXX
  )

include(GNUInstallDirs)

#
# Create targets.
#
# We namespace target names using underscore characters "_". Namespacing helps avoid ambiguity. E.g., for
# the DAM library of MeddySDK, we name it "MeddySDK_DAM" instead of just "DAM", because that would be very vague.
#

add_library(${MY_BASE_PROJECT_NAME_FULL}_Include INTERFACE)

#
# Set output names of our targets.
#
# This determines the name of the binary files generated for the target. The name may differ from the actual
# target name. E.g., for MeddySDKCLI, we name its executable file "meddy" to suit our desired command line usage.
#

set_target_properties(${MY_BASE_PROJECT_NAME_FULL}_Include
  PROPERTIES
    OUTPUT_NAME "${MY_BASE_PROJECT_NAME_FULL}"
  )

#
# Add alias targets.
#
# We name these with special characters that filenames can't have. We use these aliases instead of the direct
# target names where possible so that they can't be be interpreted as filenames.
#
# It's conventional to use "::" in these names, like namespaces. If a target doesn't have anything to namespace in its
# name then just prepend "::" to it, analogous to how we indicate global scope in cpp.
#
# The aliases set up here get used by build interface users. Make sure the same aliases are also set up for install
# interface users somewhere else. We do that in the export's config file. It's important to make sure the names are
# the same in both cases for consistency. Namely, so that build interface users can use `OVERRIDE_FIND_PACKAGE` with
# `FetchContent_Declare()`.
#

add_library(${MY_BASE_PROJECT_NAME_NAMESPACE}::${MY_BASE_PROJECT_NAME_LEAFNAME}::Include ALIAS ${MY_BASE_PROJECT_NAME_FULL}_Include)

# List out required language features.
target_compile_features(${MY_BASE_PROJECT_NAME_FULL}_Include
  INTERFACE
    cxx_std_20
  )

#
# Add all header files and set up their include directories.
#
# The `BASE_DIRS` of file sets automatically get added to the target's include
# directories. So we don't use the `target_include_directories()` command anymore
# because that would be redundant.
#
# Interface and/or public file sets are important because we use them to install headers for
# the install interface users.
#
# A nice benefit of listing all header files is that they can be easily-discoverable by IDEs.
#
# We make sure to group all of our header files into a subfolder named after our project. This is
# important for avoiding include file path naming conflicts with other libraries. It also gives
# us the freedom to name our files however, while still having context in their include path names.
#
target_sources(${MY_BASE_PROJECT_NAME_FULL}_Include
  INTERFACE
    FILE_SET ${MY_BASE_PROJECT_NAME_FULL_LOWERCASE}_include_interface_headers
      TYPE HEADERS
      BASE_DIRS
        "${CMAKE_CURRENT_SOURCE_DIR}/Include"
      FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/Include/${MY_BASE_PROJECT_NAME_FULL}/Concepts.h"
  )

# Note that we do not "find package" for our parent project. We don't need to since we are built in the same
# cmake invocation as the it. That means we're being processed during the same configuration step as them, which
# means we'll have all their targets. Also, the targets that we reference in `target_link_libraries` commands don't
# even need to exist yet because CMake only validates the existence of targets at the end of the entire
# configuration process.

# Associate our targets with an export and specify how their files would be installed.
install(TARGETS ${MY_BASE_PROJECT_NAME_FULL}_Include
  EXPORT "${MY_BASE_PROJECT_NAME_FULL}Export"
  COMPONENT "Include"
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}"
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}"
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME}"
  #
  # Add to interface include directories.
  #
  # Note: We take care of install-specific include directories here instead of `target_include_directories()`.
  #
  # Note: It's important to make sure cpp include statements of public headers are compatible between
  # install interface users and build interface users. E.g., a user's include statements should work for
  # both packaged library usage and subproject library usage, without having to adapt their code to either use case.
  #
  INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}"
  # Copy over any interface-facing headers.
  FILE_SET ${MY_BASE_PROJECT_NAME_FULL_LOWERCASE}_include_interface_headers DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}"
  )
