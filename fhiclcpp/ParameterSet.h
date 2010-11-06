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
#include "fhiclcpp/ParameterSetID.h"
#include "fhiclcpp/exception.h"
#include "fhiclcpp/type_traits.h"
#include "cpp0x/cstdint"
#include <complex>
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

  std::string  stringify( boost::any const & ) const;

  std::string    encode( std::string     const & ) const;  // string
  std::string    encode( void *                  ) const;  // nil
  std::string    encode( bool                    ) const;  // bool
  ParameterSetID encode( ParameterSet    const & ) const;  // table
  std::string    encode( std::uintmax_t          ) const;  // unsigned
  template< class T >
  typename tt::enable_if< tt::is_uint<T>::value, std::string >::type
                 encode( T               const & ) const;  // unsigned
  std::string    encode( std::intmax_t           ) const;  // signed
  template< class T >
  typename tt::enable_if< tt::is_int<T>::value, std::string >::type
                 encode( T               const & ) const;  // signed
  std::string    encode( long double             ) const;  // floating-point
  template< class T >
  typename tt::enable_if< tt::is_floating_point<T>::value, std::string >::type
                 encode( T               const & ) const;  // floating-point
  template< class T >
  std::string    encode( std::complex<T> const & ) const;  // complex
  template< class T >
  std::string    encode( std::vector<T>  const & ) const;  // sequence
  template< class T >
  typename tt::disable_if< tt::is_numeric<T>::value, std::string >::type
                 encode( T               const & ) const;  // none of the above

  void  decode( boost::any const &, std::string & ) const;  // string
  void  decode( boost::any const &, void * & ) const;  // nil
  void  decode( boost::any const &, bool & ) const;  // bool
  void  decode( boost::any const &, ParameterSet & ) const;  // table
  void  decode( boost::any const &, std::uintmax_t & ) const;  // unsigned
  template< class T >
  typename tt::enable_if< tt::is_uint<T>::value, void >::type
        decode( boost::any const &, T & ) const;  // unsigned
  void  decode( boost::any const &, std::intmax_t & ) const;  // signed
  template< class T >
  typename tt::enable_if< tt::is_int<T>::value, void >::type
        decode( boost::any const &, T & ) const;  // signed
  void  decode( boost::any const &, long double & ) const;  // floating-point
  template< class T >
  typename tt::enable_if< tt::is_floating_point<T>::value, void >::type
        decode( boost::any const &, T & ) const;  // floating-point
  void  decode( boost::any const &, std::complex<long double> & ) const;  // complex
  template< class T >
  void  decode( boost::any const &, std::complex<T> & ) const;  // complex
  template< class T >
  void  decode( boost::any const &, std::vector<T> & ) const;  // sequence
  template< class T >
  typename tt::disable_if< tt::is_numeric<T>::value, void >::type
        decode( boost::any const &, T & ) const;  // none of the above

};  // ParameterSet

// ======================================================================

template< class T >
  void
  fhicl::ParameterSet::put( std::string const & key, T const & value )
try
{
  insert(key, boost::any( encode(value) ) );
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
  decode(it->second, result);
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
catch( std::exception const & )
{
  return default_value;
}

// ----------------------------------------------------------------------

template< class T >  // unsigned
typename tt::enable_if< tt::is_uint<T>::value, std::string >::type
  fhicl::ParameterSet::encode( T const & value ) const
{
  return encode( uintmax_t(value) );
}

template< class T >  // signed
typename tt::enable_if< tt::is_int<T>::value, std::string >::type
  fhicl::ParameterSet::encode( T const & value ) const
{
  return encode( intmax_t(value) );
}

template< class T >  // floating-point
typename tt::enable_if< tt::is_floating_point<T>::value, std::string >::type
  fhicl::ParameterSet::encode( T const & value ) const
{
  typedef  long double  ldbl;
  return encode( ldbl(value) );
}

template< class T >  // complex
std::string
  fhicl::ParameterSet::encode( std::complex<T> const & value ) const
{
  return '('
        + encode( value.real() )
        + ','
        + encode( value.imag() )
        + ')';
}

template< class T >  // sequence
std::string
  fhicl::ParameterSet::encode( std::vector<T> const & value ) const
{
  if( value.empty() )
    return "[]";

  std::string result = encode(value[0]);
  for( typename std::vector<T>::const_iterator it = value.begin()
                                             , e = value.end(); ++it != e; )
    result += ',' + encode(*it);
  return '[' + result + ']';
}

template< class T >  // none of the above
typename tt::disable_if< tt::is_numeric<T>::value, std::string >::type
  fhicl::ParameterSet::encode( T const & value ) const
{
  return boost::lexical_cast<std::string>(value);
}

// ----------------------------------------------------------------------

template< class T >  // unsigned
typename tt::enable_if< tt::is_uint<T>::value, void >::type
  fhicl::ParameterSet::decode( boost::any const & a, T & result ) const
{
  std::uintmax_t via;
  decode(a, via);
  result = boost::numeric_cast<T>(via);
}

template< class T >  // signed
typename tt::enable_if< tt::is_int<T>::value, void >::type
  fhicl::ParameterSet::decode( boost::any const & a, T & result ) const
{
  std::intmax_t via;
  decode(a, via);
  result = boost::numeric_cast<T>(via);
}

template< class T >  // floating-point
typename tt::enable_if< tt::is_floating_point<T>::value, void >::type
  fhicl::ParameterSet::decode( boost::any const & a, T & result ) const
{
  long double via;
  decode(a, via);
  result = via;  // boost::numeric_cast<T>(via);
}

template< class T >  // complex
void
  fhicl::ParameterSet::decode( boost::any const & a, std::complex<T> & result ) const
{
  std::complex<long double> via;
  decode(a, via);
  result = std::complex<T>( boost::numeric_cast<T>(via.real())
                          , boost::numeric_cast<T>(via.imag())
                          );
}

template< class T >  // sequence
void
  fhicl::ParameterSet::decode( boost::any const & a, std::vector<T> & result ) const
{
  std::string str = boost::any_cast<std::string>(a);
  if(  str.empty()
    || str[0] != '['
    || str.end()[-1] != ']'
    )
    throw fhicl::exception(type_mismatch, "invalid sequence string: ")
      << str;

  str = str.substr(1, str.size()-2);  // strip off brackets
  result.clear();
  if( str.empty() )
    return;

  T via;
  for( size_t comma = str.find(',')
     ; comma != std::string::npos
     ; str.erase(0,comma+1), comma = str.find(',')
     )
  {
    decode( str.substr(0,comma), via );
    result.push_back(via);
  }
  decode( str, via );
  result.push_back(via);
}

template< class T >  // none of the above
typename tt::disable_if< tt::is_numeric<T>::value, void >::type
  fhicl::ParameterSet::decode( boost::any const & a, T & result ) const
{
  result = boost::any_cast<T>(a);
}

// ======================================================================

#endif  // FHICLCPP__PARAMETERSET_H
