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
    fhiclcppD
    )

ADD_EXECUTABLE (PSetInsert
    PSetInsert.cc
    )

TARGET_LINK_LIBRARIES (PSetInsert
    fhiclcppD
    )

ADD_TEST(PSetTest PSetTest)
ADD_TEST(PSetInsert PSetInsert)

INSTALL (FILES
    PSetSample.cc
    Sample.cfg
    DESTINATION ${version}/example
    )

INSTALL (FILES
    CMakeLists.txt
    PSetSample.cc
    DESTINATION ${version}/src/test
    )
