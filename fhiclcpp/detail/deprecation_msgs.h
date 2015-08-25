#ifndef fhiclcpp_detail_deprecation_msgs_h
#define fhiclcpp_detail_deprecation_msgs_h

#define GET_KEYS_MSG                                                    \
  "\n\n"                                                                \
  "fhiclcpp error: The 'fhicl::ParameterSet::get_keys()' function is deprecated.\n" \
  "                Please use 'fhicl::ParameterSet::get_names()'.\n\n"

#define GET_PSET_KEYS_MSG                                               \
  "\n\n"                                                                \
  "fhiclcpp error: The 'fhicl::ParameterSet::get_pset_keys()' function is deprecated.\n" \
  "                Please use 'fhicl::ParameterSet::get_pset_names()'.\n\n"

#endif
