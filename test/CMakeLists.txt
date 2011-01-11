# ======================================================================
#
# Testing script
#
# ======================================================================

# ----------------------------------------------------------------------
# Identify tests:

set( TEST_LIST
  bool_test
  complex_test
  float_test
  int_test
  nil_test
  sequence_test
  string_test
  table_test
  unsigned_test
  parse_value_test
  to_string_test
  PSetTest
)

set( BOOST_TEST_LIST
  ParameterSet_t
)

set( ALSO_BUILD
)

set( BOOST_ALSO_BUILD
)

set( ALSO_RUN
  PSetSample
)

set( INPUT_FILES
  Sample.cfg
)

# ----------------------------------------------------------------------
# Build most tests:

foreach( test ${TEST_LIST} ${ALSO_BUILD} )
  add_executable( ${test} ${test}.cc )
  target_link_libraries( ${test} fhiclcppD )
endforeach( test ${TEST_LIST} ${ALSO_BUILD} )

# ----------------------------------------------------------------------
# Build BOOST tests

foreach( test ${BOOST_TEST_LIST} ${BOOST_ALSO_BUILD} )
  add_executable( ${test} ${test}.cc )
  # Save having to put these in all tests.
  set_target_properties ( ${test} PROPERTIES
    COMPILE_DEFINITIONS BOOST_TEST_MAIN
    COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
    )
  target_link_libraries( ${test} fhiclcppD ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} )
endforeach( test ${BOOST_TEST_LIST} )

# ----------------------------------------------------------------------
# Build tests with special requirements:

add_executable( PSetSample PSetSample.cc )
target_link_libraries( PSetSample fhiclcppD )

# ----------------------------------------------------------------------
# Run tests:

foreach( test ${TEST_LIST} ${BOOST_TEST_LIST} ${ALSO_RUN} )
  add_test( ${test} ${EXECUTABLE_OUTPUT_PATH}/${test} )
endforeach( test ${TEST_LIST} )

# ----------------------------------------------------------------------
# Copy Configuration and other input files to the correct location
foreach (input ${INPUT_FILES} )
  CONFIGURE_FILE ( ${input} ${CMAKE_CURRENT_BINARY_DIR} COPYONLY )
endforeach (input ${INPUT_FILES} )

# ======================================================================

# ----------------------------------------------------------------------
# Install example:

install( FILES PSetSample.cc Sample.cfg
         DESTINATION ${product}/${version}/example
)
install( FILES PSetSample.cc
         DESTINATION ${product}/${version}/source/test
)

# ======================================================================
