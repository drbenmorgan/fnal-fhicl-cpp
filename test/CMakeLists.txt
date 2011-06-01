# ======================================================================
#
# Testing
#
# ======================================================================

include(CetTest)

# Identify libraries to be linked:
link_libraries( fhiclcppD )

cet_test(PSetSample INSTALL_EXAMPLE INSTALL_SOURCE
  DATAFILES Sample.cfg
)
cet_test(PSetTest)
cet_test(ParameterSet_t USE_BOOST_UNIT
  DATAFILES Sample.cfg
)
cet_test(failer DATAFILES test_config_fail.fcl)
cet_test(fhicl-config_t NO_AUTO)
cet_test(fhicl-expand_test.sh PREBUILT)
cet_test(fhicl-expand_test_path.sh PREBUILT)
cet_test(issue_0923_t.sh PREBUILT
  DATAFILES
  issue_0923a.fcl
  issue_0923b.fcl
  issue_0923_ref.txt
  DEPENDENCIES fhicl-config_t
)
cet_test(parse_document_test USE_BOOST_UNIT)
cet_test(parse_value_test)
cet_test(to_indented_string_test USE_BOOST_UNIT)
cet_test(to_string_test
  DATAFILES Sample.cfg
)
cet_test(values_test USE_BOOST_UNIT)
