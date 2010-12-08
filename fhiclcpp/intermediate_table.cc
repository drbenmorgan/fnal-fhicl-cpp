// ======================================================================
//
// intermediate_table
//
// ======================================================================

#include "fhiclcpp/intermediate_table.h"

#include "boost/algorithm/string.hpp"  // is_any_of, split
#include "boost/any.hpp"
#include "fhiclcpp/exception.h"
#include <cctype>   // isdigit
#include <cstdlib>  // atoi
#include <utility>  // makepair

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
  static extended_value const dummy ( true
                                    , NIL
                                    , std::string("nil")
                                    );

  std::vector<std::string> const & key = split(name);
  extended_value * p = & ex_val;

  for( unsigned k = 0, sz = key.size(); k != sz; ++k ) {
    std::string const & this_key = key[k];
    if( this_key.empty() )
      ;

    else if( std::isdigit(this_key[0]) ) {
      if( p->tag != SEQUENCE )
        throw exception(cant_find, name);
      sequence_t & s = boost::any_cast<sequence_t &>(p->value);
      unsigned i = std::atoi(this_key.c_str());
      while( s.size() <= i )
        s.push_back( dummy );
      p = & s[i];
    }

    else /* this_key[0] is alpha or '_' */ {
      if( p->tag != TABLE )
        throw exception(cant_find, name);
      table_t & t = boost::any_cast<table_t &>(p->value);
      iterator it = t.find(this_key);
      if( it == t.end() ) {
        t.insert( std::make_pair(this_key, dummy) );
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
        throw exception(cant_find, name);
      sequence_t const & s = boost::any_cast<sequence_t const &>(p->value);
      unsigned i = std::atoi(this_key.c_str());
      if( s.size() <= i )
        throw exception(cant_find, name);
      p = & s[i];
    }

    else /* this_key[0] is alpha or '_' */ {
      if( p->tag != TABLE )
        throw exception(cant_find, name);
      table_t const & t = boost::any_cast<table_t const &>(p->value);
      const_iterator it = t.find(this_key);
      if( it == t.end() )
        throw exception(cant_find, name);
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
