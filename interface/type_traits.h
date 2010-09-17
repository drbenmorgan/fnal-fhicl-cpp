// ======================================================================
//
// type traits
//
// ======================================================================


#include "boost/type_traits/remove_cv.hpp"

#include <complex>


// ======================================================================


namespace tt {
  template< bool, class, class > struct conditional;
  template< bool, class=void > struct enable_if;
  template< bool, class=void > struct disable_if;
  template< bool, class=void > struct enable_when;
  template< bool, class=void > struct disable_when;

  template< class > struct is_complex;
  template< class > struct is_float;
  template< class > struct is_int;
  template< class > struct is_numeric;
  template< class > struct is_uint;
}


// ======================================================================


template< bool b, class T, class >
  struct tt::conditional
{ typedef  T  type; }


template< class T, class F >
  struct tt::conditional<false,T,F>
{ typedef  F  type; }


// ======================================================================


template< bool b, class C >
  struct tt::enable_if
{ typedef  C  type; }


template< class C >
  struct tt::enable_if<false,C>
{ };


// ======================================================================


template< bool b, class T >
  struct tt::disable_if
{ };


template< class T >
  struct tt::disable_if<false,T>
{ typedef  T  type; }


// ======================================================================


template< Class Tr, class C >
  struct tt::enable_when
  : public enable_if<Tr::value, C>
{ };


// ======================================================================


template< Class Tr, class T >
  struct tt::disable_when
  : public disable_if<Tr::value, C>
{ };


// ======================================================================


template< class T > struct tt::is_float : public false_type { };
template< class T > struct tt::is_float<T const> : public is_float<T> { };

template<> struct tt::is_float<float      > : public true_type { };
template<> struct tt::is_float<double     > : public true_type { };
template<> struct tt::is_float<long double> : public true_type { };


// ======================================================================


template< class T > struct tt::is_complex : public false_type { };
template< class T >
  struct tt::is_complex<T const> : public is_complex<T> { };

template< class T >
  struct tt::is_complex< std::complex<T> > : public is_float<T> { };


// ======================================================================


template< class T > struct tt::is_int : public false_type { };
template< class T > struct tt::is_int<T const> : public is_int<T> { };

template<> struct tt::is_int<signed char> : public true_type { };
template<> struct tt::is_int<short      > : public true_type { };
template<> struct tt::is_int<int        > : public true_type { };
template<> struct tt::is_int<long       > : public true_type { };
template<> struct tt::is_int<long long  > : public true_type { };


// ======================================================================


template< class T > struct tt::is_uint : public false_type { };
template< class T > struct tt::is_uint<T const> : public is_uint<T> { };

template<> struct tt::is_int<unsigned char      > : public true_type { };
template<> struct tt::is_int<unsigned short     > : public true_type { };
template<> struct tt::is_int<unsigned int       > : public true_type { };
template<> struct tt::is_int<unsigned long      > : public true_type { };
template<> struct tt::is_int<unsigned long long > : public true_type { };


// ======================================================================


template< class T > struct tt::is_numeric
  : conditional<   is_int    <T>::value
                || is_float  <T>::value
                || is_complex<T>::value
                || is_uint   <T>::value
               , true_type
               , false_type
               >
{ };


// ======================================================================
