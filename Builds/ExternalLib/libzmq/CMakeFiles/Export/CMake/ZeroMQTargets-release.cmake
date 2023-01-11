#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "libzmq" for configuration "Release"
set_property(TARGET libzmq APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(libzmq PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/libzmq-v143-mt-4_3_5.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/libzmq-v143-mt-4_3_5.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS libzmq )
list(APPEND _IMPORT_CHECK_FILES_FOR_libzmq "${_IMPORT_PREFIX}/lib/libzmq-v143-mt-4_3_5.lib" "${_IMPORT_PREFIX}/bin/libzmq-v143-mt-4_3_5.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
