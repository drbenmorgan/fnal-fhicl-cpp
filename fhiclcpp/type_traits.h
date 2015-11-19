#ifndef fhiclcpp_type_traits_h
#define fhiclcpp_type_traits_h
#ifndef __GCCXML__
// ======================================================================
//
// type traits
//
// ======================================================================

#include <array>
#include <complex>
#include <tuple>
#include <utility>
#include <vector>

#include <type_traits>

// ======================================================================

namespace fhicl {

  template <typename T> struct Atom;
  template <typename T> struct OptionalAtom;

  template <typename T> struct Table;
  template <typename T> struct OptionalTable;

  template <typename T> struct TableFragment;

  template <typename ... ARGS> struct Tuple;
  template <typename ... ARGS> struct OptionalTuple;

  template <typename T, std::size_t SZ> struct Sequence;
  template <typename T, std::size_t SZ> struct OptionalSequence;

  template <typename T, typename ... ARGS> struct TupleAs;
  template <typename T, typename ... ARGS> struct OptionalTupleAs;

}

namespace tt {

  using std::is_floating_point;
  using std::enable_if;

  template <bool b, typename T = void>
  using disable_if = std::enable_if<!b, T>;

  template<typename T>
  struct is_int : public std::integral_constant<bool,
                                                std::is_integral<T>::value &&
                                                ! std::is_unsigned<T>::value> {};

  template <typename T>
  using is_numeric = std::is_arithmetic<T>;

  template <typename T>
  using is_uint = std::is_unsigned<T>;

  //=======================================================
  // Enforce (non)const-ness
  //
  enum class const_flavor { require_non_const, require_const };

  template <typename T, const_flavor C>
  struct maybe_const {
    using type = T;
  };

  template <typename T>
  struct maybe_const<T,const_flavor::require_const> {
    using type = std::add_const_t<T>;
  };

  template <typename T, const_flavor C>
  using maybe_const_t = typename maybe_const<T, C>::type;

  //=======================================================
  // Check if sequence type
  //
  template <typename Container> struct is_sequence_type : std::false_type {};

  template <typename ARG,std::size_t SZ>
  struct is_sequence_type<std::array<ARG,SZ>> : std::true_type {};
  template <typename ... ARGS>
  struct is_sequence_type<std::tuple <ARGS...>> : std::true_type {};
  template <typename ... ARGS>
  struct is_sequence_type<std::vector<ARGS...>> : std::true_type {};

  //=======================================================
  // Check if Table<>
  //
  template <typename T> struct is_table : std::false_type {};

  template <typename T>
  struct is_table<fhicl::Table<T>> : std::true_type {};

  //=======================================================
  // Check if TableFragment<>
  //
  template <typename T> struct is_table_fragment : std::false_type {};

  template <typename T>
  struct is_table_fragment<fhicl::TableFragment<T>> : std::true_type {};

  //=======================================================
  // Check if optional parameter
  //
  template <typename T> struct is_optional_parameter : std::false_type {};

  template <typename T>
  struct is_optional_parameter<fhicl::OptionalTable<T>> : std::true_type {};

  template <typename T>
  struct is_optional_parameter<fhicl::OptionalAtom<T>> : std::true_type {};

  template <typename T, std::size_t SZ>
  struct is_optional_parameter<fhicl::OptionalSequence<T, SZ>> : std::true_type {};

  template <typename ... TYPES>
  struct is_optional_parameter<fhicl::OptionalTuple<TYPES...>> : std::true_type {};

  template <typename T, typename ... ARGS>
  struct is_optional_parameter<fhicl::OptionalTupleAs<T(ARGS...)>> : std::true_type {};

  //=======================================================
  // Check if fhicl type -- i.e. Atom<>, Table<>, etc.
  //
  template <typename T>
  struct is_fhicl_type : std::false_type {};

  // ... Table
  template <typename T>
  struct is_fhicl_type<fhicl::Table<T>> : std::true_type {};

  template <typename T>
  struct is_fhicl_type<fhicl::OptionalTable<T>> : std::true_type {};

  // ... Atom
  template <typename T>
  struct is_fhicl_type<fhicl::Atom<T>> : std::true_type {};

