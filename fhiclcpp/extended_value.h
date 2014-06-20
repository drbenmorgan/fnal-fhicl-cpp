#ifndef FHICLCPP_EXTENDED_VALUE_H
#define FHICLCPP_EXTENDED_VALUE_H

// ======================================================================
//
// extended_value
//
// ======================================================================

#include "boost/any.hpp"
#include "cpp0x/string"
#include "fhiclcpp/fwd.h"
#include <map>
#include <vector>

// ----------------------------------------------------------------------

namespace fhicl {
  enum value_tag { UNKNOWN
                   , NIL, BOOL, NUMBER, COMPLEX, STRING, SEQUENCE, TABLE, TABLEID
                 };
}

// ----------------------------------------------------------------------

class fhicl::extended_value
{
public:
  typedef  std::string                            atom_t;
  typedef  std::pair<std::string, std::string>    complex_t;
  typedef  std::vector<extended_value>            sequence_t;
  typedef  std::map<std::string, extended_value>  table_t;

  extended_value( )
  : in_prolog( false )
  , tag      ( UNKNOWN )
  , value    ( )
  { }

  extended_value( bool       in_prolog
                , value_tag  tag
                , boost::any value
                )
  : in_prolog( in_prolog )
  , tag      ( tag )
  , value    ( value )
  { }

  bool
    is_a( value_tag t ) const
  { return t == tag; }

  std::string
    to_string( ) const;

  void
    set_prolog( bool new_prolog_state );

  operator atom_t( ) const
  { return boost::any_cast<atom_t>(value); }

  operator complex_t( ) const
  { return boost::any_cast<complex_t>(value); }

  operator sequence_t( ) const
  { return boost::any_cast<sequence_t>(value); }

  operator table_t( ) const
  { return boost::any_cast<table_t>(value); }

  bool       in_prolog;
  value_tag  tag;
  boost::any value;

};  // extended_value

// ======================================================================

#endif
