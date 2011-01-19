#ifndef FHICLCPP_PARAMETERSET_H
#define FHICLCPP_PARAMETERSET_H

// ======================================================================
//
// ParameterSet
//
// ======================================================================

#include "boost/any.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/numeric/conversion/cast.hpp"
#include "fhiclcpp/ParameterSetID.h"
#include "fhiclcpp/coding.h"
#include "fhiclcpp/exception.h"
#include <map>
#include <string>
#include <vector>

namespace fhicl {
  class ParameterSet;
}

// ----------------------------------------------------------------------

class fhicl::ParameterSet
{
public:
  typedef  fhicl::detail::ps_atom_t           ps_atom_t;
  typedef  fhicl::detail::ps_sequence_t       ps_sequence_t;

  // compiler generates default c'tor, d'tor, copy c'tor, copy assignment

  // observers:
  bool                      is_empty     ( ) const;
  ParameterSetID            id           ( ) const;
  std::string               to_string    ( ) const;
  std::vector<std::string>  get_keys     ( ) const;
  std::vector<std::string>  get_pset_keys( ) const;

  // retrievers:
  template< class T >
    bool get_if_present( std::string const & key, T & value ) const;
  template< class T >
    T  get( std::string const & key ) const;
  template< class T >
    T  get( std::string const & key, T const & default_value ) const;

  // inserters:
  void  insert( std::string const & key, boost::any const & value );
  template< class T >
    void  put( std::string const & key, T const & value );

  // comparators:
  bool  operator == ( ParameterSet const & other ) const;
  bool  operator != ( ParameterSet const & other ) const;

private:
  typedef  std::map<std::string,boost::any>  map_t;
  typedef  map_t::const_iterator             map_iter_t;

  map_t                    mapping_;
  mutable  ParameterSetID  id_;

  std::string  stringify( boost::any const & ) const;

};  // ParameterSet

// ======================================================================

template< class T >
  void
  fhicl::ParameterSet::put( std::string const & key, T const & value )
try
{
  insert(key, boost::any(detail::encode(value)) );
}
catch( boost::bad_lexical_cast const & e )
{
  throw fhicl::exception(cant_insert, key) << e.what();
}
catch( boost::bad_numeric_cast const & e )
{
  throw fhicl::exception(cant_insert, key) << e.what();
}
catch( fhicl::exception const & e )
{
  throw fhicl::exception(cant_insert, key, e);
}
catch( std::exception const & e )
{
  throw fhicl::exception(cant_insert, key) << e.what();
}

// ----------------------------------------------------------------------

template< class T >
  bool
  fhicl::ParameterSet::get_if_present( std::string const & key
                                     , T                 & value
                                     ) const
try
{
  map_iter_t it = mapping_.find(key);
  if( it == mapping_.end() ) {
    return false;
  }
  else {
    detail::decode(it->second, value);
    return true;
  }
}
catch( fhicl::exception const & e )
{
  throw fhicl::exception(type_mismatch, key, e);
}
catch( std::exception const & e )
{
  throw fhicl::exception(type_mismatch, e.what() );
}

template< class T >
  T
  fhicl::ParameterSet::get( std::string const & key ) const
{
  T  result;
  return get_if_present(key, result) ? result
                                     : throw fhicl::exception(cant_find, key);
}

template< class T >
  T
  fhicl::ParameterSet::get( std::string const & key
                          , T           const & default_value
                          ) const {
  T  result;
  return get_if_present(key, result) ? result
                                     : default_value;
}

// ----------------------------------------------------------------------

inline  bool
  fhicl::ParameterSet::operator == ( ParameterSet const & other ) const
{
  return id() == other.id();
}

inline  bool
  fhicl::ParameterSet::operator != ( ParameterSet const & other ) const
{
  return ! operator==(other);
}

// ======================================================================

#endif
