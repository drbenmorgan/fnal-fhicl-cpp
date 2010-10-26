#ifndef FHICLCPP__PARAMETERSET_H
#define FHICLCPP__PARAMETERSET_H

// ======================================================================
//
// ParameterSet
//
// ======================================================================

#include "boost/any.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/numeric/conversion/cast.hpp"
#include "fhiclcpp/exception.h"
#include "fhiclcpp/ParameterSetID.h"
#include "fhiclcpp/type_traits.h"
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
  // compiler generates default c'tor, d'tor, copy c'tor, copy assignment

  // observers:
  bool                      is_empty     ( ) const;
  ParameterSetID            id           ( ) const;
  std::string               to_string    ( ) const;
  std::string               hash_string  ( ) const;
  std::vector<std::string>  get_keys     ( ) const;
  std::vector<std::string>  get_pset_keys( ) const;

  // retrievers:
  template< class T >
    T  get( std::string const & key ) const;
  template< class T >
    T  get( std::string const & key, T const & default_value ) const;

  // inserters:
  void  insert( std::string const & key, boost::any const & value );
  template< class T >
    void  put( std::string const & key, T const & value );

private:
  typedef  std::map<std::string,boost::any>  map_t;
  typedef  map_t::const_iterator             map_iter_t;

  map_t                    mapping_;
  mutable  ParameterSetID  id_;

  static  void  cpp_to_atom_( std::string & );
  static  void  atom_to_cpp_( std::string & );

  boost::any  encode_( void *               ) const;  // nil
  boost::any  encode_( bool                 ) const;  // bool
  boost::any  encode_( ParameterSet const & ) const;  // table
  template< class T >
    boost::any  encode_( std::complex<T> const & ) const;  // complex
  template< class T >
    boost::any  encode_( std::vector<T>  const & ) const;  // sequence
  template< class T >
    boost::any  encode_( T               const & ) const;  // atom

  void  decode_( boost::any const &, void *       & ) const;  // nil
  void  decode_( boost::any const &, bool         & ) const;  // bool
  void  decode_( boost::any const &, ParameterSet & ) const;  // table
  template< class T >
    void  decode_( boost::any const &, std::complex<T> & ) const;  // complex
  template< class T >
    void  decode_( boost::any const &, std::vector<T>  & ) const;  // sequence
  template< class T >
    typename tt::enable_if< tt::is_numeric<T>::value, void >::type
    decode_( boost::any const &, T & ) const;  // numeric atom
  template< class T >
    typename tt::disable_if< tt::is_numeric<T>::value, void >::type
    decode_( boost::any const &, T & ) const;  // nonnumeric atom

};  // ParameterSet

// ======================================================================

template< class T >
  void
  fhicl::ParameterSet::put( std::string const & key, T const & value )
try
{
  insert(key, encode_(value) );
}
catch( boost::bad_lexical_cast const & )
{
  throw fhicl::exception(cant_insert, key);
}

// ----------------------------------------------------------------------

template< class T >
  T
  fhicl::ParameterSet::get( std::string const & key ) const
{
  map_iter_t it = mapping_.find(key);
  if( it == mapping_.end() )
    throw fhicl::exception(cant_find, key);

  T  result;
  decode_(it->second, result);
  return result;
}

template< class T >
  T
  fhicl::ParameterSet::get( std::string const & key
                          , T           const & default_value
                          ) const
try
{
  return get<T>(key);
}
catch( std::exception const & ) { return default_value; }

// ----------------------------------------------------------------------

template< class T >
boost::any
  fhicl::ParameterSet::encode_( T const & value ) const
{
  std::string str = boost::lexical_cast<std::string>(value);
  ParameterSet::cpp_to_atom_(str);
  return str;
}

template< class T >
boost::any
  fhicl::ParameterSet::encode_( std::complex<T> const & value ) const
{
  std::vector<T>  pair;
  pair.push_back(value.real()), pair.push_back(value.imag());
  return encode_(pair);
}

template< class T >
boost::any
  fhicl::ParameterSet::encode_( std::vector<T> const & value ) const
{
  std::vector<boost::any>  result;
  for( typename std::vector<T>::const_iterator it = value.begin()
                                             , e = value.end()
     ; it != e; ++it )
    result.push_back( encode_(*it) );
  return result;
}

// ----------------------------------------------------------------------

template< class T >
typename tt::enable_if< tt::is_numeric<T>::value, void >::type
  fhicl::ParameterSet::decode_( boost::any const & any_val
                              , T & result
                              ) const
try
{
  std::string str = boost::any_cast<std::string>(any_val);
  atom_to_cpp_(str);

  typedef  typename tt::largest_type<T>::type  via_t;
  via_t v = boost::lexical_cast<via_t>(str);

  result = boost::numeric_cast<T>(v);
}
catch( boost::bad_any_cast const & )
{
  throw fhicl::exception(type_mismatch, "Can't extract string");
}
catch( boost::bad_lexical_cast const & )
{
  throw fhicl::exception(type_mismatch, "Can't extract number");
}
catch( boost::bad_numeric_cast const & )
{
  throw fhicl::exception(number_is_too_large, "Value exceeds type's capacity");
}

template< class T >
typename tt::disable_if< tt::is_numeric<T>::value, void >::type
  fhicl::ParameterSet::decode_( boost::any const & any_val
                              , T & result
                              ) const
try
{
  result = boost::any_cast<T>(any_val);
}
catch( boost::bad_any_cast const & )
{
  throw fhicl::exception(type_mismatch, "Can't extract non-numeric value");
}

template< class T >
  void
    fhicl::ParameterSet::decode_( boost::any const & any_val
                                , std::complex<T>  & result
                                ) const
{
  std::vector<T>  v;
  decode_(any_val, v);
  if( v.size() != 2 )
    throw fhicl::exception(type_mismatch, "Not a complex number");
  result = std::complex<T>(v[0], v[1]);
}

template< class T >
  void
    fhicl::ParameterSet::decode_( boost::any const & any_val
                                , std::vector<T>   & result
                                ) const
{
  typedef  std::vector<boost::any>  vec_t;
  vec_t va = boost::any_cast<vec_t>(any_val);
  result.clear();
  for( vec_t::const_iterator it = va.begin()
                           , e  = va.end(); it != e; ++it ) {
    T part;
    decode_(*it, part );
    result.push_back( part );
  }
}

// ======================================================================

#endif  // FHICLCPP__PARAMETERSET_H
