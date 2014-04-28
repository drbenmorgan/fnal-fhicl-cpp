#ifndef FHICLCPP_TYPE_TRAITS_H
#define FHICLCPP_TYPE_TRAITS_H
#ifndef __GCCXML__
// ======================================================================
//
// type traits
//
// ======================================================================

#include <complex>
#include <type_traits>
// ======================================================================

namespace tt {
  using std::is_floating_point;

  using std::enable_if;
  template <bool b, typename T = void>
  using
  disable_if = std::enable_if<!b, T>;
  template<typename T> struct is_int :
    public std::integral_constant<bool,
                                  std::is_integral<T>::value && 
                                  ! std::is_unsigned<T>::value> {
  };
  template <typename T>
  using is_numeric = std::is_arithmetic<T>;
  template <typename T>
  using is_uint = std::is_unsigned<T>;
}

#endif /* __GCCXML__ */
#endif /* FHICLCPP_TYPE_TRAITS_H */

// Local Variables:
// mode: c++
// End:
