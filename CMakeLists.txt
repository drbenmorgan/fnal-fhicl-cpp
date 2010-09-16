#
#  fhiclcpp/CMakeLists.txt
#  -------------------------------------------------------------------
#
#  CMake build file for library fhiclcpp and fhiclcppD
#

CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

PROJECT (fhiclcpp)
SET (version v1_0_2)

# Check if the boost library has been set
SET (BOOST_VER $ENV{BOOST_VERSION})
IF (NOT BOOST_VER)
    MESSAGE (FATAL_ERROR "Boost library has not been setup")
ENDIF()

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

# Check if the install prefix is set
#IF (NOT CMAKE_INSTALL_PREFIX)
    SET ( CMAKE_INSTALL_PREFIX
        ${fhiclcpp_BINARY_DIR}/install
        )
#ENDIF(NOT CMAKE_INSTALL_PREFIX)


# Install files
SET ( CMAKE_INSTALL_PREFIX 
    ${fhiclcpp_SOURCE_DIR}/install
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
    DESTINATION include/fhiclcpp/
    )

INSTALL (FILES ${includeFiles}
    DESTINATION src/fhiclcpp/interface
    )

INSTALL (FILES ${srcFiles}
    DESTINATION src/fhiclcpp/src
    )

INSTALL (FILES ${cmakeFiles}
    DESTINATION src/fhiclcpp
    )

INSTALL (TARGETS fhiclcppD fhiclcpp 
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    ) 

# Add subdirectories
ENABLE_TESTING()
ADD_SUBDIRECTORY (test)
ADD_SUBDIRECTORY (ups)
