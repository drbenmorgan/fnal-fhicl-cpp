#
#  fhiclcpp/CMakeLists.txt
#  -------------------------------------------------------------------
#
#  CMake build file for library fhiclcpp and fhiclcppD
#
#  cd .../path/to/build/directory
#  source .../path/to/source/code/setup_for_development
#  cmake [-DCMAKE_INSTALL_PREFIX=/install/path]
#        -DCMAKE_BUILD_TYPE=$CETPKG_TYPE
#        $CETPKG_SOURCE
#  make
#  make test
#  make install
#  make package (builds distribution tarfile)

cmake_minimum_required(VERSION 3.3)
project(fhiclcpp VERSION 3.18.3)

# cetbuildtools contains our cmake modules
# - Cetbuildtools, version2
find_package(cetbuildtools2 0.1.0 REQUIRED)
set(CMAKE_MODULE_PATH ${cetbuildtools2_MODULE_PATH})
include(CetInstallDirs)
include(CetCMakeSettings)
include(CetCompilerSettings)

# C++ Standard Config
set(CMAKE_CXX_EXTENSIONS OFF)
set(cetlib_COMPILE_FEATURES
  cxx_auto_type
  cxx_generic_lambdas
  )

# these are minimum required versions, not the actual product versions
find_package(cetlib 1.17 REQUIRED)
find_package(Boost 1.60 REQUIRED unit_test_framework program_options)

#find_package(sqlite 3.7.15 REQUIRED)
#-----------------------------------------------------------------------
# FindSQLite - to be refactored...
#-----------------------------------------------------------------------
find_path(SQLite_INCLUDE_DIRS sqlite3.h)
find_library(SQLite_LIBRARY sqlite3)
find_program(SQLite_EXECUTABLE sqlite3)

if(EXISTS "${SQLite_EXECUTABLE}")
  execute_process(
    COMMAND "${SQLite_EXECUTABLE}" --version
    OUTPUT_VARIABLE SQLite_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
  # sqlite3 program outputs "version date time hash"
  # so only take version part
  string(REGEX REPLACE " .*$" "" SQLite_VERSION ${SQLite_VERSION})
endif()

#-----------------------------------------------------------------------
# Handle standard args
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SQLite
  FOUND_VAR
    SQLite_FOUND
  REQUIRED_VARS
    SQLite_INCLUDE_DIRS
    SQLite_LIBRARY
  VERSION_VAR
    SQLite_VERSION
    )

add_library(SQLite::SQLite UNKNOWN IMPORTED)
set_target_properties(SQLite::SQLite PROPERTIES
  IMPORTED_LOCATION "${SQLITE_LIBRARY}"
  IMPORTED_INTERFACE_INCLUDE_DIRECTORIES "${SQLITE_INCLUDE_DIRS}")
add_executable(SQLite::SQLiteExecutable IMPORTED)
set_target_properties(SQLite::SQLiteExecutable PROPERTIES
  IMPORTED_LOCATION "${SQLITE_EXECUTABLE}")
#-----------------------------------------------------------------------
# FINDSQLITE END
#-----------------------------------------------------------------------

# source code
add_subdirectory(fhiclcpp)
add_subdirectory(tools)
#add_subdirectory( ups )
if(BUILD_TESTING)
  add_subdirectory(test)
endif()

# packaging utility
#include(UseCPack)
