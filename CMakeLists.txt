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

cmake_minimum_required (VERSION 2.6)

project (fhiclcpp)
set (product fhiclcpp)
set (version v2_2_2)
set (qualifier a1)

# because we want to move these libraries about,
# do not embed full path in shared libraries or executables
set (CMAKE_SKIP_RPATH)

# cetbuildtools contains our cmake modules
SET ( CETBUILDTOOLS_VERSION $ENV{CETBUILDTOOLS_VERSION} )
IF (NOT CETBUILDTOOLS_VERSION)
    MESSAGE (FATAL_ERROR "ERROR: setup cetbuildtools to get the cmake modules")
ENDIF()
set(CMAKE_MODULE_PATH "${CMAKE_ROOT}/Modules")
set(CMAKE_MODULE_PATH $ENV{CETBUILDTOOLS_DIR}/Modules ${CMAKE_MODULE_PATH})
message(STATUS "module path is ${CMAKE_MODULE_PATH}" )

# define some necessary modules
include(FindUpsPackage)
include(FindUpsBoost)
include(ParseUpsVersion)
include(SetFlavorQual)

#set package version from ups version
set_version_from_ups( ${version} )
#define flavorqual and flavorqual_dir
set_flavor_qual()

# make sure gcc has been set
set ( GCC_VERSION $ENV{GCC_VERSION} )
if (NOT GCC_VERSION)
    message ("GCC has not been setup")
endif()
message(STATUS "Using GCC ${GCC_VERSION} " )

# these are minimum required versions, not the actual product versions
find_ups_product( cpp0x v0_0_0 )
find_ups_product( cetlib v0_1_1 )
find_ups_boost( v1_41_0 )

# Project include directory
include_directories (    ${PROJECT_SOURCE_DIR}    )

# Project library directory
link_directories(
    $ENV{CETLIB_LIB}
    )

# source code
subdirs(fhiclcpp)
subdirs(ups)

# testing
enable_testing()
add_subdirectory (test)

# packaging utility
include(UseCPack)
