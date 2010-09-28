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

ADD_EXECUTABLE (ParserTest
    PSetSample.cc
    )

TARGET_LINK_LIBRARIES (ParserTest
    fhiclcppD
    )

ADD_EXECUTABLE (PSetTest
    PSetTest.cc
    )

TARGET_LINK_LIBRARIES (PSetTest
    fhiclcppD
    )

ADD_TEST(ParserTest ParserTest)
ADD_TEST(PSetTest PSetTest)

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
