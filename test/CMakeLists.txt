# ======================================================================
#
# Testing
#
# ======================================================================

# ======================================================================
# Part 1: Categorize each source or data file
# ======================================================================

# ----------------------------------------------------------------------
# Identify libraries to be linked:

link_libraries( fhiclcppD )

# ----------------------------------------------------------------------
# Identify each test's build requirements:

set( BUILD              # compile and link with above
  bool_test
  complex_test
  fhicl-config_t
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

set( BUILD_BOOST        # compile and link with BOOST_TEST library, too
  ParameterSet_t
)

set( HANDBUILT          # use target-specific build rules, found below
  PSetSample
)

set( PREBUILT           # no build required
  fhicl-expand_test.sh
  fhicl-expand_test_path.sh
)

# ----------------------------------------------------------------------
# Identify data files needed to run the above tests:

set( DATAFILE           # install only
  Sample.cfg
)

# ----------------------------------------------------------------------
# Identify tests not to be run automatically:

set( HANDRUN            # use target-specific run rules, found below
  fhicl-config_t
)

# ======================================================================
# Part 2: Provide build instructions
# ======================================================================

# ----------------------------------------------------------------------
# BUILD:

foreach( test ${BUILD} )
  add_executable( ${test} ${test}.cc )
endforeach( test )

# ----------------------------------------------------------------------
# BUILD_BOOST:

foreach( test ${BUILD_BOOST} )
  add_executable( ${test} ${test}.cc )
  set_target_properties ( ${test} PROPERTIES
    COMPILE_DEFINITIONS BOOST_TEST_MAIN
    COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
  )
  target_link_libraries( ${test} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} )
endforeach( test )

# ----------------------------------------------------------------------
# HANDBUILT:

# add_executable( ... )
# target_link_libraries( ... )
add_executable( PSetSample PSetSample.cc )
target_link_libraries( PSetSample fhiclcppD )

# ----------------------------------------------------------------------
# PREBUILT:

foreach( test ${PREBUILT} )
  add_custom_target( ${test}_copy ALL
    COMMAND ${CMAKE_COMMAND} -E
      copy ${CMAKE_CURRENT_SOURCE_DIR}/${test}
           ${EXECUTABLE_OUTPUT_PATH}/
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${test}
  )
endforeach( test )

# ----------------------------------------------------------------------
# DATAFILE:

foreach( input ${DATAFILE} )
  add_custom_target( ${input}_copy ALL
    COMMAND ${CMAKE_COMMAND} -E
      copy ${CMAKE_CURRENT_SOURCE_DIR}/${input}
           ${CMAKE_CURRENT_BINARY_DIR}/
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${input}
  )
endforeach( input )

# ======================================================================
# Part 3: Run tests
# ======================================================================

# ----------------------------------------------------------------------
# Run each test not excepted in Part 1:

set( TESTS_TO_RUN ${BUILD} ${BUILD_BOOST} ${HANDBUILT} ${PREBUILT} )
list( REMOVE_ITEM TESTS_TO_RUN ${HANDRUN} )
foreach( test ${TESTS_TO_RUN} )
  add_test( ${test} ${EXECUTABLE_OUTPUT_PATH}/${test} )
endforeach( test )

# ----------------------------------------------------------------------
# HANDRUN:

# ======================================================================
# Part 4: Other custom commands
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
