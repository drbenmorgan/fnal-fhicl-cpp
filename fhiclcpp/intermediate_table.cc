// ======================================================================
//
// intermediate_table
//
// ======================================================================

#include "fhiclcpp/intermediate_table.h"

#include "boost/algorithm/string.hpp"
#include "boost/any.hpp"
#include "cpp0x/utility"
#include "fhiclcpp/exception.h"
#include <cctype>
#include <cstdlib>

using namespace fhicl;

typedef  intermediate_table::atom_t          atom_t;
typedef  intermediate_table::complex_t       complex_t;
typedef  intermediate_table::sequence_t      sequence_t;
typedef  intermediate_table::table_t         table_t;

typedef  intermediate_table::iterator        iterator;
typedef  intermediate_table::const_iterator  const_iterator;

// ----------------------------------------------------------------------

extended_value &
  intermediate_table::operator [] ( std::string const & name )
{
  static extended_value const nil_item( true
                                       , NIL
                                       , std::string("nil")
                                       );
  #if 0
  static extended_value const empty_seq( true
                                       , SEQUENCE
                                       , sequence_t()
                                       );
  static extended_value const empty_tbl( true
                                       , SEQUENCE
                                       , table_t()
                                       );
  #endif

  std::vector<std::string> const & key = split(name);
  extended_value * p = & ex_val;

  for( unsigned k = 0, sz = key.size(); k != sz; ++k ) {
    std::string const & this_key = key[k];
    if( this_key.empty() )
      ;

    else if( std::isdigit(this_key[0]) ) {
      #if 0
      if( p->is_a(NIL) )
        *p = empty_seq;
      #endif
      if( ! p->is_a(SEQUENCE) )
        throw exception(cant_find, name);
      sequence_t & s = boost::any_cast<sequence_t &>(p->value);
      unsigned i = std::atoi(this_key.c_str());
      while( s.size() <= i )
        s.push_back( nil_item );
      p = & s[i];
    }

    else /* this_key[0] is alpha or '_' */ {
      #if 0
      if( p->is_a(NIL) )
        *p = empty_tbl;
      #endif
      if( ! p->is_a(TABLE) )
        throw exception(cant_find, name);
      table_t & t = boost::any_cast<table_t &>(p->value);
      iterator it = t.find(this_key);
      if( it == t.end() ) {
        t.insert( std::make_pair(this_key, nil_item) );
        it = t.find(this_key);
      }
      p = & it->second;
    }
  }  // for

  return *p;

}  // operator [] ()

// ----------------------------------------------------------------------

extended_value const &
  intermediate_table::find( std::string const & name ) const
{
  std::vector<std::string> const & key = split(name);
  extended_value const * p = & ex_val;

  for( unsigned k = 0, sz = key.size(); k != sz; ++k ) {
    std::string const & this_key = key[k];
    if( this_key.empty() )
      ;

    else if( std::isdigit(this_key[0]) ) {
      if( p->tag != SEQUENCE )
        throw exception(cant_find, name)
          << "-- not a sequence (at part \"" << this_key << "\")";
      sequence_t const & s = boost::any_cast<sequence_t const &>(p->value);
      unsigned i = std::atoi(this_key.c_str());
      if( s.size() <= i )
        throw exception(cant_find, name)
          << "(at part \"" << this_key << "\")";
      p = & s[i];
    }

    else /* this_key[0] is alpha or '_' */ {
      if( p->tag != TABLE )
        throw exception(cant_find, name)
          << "-- not a table (at part \"" << this_key << "\")";
      table_t const & t = boost::any_cast<table_t const &>(p->value);
      const_iterator it = t.find(this_key);
      if( it == t.end() )
        throw exception(cant_find, name)
          << "(at part \"" << this_key << "\")";
      p = & it->second;
    }
  }  // for

  return *p;

}  // find()

// ----------------------------------------------------------------------

std::vector<std::string>
  intermediate_table::split( std::string const & name ) const
{
  std::vector<std::string> result;
  boost::algorithm::split( result
                         , name
                         , boost::algorithm::is_any_of(".[]")
                         );
  return result;
}

// ======================================================================
