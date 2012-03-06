#
#  fhiclcpp/CMakeLists.txt
#  -------------------------------------------------------------------
#
#  CMake build file for library fhiclcpp and fhiclcppD
#
#  cd .../path/to/build/directory
#  source .../path/to/source/code/setup_for_development
#  cmake [-DCMAKE_INSTALL_PREFIX=/install/path]
#        -Dqualifier:STRING=$CETPKG_QUAL
#        -DCMAKE_BUILD_TYPE=$CETPKG_TYPE
#        $CETPKG_SOURCE
#  make
#  make test
#  make install
#  make package

cmake_minimum_required( VERSION 2.8 )

project(fhiclcpp)

# cetbuildtools contains our cmake modules
set( CETBUILDTOOLS_VERSION $ENV{CETBUILDTOOLS_VERSION} )
if( NOT CETBUILDTOOLS_VERSION )
  message( FATAL_ERROR "ERROR: setup cetbuildtools to get the cmake modules" )
endif()
set( CMAKE_MODULE_PATH $ENV{CETBUILDTOOLS_DIR}/Modules ${CMAKE_MODULE_PATH} )

include(CetCMakeEnv)

if ( NOT qualifier )
    message (FATAL_ERROR "ERROR: please call cmake with -Dqualifier:STRING=qualifier")
endif ( NOT qualifier )
cet_cmake_env( fhiclcpp v2_16_07 ${qualifier} )

cet_set_compiler_flags(DIAGS VIGILANT WERROR)
if ("${qualifier}" STREQUAL "e1")
  cet_add_compiler_flags(CXX -std=c++0x;-Wno-deprecated-declarations)
endif()
cet_report_compiler_flags()

cet_check_gcc()

# these are minimum required versions, not the actual product versions
find_ups_product( cetbuildtools v1_15_00 )
find_ups_product( cpp0x v1_03_02 )
find_ups_product( cetlib v1_03_02 )
find_ups_boost( v1_48_0
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
