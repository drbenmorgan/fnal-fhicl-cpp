#
#  ParameterSet/CMakeLists.txt
#  -------------------------------------------------------------------
#
#  CMake build file for library ParameterSet and ParameterSetD
#

CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

PROJECT (FHiCLCpp)

INCLUDE (src/CMakeLists.txt)

INCLUDE_DIRECTORIES (
    ${FHiCLCpp_SOURCE_DIR}/interface
    $ENV{BOOST_DIR}/include
    )

# add all source files in the src/ diretory
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


#install files
SET ( CMAKE_INSTALL_PREFIX 
    ${FHiCLCpp_SOURCE_DIR}/build
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
