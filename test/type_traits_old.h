#ifndef FHICLCPP_TYPE_TRAITS_OLD_H
#define FHICLCPP_TYPE_TRAITS_OLD_H

// ======================================================================
//
// type traits
//
// ======================================================================

#include "cpp0x/type_traits"
#include <complex>

// ======================================================================

namespace tt_old {
  using std::is_floating_point;

  using std::enable_if;

#ifndef __GCCXML__
  template <bool b, typename T = void>
  using
  disable_if = std::enable_if<!b, T>;
#else
  template <bool b, class  T = void>
  struct disable_if : std::enable_if<!b, T> { };
#endif
  template< class > struct is_complex;
  template< class > struct is_int;
  template< class > struct is_numeric;
  template< class > struct is_uint;
}

// ======================================================================

namespace tt_old {
  template< class T > struct is_complex : public std::false_type { };

  template< class T >
    struct is_complex< std::complex<T> > : public is_floating_point<T> { };
}

// ----------------------------------------------------------------------

namespace tt_old {
  template< class T > struct is_int : public std::false_type { };
  template< class T > struct is_int<T const> : public is_int<T> { };

  template<> struct is_int<signed char> : public std::true_type { };
  template<> struct is_int<short      > : public std::true_type { };
  template<> struct is_int<int        > : public std::true_type { };
  template<> struct is_int<long       > : public std::true_type { };
}

// ----------------------------------------------------------------------

namespace tt_old {
  template< class T > struct is_uint : public std::false_type { };
  template< class T > struct is_uint<T const> : public is_uint<T> { };

  template<> struct is_uint<unsigned char     > : public std::true_type { };
  template<> struct is_uint<unsigned short    > : public std::true_type { };
  template<> struct is_uint<unsigned int      > : public std::true_type { };
  template<> struct is_uint<unsigned long     > : public std::true_type { };
}

// ----------------------------------------------------------------------

namespace tt_old {
  template< class T > struct is_numeric
    : public std::integral_constant< bool
                                   ,    is_int           <T>::value
                                     || is_uint          <T>::value
                                     || is_floating_point<T>::value
                                   >
  { };
}

// ======================================================================

#endif

// Local Variables:
// mode: c++
// End:
