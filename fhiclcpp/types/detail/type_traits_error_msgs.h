#ifndef fhiclcpp_types_detail_type_traits_error_msgs_h
#define fhiclcpp_types_detail_type_traits_error_msgs_h

#define NO_CONTAINERS_IN_ATOM                                           \
  "\n\n"                                                                \
  "fhiclcpp error: Cannot create an 'Atom' with any of the following types\n\n" \
  "                .. std::array\n"                                     \
  "                .. std::pair\n"                                      \
  "                .. std::vector\n"                                    \
  "                .. std::tuple\n\n"                                   \
  "                Please use one of the 'Sequence' options:\n\n"       \
  "                .. Sequence<int,4u>           ===> std::array <int,4u>\n" \
  "                .. Sequence<int>              ===> std::vector<int>\n" \
  "                .. Tuple<int,double>          ===> std::tuple <int,double>\n" \
  "                .. Tuple<int,double,bool>     ===> std::tuple <int,double,bool>\n" \
  "                .. Sequence<Sequence<int>,4u> ===> std::array <std::vector<int>,4u>\n" \
  "                .. etc.\n"


#define FHICL_TYPES                                                     \
  "                .. Atom<T>\n"                                        \
  "                .. Sequence<T>\n"                                    \
  "                .. Tuple<T...>\n"                                    \
  "                .. Table<T>\n"

#define NO_NESTED_FHICL_TYPES_IN_ATOM                                   \
  "\n\n"                                                                \
  "fhiclcpp error: Cannot create a nested 'Atom'--i.e. cannot create an\n" \
  "                'Atom' with the following types:\n\n"                \
  FHICL_TYPES

#define NO_NESTED_FHICL_TYPES_IN_TABLE                                  \
  "\n\n"                                                                \
  "fhiclcpp error: Cannot create a nested 'Table'--i.e. cannot create an\n" \
  "                'Table' with the following types:\n\n"               \
  FHICL_TYPES

#define NO_DEFAULTS_FOR_TABLE                                           \
  "\n\n"                                                                \
  "fhiclcpp error: Cannot specify a default for type 'Table'\n"         \
  "                Please remove 'Table<T>{}' from default list\n"

#endif

