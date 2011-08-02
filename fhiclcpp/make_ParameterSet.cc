// ======================================================================
//
// make_ParameterSet - intermediate_table -> ParameterSet
//
// ======================================================================

#include "fhiclcpp/make_ParameterSet.h"

#include "boost/any.hpp"
#include "cetlib/includer.h"
#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/ParameterSetRegistry.h"
#include "fhiclcpp/exception.h"
#include "fhiclcpp/extended_value.h"
#include "fhiclcpp/intermediate_table.h"
#include "fhiclcpp/parse.h"

using namespace fhicl;

typedef  intermediate_table::atom_t      atom_t;
typedef  intermediate_table::complex_t   complex_t;
typedef  intermediate_table::sequence_t  sequence_t;
typedef  intermediate_table::table_t     table_t;

typedef  ParameterSet::ps_atom_t      ps_atom_t;     // same as atom_t!
typedef  ParameterSet::ps_sequence_t  ps_sequence_t;

// ----------------------------------------------------------------------

static  boost::any
  encode( extended_value const & xval )
{
  switch( xval.tag ) {
    case NIL: case BOOL: case NUMBER: case STRING:
      return atom_t(xval);

    case COMPLEX: {
      complex_t const & cmplx = complex_t(xval);
      return  '(' + cmplx.first +  ',' + cmplx.second +  ')';
    }

    case SEQUENCE: {
      ps_sequence_t result;
      sequence_t const & seq = sequence_t(xval);
      for( sequence_t::const_iterator it = seq.begin()
                                    , e  = seq.end(); it != e; ++it )
        result.push_back(boost::any(encode(*it)));
      return result;
    }

    case TABLE: {
      typedef  table_t::const_iterator  const_iterator;
      table_t const & tbl = table_t(xval);
      ParameterSet result;
      for( const_iterator it = tbl.begin()
                        , e  = tbl.end(); it != e; ++it ) {
        if( ! it->second.in_prolog )
          result.insert(it->first, encode(it->second));
      }
      return ParameterSetRegistry::put(result);
    }

    case UNKNOWN: default: {
      throw fhicl::exception(type_mismatch, "unknown extended value");
    }
  }
}  // encode()

// ----------------------------------------------------------------------

void
  fhicl::make_ParameterSet( intermediate_table const & tbl
                          , ParameterSet             & ps
                          )
{
  typedef  intermediate_table::const_iterator  c_iter_t;
  for( c_iter_t it = tbl.begin(), e  = tbl.end(); it != e; ++it ) {
    if( ! it->second.in_prolog )
      ps.insert(it->first, encode(it->second));
  }
}  // make_ParameterSet()

// ----------------------------------------------------------------------

void
  fhicl::make_ParameterSet( extended_value const & xval
                          , ParameterSet         & ps
                          )
{
  if( ! xval.is_a(TABLE) )
    throw fhicl::exception(type_mismatch, "extended value not a table");
  table_t const & tbl = table_t(xval);

  typedef  table_t::const_iterator  c_iter_t;
  for( c_iter_t it = tbl.begin(), e  = tbl.end(); it != e; ++it ) {
    if( ! it->second.in_prolog )
      ps.insert(it->first, encode(it->second));
  }
}  // make_ParameterSet()

// ----------------------------------------------------------------------

void
  fhicl::make_ParameterSet( std::string const & str
                          , ParameterSet      & ps
                          )
{
  intermediate_table tbl;
  parse_document(str, tbl), make_ParameterSet(tbl, ps);
}  // make_ParameterSet()

// ----------------------------------------------------------------------

void
  fhicl::make_ParameterSet( std::string const   & filename
                          , cet::filepath_maker & maker
                          , ParameterSet        & ps
                          )
{
  intermediate_table tbl;
  parse_document(filename, maker, tbl), make_ParameterSet(tbl, ps);
}  // make_ParameterSet()

// ======================================================================
