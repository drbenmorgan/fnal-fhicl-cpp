// ======================================================================
//
// type traits
//
// ======================================================================

#include "boost/type_traits/integral_constant.hpp"
#include "boost/type_traits/is_floating_point.hpp"
#include "boost/type_traits/remove_cv.hpp"

#include <complex>


// ======================================================================

namespace tt {
  using boost::true_type;
  using boost::false_type;
  using boost::is_floating_point;

  template< bool, class, class > struct conditional;
  template< bool, class = void > struct enable_if;
  template< bool, class = void > struct disable_if;

  template< class > struct is_complex;
  template< class > struct is_int;
  template< class > struct is_numeric;
  template< class > struct is_uint;

  template< class T
          , bool = is_int           <T>::value
          , bool = is_uint          <T>::value
          , bool = is_floating_point<T>::value
          >
    struct largest_type;
}


// ======================================================================

namespace tt {
  template< bool b, class T, class >
    struct conditional            { typedef  T  type; };
  template< class T, class F >
    struct conditional<false,T,F> { typedef  F  type; };
}

// ----------------------------------------------------------------------

namespace tt {
  template< bool b, class C >
    struct enable_if          { typedef  C  type; };
  template< class C >
    struct enable_if<false,C> { };
}

// ----------------------------------------------------------------------

namespace tt {
  template< bool b, class T >
    struct disable_if          { };
  template< class T >
    struct disable_if<false,T> { typedef  T  type; };
}

// ----------------------------------------------------------------------

namespace tt {
  template< class T > struct is_complex : public false_type { };
  template< class T >
    struct is_complex<T const> : public is_complex<T> { };

  template< class T >
    struct is_complex< std::complex<T> > : public is_floating_point<T> { };
}

// ----------------------------------------------------------------------

namespace tt {
  template< class T > struct is_int : public false_type { };
  template< class T > struct is_int<T const> : public is_int<T> { };

  template<> struct is_int<signed char> : public true_type { };
  template<> struct is_int<short      > : public true_type { };
  template<> struct is_int<int        > : public true_type { };
  template<> struct is_int<long       > : public true_type { };
  template<> struct is_int<long long  > : public true_type { };
}

// ----------------------------------------------------------------------

namespace tt {
  template< class T > struct is_uint : public false_type { };
  template< class T > struct is_uint<T const> : public is_uint<T> { };

  template<> struct is_int<unsigned char     > : public true_type { };
  template<> struct is_int<unsigned short    > : public true_type { };
  template<> struct is_int<unsigned int      > : public true_type { };
  template<> struct is_int<unsigned long     > : public true_type { };
  template<> struct is_int<unsigned long long> : public true_type { };
}

// ----------------------------------------------------------------------

namespace tt {
  template< class T > struct is_numeric
    : public conditional<    is_int  <T>::value || is_uint   <T>::value
                          || is_floating_point<T>::value
                        , true_type
                        , false_type
                        >
  { };
}

// ----------------------------------------------------------------------

namespace tt {
  template< class T > struct largest_type<T,true,false,false>
  { typedef  long long  type; };

  template< class T > struct largest_type<T,false,true,false>
  { typedef  long long  type; };

  template< class T > struct largest_type<T,false,false,true>
  { typedef  long double  type; };
}

// ----------------------------------------------------------------------
