#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "sc-machine::sc-machine" for configuration "Release"
set_property(TARGET sc-machine::sc-machine APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(sc-machine::sc-machine PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/sc-machine"
  )

list(APPEND _cmake_import_check_targets sc-machine::sc-machine )
list(APPEND _cmake_import_check_files_for_sc-machine::sc-machine "${_IMPORT_PREFIX}/bin/sc-machine" )

# Import target "sc-machine::sc-builder" for configuration "Release"
set_property(TARGET sc-machine::sc-builder APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(sc-machine::sc-builder PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/sc-builder"
  )

list(APPEND _cmake_import_check_targets sc-machine::sc-builder )
list(APPEND _cmake_import_check_files_for_sc-machine::sc-builder "${_IMPORT_PREFIX}/bin/sc-builder" )

# Import target "sc-machine::sc-machine-runner" for configuration "Release"
set_property(TARGET sc-machine::sc-machine-runner APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(sc-machine::sc-machine-runner PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libsc-machine-runner.dylib"
  IMPORTED_SONAME_RELEASE "@rpath/libsc-machine-runner.dylib"
  )

list(APPEND _cmake_import_check_targets sc-machine::sc-machine-runner )
list(APPEND _cmake_import_check_files_for_sc-machine::sc-machine-runner "${_IMPORT_PREFIX}/lib/libsc-machine-runner.dylib" )

# Import target "sc-machine::sc-core" for configuration "Release"
set_property(TARGET sc-machine::sc-core APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(sc-machine::sc-core PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libsc-core.dylib"
  IMPORTED_SONAME_RELEASE "@rpath/libsc-core.dylib"
  )

list(APPEND _cmake_import_check_targets sc-machine::sc-core )
list(APPEND _cmake_import_check_files_for_sc-machine::sc-core "${_IMPORT_PREFIX}/lib/libsc-core.dylib" )

# Import target "sc-machine::sc-memory" for configuration "Release"
set_property(TARGET sc-machine::sc-memory APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(sc-machine::sc-memory PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libsc-memory.dylib"
  IMPORTED_SONAME_RELEASE "@rpath/libsc-memory.dylib"
  )

list(APPEND _cmake_import_check_targets sc-machine::sc-memory )
list(APPEND _cmake_import_check_files_for_sc-machine::sc-memory "${_IMPORT_PREFIX}/lib/libsc-memory.dylib" )

# Import target "sc-machine::sc-agents-common" for configuration "Release"
set_property(TARGET sc-machine::sc-agents-common APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(sc-machine::sc-agents-common PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libsc-agents-common.dylib"
  IMPORTED_SONAME_RELEASE "@rpath/libsc-agents-common.dylib"
  )

list(APPEND _cmake_import_check_targets sc-machine::sc-agents-common )
list(APPEND _cmake_import_check_files_for_sc-machine::sc-agents-common "${_IMPORT_PREFIX}/lib/libsc-agents-common.dylib" )

# Import target "sc-machine::sc-builder-lib" for configuration "Release"
set_property(TARGET sc-machine::sc-builder-lib APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(sc-machine::sc-builder-lib PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libsc-builder-lib.dylib"
  IMPORTED_SONAME_RELEASE "@rpath/libsc-builder-lib.dylib"
  )

list(APPEND _cmake_import_check_targets sc-machine::sc-builder-lib )
list(APPEND _cmake_import_check_files_for_sc-machine::sc-builder-lib "${_IMPORT_PREFIX}/lib/libsc-builder-lib.dylib" )

# Import target "sc-machine::sc-config" for configuration "Release"
set_property(TARGET sc-machine::sc-config APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(sc-machine::sc-config PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libsc-config.dylib"
  IMPORTED_SONAME_RELEASE "@rpath/libsc-config.dylib"
  )

list(APPEND _cmake_import_check_targets sc-machine::sc-config )
list(APPEND _cmake_import_check_files_for_sc-machine::sc-config "${_IMPORT_PREFIX}/lib/libsc-config.dylib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
