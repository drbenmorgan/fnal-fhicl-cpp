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

cmake_minimum_required( VERSION 2.8 )  # use cmake 2.8 or later

project(fhiclcpp)

# cetbuildtools contains our cmake modules
set( CETBUILDTOOLS_VERSION $ENV{CETBUILDTOOLS_VERSION} )
if( NOT CETBUILDTOOLS_VERSION )
  message( FATAL_ERROR "ERROR: setup cetbuildtools to get the cmake modules" )
endif()
set( CMAKE_MODULE_PATH $ENV{CETBUILDTOOLS_DIR}/Modules ${CMAKE_MODULE_PATH} )

include(CetCMakeEnv)
cet_cmake_env()

cet_set_compiler_flags(DIAGS VIGILANT 
                       WERROR 
		       NO_UNDEFINED
		       EXTRA_CXX_FLAGS -std=c++11)
cet_report_compiler_flags()

cet_check_gcc()

# these are minimum required versions, not the actual product versions
find_ups_product( cetbuildtools v3_02_00 )
find_ups_product( cetlib v1_03_02 )

# source code
add_subdirectory( fhiclcpp )
add_subdirectory( ups )
add_subdirectory( test )

# packaging utility
include(UseCPack)
