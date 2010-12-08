#ifndef FHICLCPP__INTERMEDIATE_TABLE_H
#define FHICLCPP__INTERMEDIATE_TABLE_H

// ======================================================================
//
// intermediate_table
//
// ======================================================================

#include "boost/any.hpp"
#include "fhiclcpp/extended_value.h"
#include <map>
#include <string>
#include <vector>

// ----------------------------------------------------------------------

namespace fhicl {
  class intermediate_table;
}

// ----------------------------------------------------------------------

class fhicl::intermediate_table
{
public:
  typedef  extended_value::atom_t      atom_t;
  typedef  extended_value::complex_t   complex_t;
  typedef  extended_value::sequence_t  sequence_t;
  typedef  extended_value::table_t     table_t;

  typedef  table_t::iterator           iterator;
  typedef  table_t::const_iterator     const_iterator;

  intermediate_table( )
  : ex_val(false, TABLE, table_t() )
  { }

  const_iterator
    begin( ) const
  { return boost::any_cast<table_t const &>(ex_val.value).begin(); }
  const_iterator
    end  ( ) const
  { return boost::any_cast<table_t const &>(ex_val.value).end(); }

  void
    insert( std::string const & name
          , bool                in_prolog
          , value_tag           tag
          , boost::any const &  value
          )
  { insert(name, extended_value(in_prolog, tag, value)); }

  void
    insert( std::string    const & name
          , extended_value const & value
          )
  { this->operator[](name)  = value; }

  extended_value &
    operator [] ( std::string const & name );

  extended_value const &
    find( std::string const & name ) const;

private:
  extended_value  ex_val;

  std::vector<std::string>
    split( std::string const & name ) const;

};  // intermediate_table

// ======================================================================

#endif  // FHICLCPP__INTERMEDIATE_TABLE_H
