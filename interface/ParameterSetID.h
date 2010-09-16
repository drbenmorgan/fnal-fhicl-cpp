#ifndef FHICL_CPP_PARAMETERSETID_H
#define FHICL_CPP_PARAMETERSETID_H

// ======================================================================
//
// ParameterSetID
//
// ======================================================================


#include "boost/array.hpp"


namespace fhicl {

  class ParameterSetID {

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

}

#endif  // FHICL_CPP_PARAMETERSETID_H