  template <typename T>
  struct is_fhicl_type<fhicl::OptionalAtom<T>> : std::true_type {};

  // ... Sequence
  template <typename T, std::size_t SZ>
  struct is_fhicl_type<fhicl::Sequence<T, SZ>> : std::true_type {};

  template <typename T, std::size_t SZ>
  struct is_fhicl_type<fhicl::OptionalSequence<T, SZ>> : std::true_type {};

  // ... Tuple
  template <typename ... TYPES>
  struct is_fhicl_type<fhicl::Tuple<TYPES...>> : std::true_type {};

  template <typename ... TYPES>
  struct is_fhicl_type<fhicl::OptionalTuple<TYPES...>> : std::true_type {};

  // ... TupleAs
  template <typename T, typename ... ARGS>
  struct is_fhicl_type<fhicl::TupleAs<T(ARGS...)>> : std::true_type {};

  template <typename T, typename ... ARGS>
  struct is_fhicl_type<fhicl::OptionalTupleAs<T(ARGS...)>> : std::true_type {};

  //=======================================================
  // Get FHiCL types: can be Atom, Sequence, Tuple, or Table
  //
  template <typename T>
  struct fhicl_type_impl {
    using type = fhicl::Atom<T>;
  };

  template <typename T, std::size_t SZ>
  struct fhicl_type_impl<fhicl::Sequence<T,SZ>> {
    using type = fhicl::Sequence<T,SZ>;
  };

  template <typename T>
  struct fhicl_type_impl<fhicl::Table<T>> {
    using type = fhicl::Table<T>;
  };

  template <typename ... ARGS>
  struct fhicl_type_impl<fhicl::Tuple<ARGS...>> {
    using type = fhicl::Tuple<ARGS...>;
  };

  template <typename T, typename ... ARGS>
  struct fhicl_type_impl<fhicl::TupleAs<T(ARGS...)>> {
    using type = fhicl::TupleAs<T(ARGS...)>;
  };

  template <typename T>
  struct fhicl_type_impl<fhicl::OptionalAtom<T>> {
    using type = fhicl::OptionalAtom<T>;
  };

  template <typename T, std::size_t SZ>
  struct fhicl_type_impl<fhicl::OptionalSequence<T,SZ>> {
    using type = fhicl::OptionalSequence<T,SZ>;
  };

  template <typename T>
  struct fhicl_type_impl<fhicl::OptionalTable<T>> {
    using type = fhicl::OptionalTable<T>;
  };

  template <typename ... ARGS>
  struct fhicl_type_impl<fhicl::OptionalTuple<ARGS...>> {
    using type = fhicl::OptionalTuple<ARGS...>;
  };

  template <typename T, typename ... ARGS>
  struct fhicl_type_impl<fhicl::OptionalTupleAs<T(ARGS...)>> {
    using type = fhicl::OptionalTupleAs<T(ARGS...)>;
  };

  // The alias
  template <typename T>
  using fhicl_type = typename fhicl_type_impl<T>::type;


  //=======================================================
  // Get Return types
  //
  template <typename T> struct return_type_impl {
    using rtype = T;
  };

  template <typename T>
  struct return_type_impl<fhicl::Atom<T> >{
    using rtype = typename fhicl::Atom<T>::rtype;
  };

  template <typename T, std::size_t SZ>
  struct return_type_impl<fhicl::Sequence<T,SZ>> {
    using rtype = typename fhicl::Sequence<T,SZ>::rtype;
  };

  template <typename S>
  struct return_type_impl<fhicl::Table<S>> {
    using rtype = typename fhicl::Table<S>::rtype;
  };

  template <typename ... ARGS>
  struct return_type_impl<fhicl::Tuple<ARGS...>> {
    using rtype = typename fhicl::Tuple<ARGS...>::rtype;
  };

  template <typename T, typename ... ARGS>
  struct return_type_impl<fhicl::TupleAs<T(ARGS...)>> {
    using rtype = typename fhicl::TupleAs<T(ARGS...)>::rtype;
  };

  // The alias
  template < typename ... ARGS>
  using return_type = typename return_type_impl<ARGS...>::rtype;

}

#endif /* __GCCXML__ */
#endif /* fhiclcpp_type_traits_h */

// Local Variables:
// mode: c++
// End:
