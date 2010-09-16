#ifndef FHICL_CPP_PARAMETERSETID_H
#define FHICL_CPP_PARAMETERSETID_H

// ======================================================================
//
// ParameterSetID
//
// ======================================================================


#include "boost/array.hpp"


namespace fhicl {
  typedef  boost::array<unsigned char, 40>  ParameterSetID;
}

#endif  // FHICL_CPP_PARAMETERSETID_H
