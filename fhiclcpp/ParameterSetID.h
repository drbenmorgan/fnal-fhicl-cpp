#ifndef FHICLCPP_PARAMETERSETID_H
#define FHICLCPP_PARAMETERSETID_H

// ======================================================================
//
// ParameterSetID
//
// ======================================================================

#include "cetlib/sha1.h"
#include <string>
#include "fhiclcpp/fwd.h"
#include <ostream>

namespace fhicl {
  std::ostream &
    operator << ( std::ostream &, ParameterSetID const & );
}

// ----------------------------------------------------------------------

class fhicl::ParameterSetID
{
public:
  // compiler generates d'tor, copy c'tor, copy assignment

  // c'tor's:
  ParameterSetID( );
  ParameterSetID( ParameterSet const & );

  // observers:
  bool         is_valid ( ) const;
  std::string  to_string( ) const;

  // mutators:
  void  invalidate( );
  void  reset( ParameterSet const & );
  void  swap( ParameterSetID & );

  // comparators:
  bool  operator == ( ParameterSetID const & ) const;
  bool  operator != ( ParameterSetID const & ) const;
  bool  operator <  ( ParameterSetID const & ) const;
  bool  operator >  ( ParameterSetID const & ) const;
  bool  operator <= ( ParameterSetID const & ) const;
  bool  operator >= ( ParameterSetID const & ) const;

private:
  bool                 valid_;
  cet::sha1::digest_t  id_;

};  // ParameterSetID

// ======================================================================

#endif
