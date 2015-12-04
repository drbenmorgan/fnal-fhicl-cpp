#ifndef test_fhiclcpp_types_TestMacros_h
#define test_fhiclcpp_types_TestMacros_h

#include "boost/test/test_tools.hpp"

#define FHICLCPP_CHECK_EQUAL_COLLECTIONS(test, ref)             \
  BOOST_CHECK_EQUAL_COLLECTIONS(test.begin(), test.end(),       \
                                ref.begin(), ref.end())

#endif

// Local variables
// mode: c++
// End:
