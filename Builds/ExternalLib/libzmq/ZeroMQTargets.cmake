# Generated by CMake

if("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" LESS 2.6)
   message(FATAL_ERROR "CMake >= 2.6.0 required")
endif()
cmake_policy(PUSH)
cmake_policy(VERSION 2.6...3.21)
#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Protect against multiple inclusion, which would fail when already imported targets are added once more.
set(_targetsDefined)
set(_targetsNotDefined)
set(_expectedTargets)
foreach(_expectedTarget libzmq)
  list(APPEND _expectedTargets ${_expectedTarget})
  if(NOT TARGET ${_expectedTarget})
    list(APPEND _targetsNotDefined ${_expectedTarget})
  endif()
  if(TARGET ${_expectedTarget})
    list(APPEND _targetsDefined ${_expectedTarget})
  endif()
endforeach()
if("${_targetsDefined}" STREQUAL "${_expectedTargets}")
  unset(_targetsDefined)
  unset(_targetsNotDefined)
  unset(_expectedTargets)
  set(CMAKE_IMPORT_FILE_VERSION)
  cmake_policy(POP)
  return()
endif()
if(NOT "${_targetsDefined}" STREQUAL "")
  message(FATAL_ERROR "Some (but not all) targets in this export set were already defined.\nTargets Defined: ${_targetsDefined}\nTargets not yet defined: ${_targetsNotDefined}\n")
endif()
unset(_targetsDefined)
unset(_targetsNotDefined)
unset(_expectedTargets)


# Create imported target libzmq
add_library(libzmq SHARED IMPORTED)

set_target_properties(libzmq PROPERTIES
  INTERFACE_COMPILE_DEFINITIONS "ZMQ_BUILD_DRAFT_API"
  INTERFACE_INCLUDE_DIRECTORIES "C:/Users/pagliani/source/repos/ComuFrame/ExternalLib/libzmq/include;C:/Users/pagliani/source/repos/ComuFrame/Builds/ExternalLib/libzmq"
  INTERFACE_LINK_LIBRARIES "ws2_32;rpcrt4;iphlpapi"
)

# Import target "libzmq" for configuration "Debug"
set_property(TARGET libzmq APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(libzmq PROPERTIES
  IMPORTED_IMPLIB_DEBUG "C:/Users/pagliani/source/repos/ComuFrame/Builds/lib/Debug/libzmq-v143-mt-gd-4_3_5.lib"
  IMPORTED_LOCATION_DEBUG "C:/Users/pagliani/source/repos/ComuFrame/Builds/bin/Debug/libzmq-v143-mt-gd-4_3_5.dll"
  )

# Import target "libzmq" for configuration "Release"
set_property(TARGET libzmq APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(libzmq PROPERTIES
  IMPORTED_IMPLIB_RELEASE "C:/Users/pagliani/source/repos/ComuFrame/Builds/lib/Release/libzmq-v143-mt-4_3_5.lib"
  IMPORTED_LOCATION_RELEASE "C:/Users/pagliani/source/repos/ComuFrame/Builds/bin/Release/libzmq-v143-mt-4_3_5.dll"
  )

# Import target "libzmq" for configuration "MinSizeRel"
set_property(TARGET libzmq APPEND PROPERTY IMPORTED_CONFIGURATIONS MINSIZEREL)
set_target_properties(libzmq PROPERTIES
  IMPORTED_IMPLIB_MINSIZEREL "C:/Users/pagliani/source/repos/ComuFrame/Builds/lib/MinSizeRel/libzmq-v143-mt-4_3_5.lib"
  IMPORTED_LOCATION_MINSIZEREL "C:/Users/pagliani/source/repos/ComuFrame/Builds/bin/MinSizeRel/libzmq-v143-mt-4_3_5.dll"
  )

# Import target "libzmq" for configuration "RelWithDebInfo"
set_property(TARGET libzmq APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(libzmq PROPERTIES
  IMPORTED_IMPLIB_RELWITHDEBINFO "C:/Users/pagliani/source/repos/ComuFrame/Builds/lib/RelWithDebInfo/libzmq-v143-mt-4_3_5.lib"
  IMPORTED_LOCATION_RELWITHDEBINFO "C:/Users/pagliani/source/repos/ComuFrame/Builds/bin/RelWithDebInfo/libzmq-v143-mt-4_3_5.dll"
  )

# This file does not depend on other imported targets which have
# been exported from the same project but in a separate export set.

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
cmake_policy(POP)
