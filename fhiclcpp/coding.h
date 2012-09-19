#ifndef FHICLCPP_CODING_H
#define FHICLCPP_CODING_H

// ======================================================================
//
// coding
//
// ======================================================================

#include "boost/any.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/numeric/conversion/cast.hpp"
#include "cpp0x/cstdint"
#include "cpp0x/string"
#include "fhiclcpp/ParameterSetID.h"
#include "fhiclcpp/exception.h"
#include "fhiclcpp/extended_value.h"
#include "fhiclcpp/fwd.h"
#include "fhiclcpp/parse.h"
#include "fhiclcpp/type_traits.h"
#include <complex>
#include <vector>

namespace fhicl {  namespace detail {
  typedef  std::string                ps_atom_t;
  typedef  std::vector<boost::any>    ps_sequence_t;
  typedef  long double                ldbl;

  inline  bool
    is_sequence( boost::any const & val )
  { return val.type() == typeid(ps_sequence_t); }

  inline  bool
    is_table( boost::any const & val )
  { return val.type() == typeid(ParameterSetID); }

// ----------------------------------------------------------------------

  ps_atom_t      encode( std::string     const & );  // string (w/ quotes)
  ps_atom_t      encode( char            const * );  // string (w/ quotes)
  ps_atom_t      encode( void *                  );  // nil
  ps_atom_t      encode( bool                    );  // bool
  ParameterSetID encode( ParameterSet    const & );  // table
  ps_atom_t      encode( std::uintmax_t          );  // unsigned
  template< class T >
  typename tt::enable_if< tt::is_uint<T>::value, ps_atom_t >::type
                 encode( T               const & );  // unsigned
  ps_atom_t      encode( std::intmax_t           );  // signed
  template< class T >
  typename tt::enable_if< tt::is_int<T>::value, ps_atom_t >::type
                 encode( T               const & );  // signed
  ps_atom_t      encode( ldbl                    );  // floating-point
  template< class T >
  typename tt::enable_if< tt::is_floating_point<T>::value, ps_atom_t >::type
                 encode( T               const & );  // floating-point
  template< class T >
  ps_atom_t      encode( std::complex<T> const & );  // complex
  template< class T >
  ps_sequence_t  encode( std::vector<T>  const & );  // sequence
  template< class T >
  typename tt::disable_if< tt::is_numeric<T>::value, std::string >::type
                 encode( T               const & );  // none of the above

// ----------------------------------------------------------------------

  void  decode( boost::any const &, std::string        & );  // string
  void  decode( boost::any const &, void *             & );  // nil
  void  decode( boost::any const &, bool               & );  // bool
  void  decode( boost::any const &, ParameterSet       & );  // table
  void  decode( boost::any const &, std::uintmax_t     & );  // unsigned
  template< class T >
  typename tt::enable_if< tt::is_uint<T>::value, void >::type
        decode( boost::any const &, T                  & );  // unsigned
  void  decode( boost::any const &, std::intmax_t      & );  // signed
  template< class T >
  typename tt::enable_if< tt::is_int<T>::value, void >::type
        decode( boost::any const &, T                  & );  // signed
  void  decode( boost::any const &, ldbl               & );  // floating-point
  template< class T >
  typename tt::enable_if< tt::is_floating_point<T>::value, void >::type
        decode( boost::any const &, T                  & );  // floating-point
  void  decode( boost::any const &, std::complex<ldbl> & );  // complex
  template< class T >
  void  decode( boost::any const &, std::complex<T>    & );  // complex
  template< class T >
  void  decode( boost::any const &, std::vector<T>     & );  // sequence
  template< class T >
  typename tt::disable_if< tt::is_numeric<T>::value, void >::type
        decode( boost::any const &, T                  & );  // none of the above

} }  // fhicl::detail, fhicl

// ======================================================================

template< class T >  // unsigned
typename tt::enable_if< tt::is_uint<T>::value
                      , fhicl::detail::ps_atom_t
                      >::type
  fhicl::detail::encode( T const & value )
{
  return encode( uintmax_t(value) );
}

template< class T >  // signed
typename tt::enable_if< tt::is_int<T>::value
                      , fhicl::detail::ps_atom_t
                      >::type
  fhicl::detail::encode( T const & value )
{
  return encode( intmax_t(value) );
}

template< class T >  // floating-point
typename tt::enable_if< tt::is_floating_point<T>::value
                      , fhicl::detail::ps_atom_t
                      >::type
  fhicl::detail::encode( T const & value )
{
  return encode( ldbl(value) );
}

template< class T >  // complex
fhicl::detail::ps_atom_t
  fhicl::detail::encode( std::complex<T> const & value )
{
  return  '(' + encode(value.real())
       +  ',' + encode(value.imag())
       +  ')';
}

template< class T >  // sequence
fhicl::detail::ps_sequence_t
  fhicl::detail::encode( std::vector<T> const & value )
{
  ps_sequence_t  result;
  for( typename std::vector<T>::const_iterator it = value.begin()
                                             , e  = value.end()
     ; it != e; ++it ) {
    result.push_back( boost::any(encode(*it)) );
  }
  return result;
}

template< class T >  // none of the above
typename tt::disable_if< tt::is_numeric<T>::value, std::string >::type
  fhicl::detail::encode( T const & value )
{
  return boost::lexical_cast<std::string>(value);
}

// ----------------------------------------------------------------------

template< class T >  // unsigned
typename tt::enable_if< tt::is_uint<T>::value, void >::type
  fhicl::detail::decode( boost::any const & a, T & result )
{
  std::uintmax_t via;
  decode(a, via);
  result = boost::numeric_cast<T>(via);
}

template< class T >  // signed
typename tt::enable_if< tt::is_int<T>::value, void >::type
  fhicl::detail::decode( boost::any const & a, T & result )
{
  std::intmax_t via;
  decode(a, via);
  result = boost::numeric_cast<T>(via);
}

template< class T >  // floating-point
typename tt::enable_if< tt::is_floating_point<T>::value, void >::type
  fhicl::detail::decode( boost::any const & a, T & result )
{
  ldbl via;
  decode(a, via);
  result = via;  // boost::numeric_cast<T>(via);
}

template< class T >  // complex
void
  fhicl::detail::decode( boost::any const & a, std::complex<T> & result )
{
  std::complex<ldbl> via;
  decode(a, via);
  result = std::complex<T>( boost::numeric_cast<T>(via.real())
                          , boost::numeric_cast<T>(via.imag())
                          );
}

template< class T >  // sequence
void
  fhicl::detail::decode( boost::any const & a, std::vector<T> & result )
{
  if( a.type() == typeid(std::string) ) {
    typedef  fhicl::extended_value       extended_value;
    typedef  extended_value::sequence_t  sequence_t;

    std::string str;
    decode(a, str);

    extended_value xval;
    std::string unparsed;
    if( ! parse_value_string(str, xval, unparsed) || ! xval.is_a(SEQUENCE) )
      throw fhicl::exception(type_mismatch, "error in sequence string:\n")
        << str
        << "\nat or before:\n" << unparsed;

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
  fhicl::detail::decode( boost::any const & a, T & result )
{
  result = boost::any_cast<T>(a);
}

// ======================================================================

#endif
