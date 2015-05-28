# ======================================================================
#
# Testing for fhiclcpp-types: Atom<>, Sequence<>, Table<>, Tuple<>
#
# ======================================================================

cet_test(keymap_t                USE_BOOST_UNIT)
cet_test(keymap_defaults_t       USE_BOOST_UNIT)
cet_test(keymap_table_t          USE_BOOST_UNIT)
cet_test(return_types_t          USE_BOOST_UNIT)
cet_test(return_types_table_t    USE_BOOST_UNIT)
cet_test(return_value_defaults_t USE_BOOST_UNIT)

cet_test(print_reference_t
  REF "${CMAKE_CURRENT_SOURCE_DIR}/print_reference_t-ref.txt"
  )

cet_test(print_reference_defaults_t
  REF "${CMAKE_CURRENT_SOURCE_DIR}/print_reference_defaults_t-ref.txt"
  )

cet_test(validate_ParameterSet_t
  DATAFILES
  validate_ParameterSet_t.fcl
  REF "${CMAKE_CURRENT_SOURCE_DIR}/validate_ParameterSet_t-ref.txt"
  )

cet_test(return_values_from_fcl_t USE_BOOST_UNIT
  DATAFILES
  return_values_from_fcl_t.fcl
  )

cet_test(return_table_values_from_fcl_t USE_BOOST_UNIT
  DATAFILES
  return_table_values_from_fcl_t.fcl
  )


