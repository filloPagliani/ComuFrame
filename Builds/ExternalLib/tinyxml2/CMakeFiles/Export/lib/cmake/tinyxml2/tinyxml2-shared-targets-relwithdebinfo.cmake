#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "tinyxml2::tinyxml2" for configuration "RelWithDebInfo"
set_property(TARGET tinyxml2::tinyxml2 APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(tinyxml2::tinyxml2 PROPERTIES
  IMPORTED_IMPLIB_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/tinyxml2.lib"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/bin/tinyxml2.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS tinyxml2::tinyxml2 )
list(APPEND _IMPORT_CHECK_FILES_FOR_tinyxml2::tinyxml2 "${_IMPORT_PREFIX}/lib/tinyxml2.lib" "${_IMPORT_PREFIX}/bin/tinyxml2.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
