#
#  fhiclcpp/CMakeLists.txt
#  -------------------------------------------------------------------
#
#  CMake build file for library fhiclcpp and fhiclcppD
#
#   cmake [-DCMAKE_INSTALL_PREFIX=/install/path] /path/to/source/code

cmake_minimum_required (VERSION 2.6)

project (fhiclcpp)
set (version v2_0_0)
set (qualifier a1)

if (CMAKE_CROSSCOMPILING)
    set (flavorqual sl5.${CMAKE_SYSTEM_PROCESSOR}.${qualifier})
else (CMAKE_CROSSCOMPILING)
    set (flavorqual sl5.${CMAKE_HOST_SYSTEM_PROCESSOR}.${qualifier})
endif (CMAKE_CROSSCOMPILING)

set (flavorqual_dir ${version}/${flavorqual} )

# make sure gcc has been set
set ( GCC_VERSION $ENV{GCC_VERSION} )
if (NOT GCC_VERSION)
    message ("GCC has not been setup")
endif()
message(STATUS "Using GCC ${GCC_VERSION} " )

# Check if the cetlib library has been set
set ( CETLIB_VERS $ENV{CETLIB_VERSION} )
if (NOT CETLIB_VERS)
    message (FATAL_ERROR "cetlib library has not been setup")
endif()
set ( CETLIB_STRING $ENV{SETUP_CETLIB} )
string( REGEX REPLACE ".*(-q.*)" "\\1" CETLIB_QUAL "${CETLIB_STRING}" )
message(STATUS "Using CETLIB ${GCC_VERSION} " )

# Check if the boost library has been set
set ( BOOST_VERS $ENV{BOOST_VERSION} )
if (NOT BOOST_VERS)
    message (FATAL_ERROR "Boost library has not been setup")
endif()
set ( BOOST_STRING $ENV{SETUP_BOOST} )
string( REGEX REPLACE ".*(-q.*)" "\\1" BOOST_QUAL "${BOOST_STRING}" )
message(STATUS "Using Boost ${BOOST_VERS} ${BOOST_QUAL}" )

# Define the boost environment so we don't get system libraries
set ( BOOST_ROOT       $ENV{BOOST_DIR} )
set ( BOOST_INCLUDEDIR $ENV{BOOST_INC} )
set ( BOOST_LIBRARYDIR $ENV{BOOST_LIB} )
find_package ( Boost 1.41 REQUIRED )

# Project include directory
include_directories (
    ${PROJECT_SOURCE_DIR}
    ${Boost_INCLUDE_DIR}
    $ENV{CETLIB_INC}
    $ENV{CPP0X_INC}
    )

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
