#
#  fhiclcpp/CMakeLists.txt
#  -------------------------------------------------------------------
#
#  CMake build file for library fhiclcpp and fhiclcppD
#
#   cmake [-DCMAKE_INSTALL_PREFIX=/install/path] /path/to/source/code

CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

PROJECT (fhiclcpp)
SET (version v1_0_2)
SET (qualifier a1)
SET (flavorqual sl5.x86_64.${qualifier})
SET (flavorqual_dir ${version}/${flavorqual} )

# make sure gcc has been set
SET ( GCC_VERSION $ENV{GCC_VERSION} )
IF (NOT GCC_VERSION)
    MESSAGE (FATAL_ERROR "GCC has not been setup")
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

# Add source files in the src/ diretory
INCLUDE (src/CMakeLists.txt)

FOREACH (FILE ${FILES})
    SET (srcFiles ${srcFiles} src/${FILE})
ENDFOREACH (FILE)

ADD_LIBRARY (fhiclcppD SHARED ${srcFiles})  
ADD_LIBRARY (fhiclcpp         ${srcFiles})  

SET_TARGET_PROPERTIES (fhiclcppD
    PROPERTIES OUTPUT_NAME fhiclcpp
    )

SET_TARGET_PROPERTIES (fhiclcpp
    PROPERTIES OUTPUT_NAME fhiclcpp
    )

FILE (GLOB includeFiles
    ${fhiclcpp_SOURCE_DIR}/interface/*.h
    )

LIST (APPEND srcFiles
    src/CMakeLists.txt
    )

FILE (GLOB cmakeFiles
    ${fhiclcpp_SOURCE_DIR}/CMakeLists.txt
    )

INSTALL (FILES ${includeFiles}
    DESTINATION ${version}/include/fhiclcpp/
    )

INSTALL (FILES ${includeFiles}
    DESTINATION ${version}/src/fhiclcpp/interface
    )

INSTALL (FILES ${srcFiles}
    DESTINATION ${version}/src/fhiclcpp/src
    )

INSTALL (FILES ${cmakeFiles}
    DESTINATION ${version}/src/fhiclcpp
    )

INSTALL (TARGETS fhiclcppD fhiclcpp 
    RUNTIME DESTINATION ${flavorqual_dir}/bin
    LIBRARY DESTINATION ${flavorqual_dir}/lib
    ARCHIVE DESTINATION ${flavorqual_dir}/lib
    ) 

# Add subdirectories
ENABLE_TESTING()
ADD_SUBDIRECTORY (test)
ADD_SUBDIRECTORY (ups)
