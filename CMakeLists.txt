#
#  fhiclcpp/CMakeLists.txt
#  -------------------------------------------------------------------
#
#  CMake build file for library fhiclcpp and fhiclcppD
#
#   cmake [-DCMAKE_INSTALL_PREFIX=/install/path] /path/to/source/code

CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

PROJECT (fhiclcpp)
SET (version v2_0_0)
SET (qualifier a1)

IF (CMAKE_CROSSCOMPILING)
    SET (flavorqual sl5.${CMAKE_SYSTEM_PROCESSOR}.${qualifier})
ELSE (CMAKE_CROSSCOMPILING)
    SET (flavorqual sl5.${CMAKE_HOST_SYSTEM_PROCESSOR}.${qualifier})
ENDIF (CMAKE_CROSSCOMPILING)

SET (flavorqual_dir ${version}/${flavorqual} )

# make sure gcc has been set
SET ( GCC_VERSION $ENV{GCC_VERSION} )
IF (NOT GCC_VERSION)
    MESSAGE ("GCC has not been setup")
ENDIF()
message(STATUS "GCC version is ${GCC_VERSION} " )

# Check if the boost library has been set
SET ( BOOST_VERS $ENV{BOOST_VERSION} )
IF (NOT BOOST_VERS)
    MESSAGE (FATAL_ERROR "Boost library has not been setup")
ENDIF()
SET ( BOOST_STRING $ENV{SETUP_BOOST} )
STRING( REGEX REPLACE ".*(-q.*)" "\\1" BOOST_QUAL "${BOOST_STRING}" )
message(STATUS "Boost version and qualifier are ${BOOST_VERS} ${BOOST_QUAL}" )

# Define the boost envrionment so we don't get system libraries
set ( BOOST_ROOT       $ENV{BOOST_DIR} )
set ( BOOST_INCLUDEDIR $ENV{BOOST_INC} )
set ( BOOST_LIBRARYDIR $ENV{BOOST_LIB} )
FIND_PACKAGE ( Boost 1.41 REQUIRED )

# Project include directory
INCLUDE_DIRECTORIES (
    ${fhiclcpp_SOURCE_DIR}/interface
    ${Boost_INCLUDE_DIR}
    )

# making a copy of the source code
INSTALL (FILES CMakeLists.txt
    DESTINATION ${version}/src/fhiclcpp
    )

# source code
subdirs(interface)
subdirs(src)
subdirs(ups)

# testing
ENABLE_TESTING()
ADD_SUBDIRECTORY (test)
