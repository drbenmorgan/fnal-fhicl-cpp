#ifndef fhiclcpp_detail_encode_extended_value_h
#define fhiclcpp_detail_encode_extended_value_h

// ======================================================================
//
// encode overload for extended_value, receiving std::string annotation
//
// ======================================================================

#include "boost/any.hpp"
#include <string>

namespace fhicl {

  class extended_value;

  namespace detail {

    boost::any
      encode( extended_value const & xval );

  }
}
#endif /* fhiclcpp_detail_encode_extended_value_h */
