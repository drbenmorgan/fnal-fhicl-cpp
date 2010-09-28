#ifndef FHICL_CPP_PARAMETERSETID_H
#define FHICL_CPP_PARAMETERSETID_H

// ======================================================================
//
// ParameterSetID
//
// ======================================================================

#include "boost/array.hpp"
#include "nybbler.h"  // cetlib/

#include <ostream>


namespace fhicl {
  class ParameterSet;
  class ParameterSetID;
  std::ostream &
    operator << ( std::ostream &, ParameterSetID const & );
}


// ======================================================================

class fhicl::ParameterSetID
{
  static  unsigned  const  sha1_size = 40u;  // TODO: get this value from elsewhere

  typedef  ParameterSet    ps_t;
  typedef  ParameterSetID  psid_t;
  typedef  boost::array<unsigned char, sha1_size>  array_t;

public:
  // compiler generates d'tor, copy c'tor, copy assignment

  ParameterSetID( )
  : valid_( false )
  , id_   ( invalid_id_() )
  { }

  ParameterSetID( ps_t const & ps )
  : valid_(false)
  , id_( )
  { reset(ps); }

  bool
    isValid( ) const
  { return valid_; }

  void
    invalidate( )
  { valid_ = false; id_ = invalid_id_(); }

  void
    reset( ps_t const & ps )
  {
    // TODO: id_ = sha1 obtained from ps
    valid_ = true;
  }

  std::string
    to_string() const
  {
    std::string str(id_.begin(), id_.end());
    return cet::nybbler(str).as_hex();
  }

  bool  operator == ( psid_t const & that ) const { return id_ == that.id_; }
  bool  operator != ( psid_t const & that ) const { return id_ != that.id_; }
  bool  operator <  ( psid_t const & that ) const { return id_ <  that.id_; }
  bool  operator >  ( psid_t const & that ) const { return id_ >  that.id_; }
  bool  operator <= ( psid_t const & that ) const { return id_ <= that.id_; }
  bool  operator >= ( psid_t const & that ) const { return id_ >= that.id_; }

private:
  bool     valid_;
  array_t  id_;

  array_t
    invalid_id_() const
  {
    static array_t INVALID_VALUE; //TODO: initialize this value
    return INVALID_VALUE;
  }

};  // ParameterSetID

  inline
  std::ostream &
    fhicl::operator << ( std::ostream         & os
                       , ParameterSetID const & psid
                       )
  { return os << psid.to_string(); }


// ======================================================================

#endif  // FHICL_CPP_PARAMETERSETID_H
