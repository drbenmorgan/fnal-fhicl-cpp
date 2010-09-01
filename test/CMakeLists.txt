#
#  Test/CMakeLists.txt
#  -------------------------------------------------------------------
#
#  CMake build file for tests
#

CONFIGURE_FILE (
    ${CMAKE_CURRENT_SOURCE_DIR}/Sample.cfg
    ${CMAKE_CURRENT_BINARY_DIR}/Sample.cfg
    COPYONLY
    )

ADD_EXECUTABLE (PSetTest
    PSetSample.cc
    )

TARGET_LINK_LIBRARIES (PSetTest
    FHiCLCppD
    )

ADD_EXECUTABLE (PSetInsert
    PSetInsert.cc
    )

TARGET_LINK_LIBRARIES (PSetInsert
    FHiCLCppD
    )

INSTALL (FILES
    PSetSample.cc
    Sample.cfg
    DESTINATION example
    )

INSTALL (FILES
    CMakeLists.txt
    PSetSample.cc
    DESTINATION src/test
    )
