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
project(fhiclcpp VERSION 3.20.1)

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
find_package(SQLite 3.7.15 REQUIRED)

# source code
add_subdirectory(fhiclcpp)
add_subdirectory(tools)

#-----------------------------------------------------------------------
# Documentation
#
find_package(Doxygen 1.8)
if(DOXYGEN_FOUND)
  set(DOXYGEN_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/Doxygen")
  configure_file(Doxyfile.in Doxyfile @ONLY)
  add_custom_command(
    OUTPUT "${DOXYGEN_OUTPUT_DIR}/html/index.html"
    COMMAND "${DOXYGEN_EXECUTABLE}" "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile"
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile" fhiclcpp
    COMMENT "Generating Doxygen docs for ${PROJECT_NAME}"
    )
  add_custom_target(doc ALL DEPENDS "${DOXYGEN_OUTPUT_DIR}/html/index.html")
  install(DIRECTORY "${DOXYGEN_OUTPUT_DIR}/"
    DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/API"
    )
endif()
# packaging utility
#include(UseCPack)
