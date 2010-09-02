#
#  ParameterSet/CMakeLists.txt
#  -------------------------------------------------------------------
#
#  CMake build file for library ParameterSet and ParameterSetD
#

CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

PROJECT (fhicl-cpp)

# Check if the boost library has been set
SET ( BOOST_VERSION $ENV{BOOST_VERSION} )
IF (NOT BOOST_VERSION)
    MESSAGE (FATAL_ERROR "Boost library has not been setup")
ENDIF()

# Define the boost envrionment so we don't get system libraries
set ( BOOST_ROOT       $ENV{BOOST_DIR} )
set ( BOOST_INCLUDEDIR $ENV{BOOST_INC} )
set ( BOOST_LIBRARYDIR $ENV{BOOST_LIB} )

FIND_PACKAGE ( Boost 1.41 REQUIRED )

# Project include directory
INCLUDE_DIRECTORIES (
    ${fhicl-cpp_SOURCE_DIR}/interface
    ${Boost_INCLUDE_DIR}
    )

# Add source files in the src/ diretory
INCLUDE (src/CMakeLists.txt)

FOREACH (FILE ${FILES})
    SET (srcFiles ${srcFiles} src/${FILE})
ENDFOREACH (FILE)

ADD_LIBRARY (fhicl-cppD SHARED ${srcFiles})  
ADD_LIBRARY (fhicl-cpp         ${srcFiles})  

SET_TARGET_PROPERTIES (fhicl-cppD
    PROPERTIES OUTPUT_NAME fhicl-cpp
    )

SET_TARGET_PROPERTIES (fhicl-cpp
    PROPERTIES OUTPUT_NAME fhicl-cpp
    )

# Check if the install prefix is set
IF (NOT CMAKE_INSTALL_PREFIX)
    SET ( CMAKE_INSTALL_PREFIX
        ${fhicl-cpp_SOURCE_DIR}/install
        )
ENDIF(NOT CMAKE_INSTALL_PREFIX)


# Install files
SET ( CMAKE_INSTALL_PREFIX 
    ${fhicl-cpp_SOURCE_DIR}/install
    )

FILE (GLOB includeFiles
    ${fhicl-cpp_SOURCE_DIR}/interface/*.h
    )

LIST (APPEND srcFiles
    src/CMakeLists.txt
    )

FILE (GLOB cmakeFiles
    ${fhicl-cpp_SOURCE_DIR}/CMakeLists.txt
    )

INSTALL (FILES ${includeFiles}
    DESTINATION include/
    )

INSTALL (FILES ${includeFiles}
    DESTINATION src/ParameterSet/interface
    )

INSTALL (FILES ${srcFiles}
    DESTINATION src/ParameterSet/src
    )

INSTALL (FILES ${cmakeFiles}
    DESTINATION src/ParameterSet
    )

INSTALL (TARGETS fhicl-cppD fhicl-cpp 
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    ) 

# Add subdirectories
ADD_SUBDIRECTORY (test)
ADD_SUBDIRECTORY (ups)
