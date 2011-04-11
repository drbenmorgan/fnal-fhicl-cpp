#
#  fhiclcpp/CMakeLists.txt
#  -------------------------------------------------------------------
#
#  CMake build file for library fhiclcpp and fhiclcppD
#
#  cd .../path/to/build/directory
#  source .../path/to/source/code/setup_for_development
#  cmake [-DCMAKE_INSTALL_PREFIX=/install/path] $FHICLCPP_SOURCE
#  make
#  make test
#  make install
#  make package

cmake_minimum_required( VERSION 2.6 )

project( fhiclcpp )
set( product fhiclcpp )
set( version v2_12_01 )
set( qualifier a2 )

# because we want to move these libraries about,
# do not embed full path in shared libraries or executables
set( CMAKE_SKIP_RPATH )

# cetbuildtools contains our cmake modules
set( CETBUILDTOOLS_VERSION $ENV{CETBUILDTOOLS_VERSION} )
if( NOT CETBUILDTOOLS_VERSION )
  message( FATAL_ERROR "ERROR: setup cetbuildtools to get the cmake modules" )
endif()
set( CMAKE_MODULE_PATH "${CMAKE_ROOT}/Modules" )
set( CMAKE_MODULE_PATH $ENV{CETBUILDTOOLS_DIR}/Modules ${CMAKE_MODULE_PATH} )
message( STATUS "module path is ${CMAKE_MODULE_PATH}" )

# make sure someone isn't typing cmake in the source code tree
include(EnsureOutOfSourceBuild)
cet_ensure_out_of_source_build()

# define some necessary modules
include( FindUpsPackage )
include( FindUpsBoost )
include( ParseUpsVersion )
include( SetFlavorQual )

#set package version from ups version
set_version_from_ups( ${version} )
#define flavorqual and flavorqual_dir
set_flavor_qual()

# ensure gcc has been set
set( GCC_VERSION $ENV{GCC_VERSION} )
if( NOT GCC_VERSION )
  message( "GCC has not been setup" )
endif()
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall" )
message( STATUS "Using GCC ${GCC_VERSION} with ${CMAKE_CXX_FLAGS}" )

# these are minimum required versions, not the actual product versions
find_ups_product( cetbuildtools v1_10_02 )
find_ups_product( cetlib v0_3_2 )
find_ups_product( cpp0x v1_0_1 )
find_ups_boost( v1_45_0
  unit_test_framework
  program_options
  )

# Project include directory
include_directories( ${PROJECT_SOURCE_DIR} )

# Project library directory
link_directories( $ENV{CETLIB_LIB}
                  $ENV{BOOST_LIB}  )

#build all libraries in a single directory to enable testing
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
#build all executables in a single directory
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

# source code
add_subdirectory( fhiclcpp )
add_subdirectory( ups )

# testing
enable_testing()
add_subdirectory( test )

# packaging utility
include( UseCPack )
