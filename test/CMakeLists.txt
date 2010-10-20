#
#  Test/CMakeLists.txt
#  -------------------------------------------------------------------
#
#  CMake build file for tests
#

configure_file (
    ${CMAKE_CURRENT_SOURCE_DIR}/Sample.cfg
    ${CMAKE_CURRENT_BINARY_DIR}/Sample.cfg
    COPYONLY
    )

add_executable (ParserTest
    PSetSample.cc
    )

target_link_libraries (ParserTest
    fhiclcppD
    )

add_executable (PSetTest
    PSetTest.cc
    )

target_link_libraries (PSetTest
    fhiclcppD
    )

add_test(ParserTest ParserTest)
add_test(PSetTest PSetTest)

install (FILES
    PSetSample.cc
    Sample.cfg
    DESTINATION ${version}/example
    )

install (FILES
    PSetSample.cc
    DESTINATION ${version}/source/test
    )
