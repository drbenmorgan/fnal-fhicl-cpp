#ifndef FHICLCPP_INTERMEDIATE_TABLE_H
#define FHICLCPP_INTERMEDIATE_TABLE_H

// ======================================================================
//
// intermediate_table
//
// ======================================================================

#include "boost/any.hpp"
#include "cpp0x/string"
#include "fhiclcpp/extended_value.h"
#include "fhiclcpp/fwd.h"
#include <map>
#include <vector>

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

  intermediate_table( );

  const_iterator  begin( ) const;
  const_iterator  end  ( ) const;

  bool  empty( ) const;

  void  insert( std::string const & name
              , bool                in_prolog
              , value_tag           tag
              , boost::any const &  value
              );
  void  insert( std::string    const & name
              , extended_value const & value
              );

  extended_value &  operator [] ( std::string const & name );
  extended_value const &  find( std::string const & name ) const;

  bool  exists( std::string const & name ) const;

private:
  extended_value  ex_val;

  std::vector<std::string>  split( std::string const & name ) const;

};  // intermediate_table

// ======================================================================

#endif
