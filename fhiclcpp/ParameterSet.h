#ifndef FHICLCPP_PARAMETERSET_H
#define FHICLCPP_PARAMETERSET_H

// ======================================================================
//
// ParameterSet
//
// ======================================================================

#include "boost/algorithm/string.hpp"
#include "boost/any.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/numeric/conversion/cast.hpp"
#include "cpp0x/string"
#include "fhiclcpp/ParameterSetID.h"
#include "fhiclcpp/coding.h"
#include "fhiclcpp/exception.h"
#include "fhiclcpp/fwd.h"
#include <cctype>
#include <map>
#include <vector>

// ----------------------------------------------------------------------

class fhicl::ParameterSet
{
public:
  typedef  fhicl::detail::ps_atom_t           ps_atom_t;
  typedef  fhicl::detail::ps_sequence_t       ps_sequence_t;

  // compiler generates default c'tor, d'tor, copy c'tor, copy assignment

  // observers:
  bool                      is_empty          ( ) const;
  ParameterSetID            id                ( ) const;
  std::string               to_string         ( ) const;
  std::string               to_indented_string( unsigned initial_indent_level = 0 ) const;
  std::vector<std::string>  get_keys          ( ) const;
  std::vector<std::string>  get_pset_keys     ( ) const;

  // retrievers:
  template< class T >
    bool get_if_present( std::string const & key, T & value ) const;
  template< class T, class Via >
    bool get_if_present( std::string const & key, T & value
                       , T convert(Via const &) ) const;
  template< class T >
    T  get( std::string const & key ) const;
  template< class T, class Via >
    T  get( std::string const & key
          , T convert(Via const &) ) const;
  template< class T >
    T  get( std::string const & key, T const & default_value ) const;
  template< class T, class Via >
    T  get( std::string const & key, T const & default_value
          , T convert(Via const &) ) const;

  // inserters:
  void  insert( std::string const & key, boost::any const & value );
  void  put( std::string const & key );  // implicit nil value
  template< class T >
    void  put( std::string const & key, T const & value );

  // deleters:
  bool  erase( std::string const & key );

  // comparators:
  bool  operator == ( ParameterSet const & other ) const;
  bool  operator != ( ParameterSet const & other ) const;

private:
  typedef  std::map<std::string,boost::any>  map_t;
  typedef  map_t::const_iterator             map_iter_t;

  map_t                    mapping_;
  mutable  ParameterSetID  id_;

  std::string  stringify( boost::any const & ) const;

  template< class T >
    bool get_one( std::string const & key, T & value ) const;

  class Prettifier;

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
{
  typedef  std::vector<std::string>  Keys;
  Keys  keys;
  boost::algorithm::split( keys
                         , key
                         , boost::algorithm::is_any_of(".")
                         );
  Keys::iterator  b = keys.begin()
               ,  e = keys.end()
               ,  r = b;
  for( ; b != e; ++b )
    if( ! b->empty() )
      *r++ = *b;
  keys.erase(r, e);

  if( keys.empty() )
    throw fhicl::exception(cant_find, "vacuous key");

  ParameterSet const * p = this;
  ParameterSet pset;
  for( b = keys.begin(), e = keys.end() - 1; b != e; ++b ) {
    std::string const & this_key = *b;
    if( std::isdigit(this_key[0]) )
      throw fhicl::exception(unimplemented, "lookup in a sequence");
    if( ! p->get_one(this_key, pset) )
      return false;
    p = & pset;
  }

  return p->get_one(keys.back(), value);
}  // get_if_present<>()

template< class T, class Via >
  bool
  fhicl::ParameterSet::get_if_present( std::string const & key
                                     , T                 & result
                                     , T convert(Via const &)
                                     ) const
{
  Via go_between;
  if( ! get_if_present(key, go_between) )
    return false;
  result = convert(go_between);
  return true;
}  // get_if_present<>()

template< class T >
  T
  fhicl::ParameterSet::get( std::string const & key ) const
{
  T  result;
  return get_if_present(key, result) ? result
                                     : throw fhicl::exception(cant_find, key);
}

template< class T, class Via >
  T
  fhicl::ParameterSet::get( std::string const & key
                          , T convert(Via const &)
                          ) const
{
  T  result;
  return get_if_present(key, result, convert)
       ? result
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

template< class T, class Via >
  T
  fhicl::ParameterSet::get( std::string const & key
                          , T           const & default_value
                          , T convert(Via const &)
                          ) const {
  T  result;
  return get_if_present(key, result, convert)
       ? result
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

// ----------------------------------------------------------------------

template< class T >
  bool
  fhicl::ParameterSet::get_one( std::string const & key
                              , T                 & value
                              ) const
try
{
  map_iter_t it = mapping_.find(key);
  if( it == mapping_.end() )
    return false;

  detail::decode(it->second, value);
  return true;
}
catch( fhicl::exception const & e )
{
  throw fhicl::exception(type_mismatch, key, e);
}
catch( std::exception const & e )
{
  throw fhicl::exception(type_mismatch, key + "\n" + e.what());
}

// ======================================================================

#endif
