// ======================================================================
//
// extended_value
//
// ======================================================================

#include "fhiclcpp/extended_value.h"

// ----------------------------------------------------------------------

std::string
  fhicl::extended_value::to_string( ) const
{
  using std::string;
  using boost::any_cast;

  if( in_prolog )
    return "";

  switch( tag )  {

    case NIL: case BOOL: case NUMBER: case STRING: {
      return any_cast<atom_t>(value);
    }

    case COMPLEX: {
      complex_t c = any_cast<complex_t>(value);
      return '(' + c.first + ',' + c.second + ')';
    }

    case SEQUENCE: {
      sequence_t q = any_cast<sequence_t>(value);
      string s("[");
      string sep;
      for( sequence_t::const_iterator b  = q.begin()
                                    , e  = q.end()
                                    , it = b; it != e; ++it ) {
        s.append(sep)
         .append( it->to_string() );
        sep = ",";
      }
      return s + ']';
    }

    case TABLE: {
      table_t t = any_cast<table_t>(value);
      string s("{");
      string sep;
      for( table_t::const_iterator b  = t.begin()
                                 , e  = t.end()
                                 , it = b; it != e; ++it ) {
        s.append(sep)
         .append( it->first + ':' + it->second.to_string() );
        sep = " ";
      }
      return s + '}';
    }

    case UNKNOWN: default: {
      return "";
    }

  };  // switch

}  // to_string()

// ======================================================================
