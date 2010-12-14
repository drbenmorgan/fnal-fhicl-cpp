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
#include "cpp0x/cstdint"
#include "fhiclcpp/ParameterSetID.h"
#include "fhiclcpp/exception.h"
#include "fhiclcpp/extended_value.h"
#include "fhiclcpp/parse.h"
#include "fhiclcpp/type_traits.h"
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
  typedef  std::string                        ps_atom_t;
  typedef  std::vector<boost::any>            ps_sequence_t;
  //typedef  std::map<std::string, boost::any>  ps_table_t;

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

  // comparators:
  bool  operator == ( ParameterSet const & other ) const;
  bool  operator != ( ParameterSet const & other ) const;

private:
  typedef  std::map<std::string,boost::any>  map_t;
  typedef  map_t::const_iterator             map_iter_t;

  map_t                    mapping_;
  mutable  ParameterSetID  id_;

  std::string  stringify( boost::any const & ) const;

  ps_atom_t      encode( std::string     const & ) const;  // string (w/ quotes)
  ps_atom_t      encode( void *                  ) const;  // nil
  ps_atom_t      encode( bool                    ) const;  // bool
  ParameterSetID encode( ParameterSet    const & ) const;  // table
  ps_atom_t      encode( std::uintmax_t          ) const;  // unsigned
  template< class T >
  typename tt::enable_if< tt::is_uint<T>::value, ps_atom_t >::type
                 encode( T               const & ) const;  // unsigned
  ps_atom_t      encode( std::intmax_t           ) const;  // signed
  template< class T >
  typename tt::enable_if< tt::is_int<T>::value, ps_atom_t >::type
                 encode( T               const & ) const;  // signed
  ps_atom_t      encode( long double             ) const;  // floating-point
  template< class T >
  typename tt::enable_if< tt::is_floating_point<T>::value, ps_atom_t >::type
                 encode( T               const & ) const;  // floating-point
  template< class T >
  ps_atom_t      encode( std::complex<T> const & ) const;  // complex
  template< class T >
  ps_sequence_t  encode( std::vector<T>  const & ) const;  // sequence
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

inline  bool
  fhicl::ParameterSet::operator == ( ParameterSet const & other ) const
{ return id_ == other.id(); }

inline  bool
  fhicl::ParameterSet::operator != ( ParameterSet const & other ) const
{ return ! operator==(other); }

// ----------------------------------------------------------------------

template< class T >  // unsigned
typename tt::enable_if< tt::is_uint<T>::value
                      , fhicl::ParameterSet::ps_atom_t
                      >::type
  fhicl::ParameterSet::encode( T const & value ) const
{
  return encode( uintmax_t(value) );
}

template< class T >  // signed
typename tt::enable_if< tt::is_int<T>::value
                      , fhicl::ParameterSet::ps_atom_t
                      >::type
  fhicl::ParameterSet::encode( T const & value ) const
{
  return encode( intmax_t(value) );
}

template< class T >  // floating-point
typename tt::enable_if< tt::is_floating_point<T>::value
                      , fhicl::ParameterSet::ps_atom_t
                      >::type
  fhicl::ParameterSet::encode( T const & value ) const
{
  typedef  long double  ldbl;
  return encode( ldbl(value) );
}

template< class T >  // complex
fhicl::ParameterSet::ps_atom_t
  fhicl::ParameterSet::encode( std::complex<T> const & value ) const
{
  return  '(' + encode(value.real())
       +  ',' + encode(value.imag())
       +  ')';
}

template< class T >  // sequence
fhicl::ParameterSet::ps_sequence_t
  fhicl::ParameterSet::encode( std::vector<T> const & value ) const
{
  ps_sequence_t  result;
  for( typename std::vector<T>::const_iterator it = value.begin()
                                             , e  = value.end()
     ; it != e; ++it ) {
    result.push_back(boost::any(encode(*it)));
  }
  return result;
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
  if( a.type() == typeid(std::string) ) {
    typedef  fhicl::extended_value       extended_value;
    typedef  extended_value::sequence_t  sequence_t;

    std::string str;
    decode(a, str);

    extended_value xval;
    if( ! parse_value(str, xval) || ! xval.is_a(SEQUENCE) )
      throw fhicl::exception(type_mismatch, "invalid sequence string: ") << str;

    sequence_t const & seq = sequence_t(xval);
    result.clear();
    T via;
    for( sequence_t::const_iterator it = seq.begin()
                                  , e  = seq.end(); it != e; ++it ) {
      decode( it->to_string(), via );
      result.push_back(via);
    }
  }

  else if( a.type() == typeid(ps_sequence_t) ) {
    ps_sequence_t const & seq = boost::any_cast<ps_sequence_t>(a);
    result.clear();
    T via;
    for( ps_sequence_t::const_iterator it = seq.begin()
                                     , e  = seq.end(); it != e; ++it ) {
      decode( *it, via );
      result.push_back(via);
    }
  }

  else
      throw fhicl::exception(type_mismatch, "invalid sequence");

}

template< class T >  // none of the above
typename tt::disable_if< tt::is_numeric<T>::value, void >::type
  fhicl::ParameterSet::decode( boost::any const & a, T & result ) const
{
  result = boost::any_cast<T>(a);
}

// ======================================================================

#endif  // FHICLCPP__PARAMETERSET_H
