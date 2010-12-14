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

set( ALSO_BUILD
)

set( ALSO_RUN
  ParserTest
)

# ----------------------------------------------------------------------
# Build most tests:

foreach( test ${TEST_LIST} ${ALSO_BUILD} )
  add_executable( ${test} ${test}.cc )
  target_link_libraries( ${test} fhiclcppD )
endforeach( test ${TEST_LIST} ${ALSO_BUILD} )

# ----------------------------------------------------------------------
# Build tests with special requirements:

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/Sample.cfg
  ${CMAKE_CURRENT_BINARY_DIR}/Sample.cfg
  COPYONLY
)
add_executable( ParserTest PSetSample.cc )
target_link_libraries( ParserTest fhiclcppD )

# ----------------------------------------------------------------------
# Run tests:

foreach( test ${TEST_LIST} ${ALSO_RUN} )
  add_test( ${test} ${EXECUTABLE_OUTPUT_PATH}/${test} )
endforeach( test ${TEST_LIST} )

# ----------------------------------------------------------------------
# Install example:

install( FILES PSetSample.cc Sample.cfg
         DESTINATION ${product}/${version}/example
)
install( FILES PSetSample.cc
         DESTINATION ${product}/${version}/source/test
)

# ======================================================================
