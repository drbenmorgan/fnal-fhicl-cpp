#ifndef PARAMETERSETID_HEADER_
#define PARAMETERSETID_HEADER_

#include "boost/array.hpp"

namespace fhicl {
  typedef  boost::array<unsigned char, 40>  ParameterSetID;
}

#endif  // PARAMETERSETID_HEADER_
