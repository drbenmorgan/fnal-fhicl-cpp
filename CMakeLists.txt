#
#  ParameterSet/CMakeLists.txt
#  -------------------------------------------------------------------
#
#  CMake build file for library ParameterSet and ParameterSetD
#

CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

PROJECT (FHiCLCpp)

# Define the boost envrionment so we don't get system libraries
set ( BOOST_ROOT       $ENV{BOOST_DIR} )
set ( BOOST_INCLUDEDIR $ENV{BOOST_INC} )
set ( BOOST_LIBRARYDIR $ENV{BOOST_LIB} )

FIND_PACKAGE ( Boost 1.42 REQUIRED ) 

# Project include directory
INCLUDE_DIRECTORIES (
    ${FHiCLCpp_SOURCE_DIR}/interface
    ${Boost_INCLUDE_DIR}
    )

# Add source files in the src/ diretory
INCLUDE (src/CMakeLists.txt)

FOREACH (FILE ${FILES})
    SET (srcFiles ${srcFiles} src/${FILE})
ENDFOREACH (FILE)

ADD_LIBRARY (FHiCLCppD SHARED ${srcFiles})  
ADD_LIBRARY (FHiCLCpp         ${srcFiles})  

SET_TARGET_PROPERTIES (FHiCLCppD
    PROPERTIES OUTPUT_NAME FHiclCpp
    )

SET_TARGET_PROPERTIES (FHiCLCpp
    PROPERTIES OUTPUT_NAME FHiCLCpp
    )


# Install files
SET ( CMAKE_INSTALL_PREFIX 
    ${FHiCLCpp_SOURCE_DIR}/install
    )

FILE (GLOB includeFiles
    ${FHiCLCpp_SOURCE_DIR}/interface/*.h
    )

LIST (APPEND srcFiles
    src/CMakeLists.txt
    )

FILE (GLOB cmakeFiles
    ${FHiCLCpp_SOURCE_DIR}/CMakeLists.txt
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

INSTALL (TARGETS FHiCLCpp FHiCLCppD
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    ) 
