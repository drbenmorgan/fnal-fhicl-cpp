#ifndef FHICLCPP__PARAMETERSETID_H
#define FHICLCPP__PARAMETERSETID_H

// ======================================================================
//
// ParameterSetID
//
// ======================================================================


#include "cetlib/sha1.h"
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
  typedef  ParameterSet         ps_t;
  typedef  ParameterSetID       psid_t;
  typedef  cet::sha1::digest_t  array_t;

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

  void
    swap( ParameterSetID & other )
  { id_.swap(other.id_); std::swap(valid_, other.valid_); }

  bool
    isValid( ) const
  { return valid_; }

  void
    invalidate( )
  { valid_ = false; id_ = invalid_id_(); }

  void
    reset( ps_t const & ps );

  std::string
    to_string() const;

  bool  operator == ( psid_t const & that ) const { return id_ == that.id_; }
  bool  operator != ( psid_t const & that ) const { return id_ != that.id_; }
  bool  operator <  ( psid_t const & that ) const { return id_ <  that.id_; }
  bool  operator >  ( psid_t const & that ) const { return id_ >  that.id_; }
  bool  operator <= ( psid_t const & that ) const { return id_ <= that.id_; }
  bool  operator >= ( psid_t const & that ) const { return id_ >= that.id_; }

private:
  bool     valid_;
  array_t  id_;

  array_t const &
    invalid_id_() const
  {
    static array_t INVALID_VALUE = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
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


#endif  // FHICLCPP__PARAMETERSETID_H
