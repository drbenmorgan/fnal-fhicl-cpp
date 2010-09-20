#ifndef FHICL_CPP_PARAMETERSETID_H
#define FHICL_CPP_PARAMETERSETID_H

// ======================================================================
//
// ParameterSetID
//
// ======================================================================

#include "boost/array.hpp"

#include <ostream>


namespace fhicl {
  class ParameterSetID;
  std::ostream &
    operator << ( std::ostream &, ParameterSetID const & );
}


// ======================================================================

class fhicl::ParameterSetID
{
public:
  ParameterSetID() : valid_(false), id_() { }

  // Is an valid ID
  bool isValid() const { return valid_; }

  // A dummy set id function
  void setID() {}

private:
  boost::array<unsigned char, 40> id_;
  bool valid_;
};


inline
std::ostream &
  fhicl::operator << ( std::ostream         & os
                     , ParameterSetID const & psid
                     )
{
  // TODO: insert output logic
  return os;
}


// ======================================================================

#endif  // FHICL_CPP_PARAMETERSETID_H
