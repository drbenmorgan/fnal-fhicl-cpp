#ifndef fhiclcpp_extended_value_h
#define fhiclcpp_extended_value_h

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
#include <string>

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

  extended_value()
    : in_prolog( false )
    , tag      ( UNKNOWN )
    , value    ()
    , src_info ()
  { }

  extended_value(bool       in_prolog,
                 value_tag  tag,
                 boost::any value,
                 std::string const& src = {})
    : in_prolog( in_prolog )
    , tag      ( tag )
    , value    ( value )
    , src_info ( src )
  { }

  bool
  is_a( value_tag t ) const
  { return t == tag; }

  std::string
  to_string() const;

  void
  set_prolog( bool new_prolog_state );

  void
  set_src_info( std::string const & src )
  { src_info = src; }

  operator atom_t() const
  { return boost::any_cast<atom_t>(value); }

  operator complex_t() const
  { return boost::any_cast<complex_t>(value); }

  operator sequence_t() const
  { return boost::any_cast<sequence_t>(value); }

  operator table_t() const
  { return boost::any_cast<table_t>(value); }

  bool       in_prolog;
  value_tag  tag;
  boost::any value;
  std::string src_info;

};  // extended_value

// ======================================================================

#endif /* fhiclcpp_extended_value_h */

// Local Variables:
// mode: c++
// End:
