// ======================================================================
//
// extended_value
//
// ======================================================================

#include "fhiclcpp/extended_value.h"

using std::string;
using boost::any_cast;

// ----------------------------------------------------------------------

std::string
  fhicl::extended_value::to_string( ) const
{
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

    case TABLEID: {
      return string("@id::") + any_cast<atom_t>(value);
    }

    case UNKNOWN: default: {
      return "";
    }

  };  // switch

}  // to_string()

// ----------------------------------------------------------------------

void
  fhicl::extended_value::set_prolog( bool new_prolog_state )
{
  in_prolog = new_prolog_state;

  switch( tag )  {

  case NIL: case BOOL: case NUMBER: case STRING: case COMPLEX: case TABLEID: {
      break;
    }

    case SEQUENCE: {
      sequence_t & q = any_cast<sequence_t &>(value);
      for( sequence_t::iterator it = q.begin()
                              , e  = q.end(); it != e; ++it ) {
        it->set_prolog(new_prolog_state);
      }
      break;
    }

    case TABLE: {
      table_t & t = any_cast<table_t &>(value);
      for( table_t::iterator it = t.begin()
                           , e  = t.end(); it != e; ++it ) {
        it->second.set_prolog(new_prolog_state);
      }
      break;
    }

    case UNKNOWN: default: {
      break;
    }

  };  // switch

}  // set_prolog()

// ======================================================================
