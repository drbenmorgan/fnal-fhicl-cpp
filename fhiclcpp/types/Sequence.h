#ifndef fhiclcpp_types_Sequence_h
#define fhiclcpp_types_Sequence_h

#include "fhiclcpp/types/Atom.h"
#include "fhiclcpp/types/Table.h"
#include "fhiclcpp/types/detail/NameStackRegistry.h"
#include "fhiclcpp/types/detail/ParameterArgumentTypes.h"
#include "fhiclcpp/types/detail/ParameterMetadata.h"
#include "fhiclcpp/types/detail/ParameterSchemaRegistry.h"
#include "fhiclcpp/types/detail/SequenceBase.h"
#include "fhiclcpp/types/detail/ostream_helpers.h"
#include "fhiclcpp/types/detail/SeqVectorBase.h"
#include "fhiclcpp/type_traits.h"

#include <array>
#include <string>
#include <type_traits>

namespace fhicl {

  class ParameterSet;

  //==================================================================
  // e.g. Sequence<int,4> ====> std::array<int,4>
  //
  template<typename T, std::size_t SIZE = -1>
  class Sequence final : public detail::SequenceBase {
  public:

    static_assert(!tt::is_table_fragment<T>::value, NO_NESTED_TABLE_FRAGMENTS );
    static_assert(!tt::is_optional_parameter<T>::value, NO_OPTIONAL_TYPES );

    using ftype = std::array< tt::fhicl_type <T>, SIZE >;
    using rtype = std::array< tt::return_type<T>, SIZE >;

    explicit Sequence(Name && name, Comment && cmt );
    explicit Sequence(Name && name, Comment && cmt, Sequence<T,SIZE> const& dflt);

    explicit Sequence(Name && name) : Sequence{std::move(name), Comment("")} {}
    explicit Sequence(Name && name, Sequence<T,SIZE> const & dflt ) : Sequence{std::move(name), Comment(""), dflt} {}
    explicit Sequence(Name && name, Sequence<T,SIZE> const & dflt, Comment && cmt) : Sequence{std::move(name), std::move(cmt), dflt} {}

    /* Default value support

       Two properties regarding the following constructor are
       motivated due to nested default value support (e.g.):

       Sequence< Sequence< int, 2>, 2> { { 1, 2}, { 3, 4} };

       1. The constructor cannot be 'explicit' since the inner-nested
       initializer lists need to be implicitly converted to type
       std::initializer_list.

       2. The template argument must be 'T' and cannot be
       'tt::return_type<T>'; otherwise the return type for
       Sequence<int> is an std::array<int,2> and an additional
       'Sequence(std::array<T,SIZE> const & vec)' constructor is
       necessary.
    */
    Sequence(std::initializer_list<T> args);

    Sequence();

    auto operator()() const {

      rtype result = { tt::return_type<T>() };
      std::size_t i{};

      for ( auto const& elem : value_ ) {
        result.at(i++) = elem();
      }
      return result;
    }

    auto operator()(std::size_t i) const {
      return value_.at(i)();
    }

    auto const & get_ftype() const { return value_; }
    auto       & get_ftype()       { return value_; }

  private:

    ftype value_;

    void finalize_elements()
    {
      std::size_t i{};
      clear_elements();
      for ( auto & elem : value_ ) {
        set_elements( elem.get_ftype(), elem, key(), i++ );
        append_to_elements( ptr_to_base(elem) );
      }
    }

    void do_set_value(fhicl::ParameterSet const&, bool /*trimParents*/) override {}

  };

  //==================================================================
  // e.g. Sequence<int> ====> std::vector<int>
  //
  template<typename T>
  class Sequence<T,-1> final : public detail::SeqVectorBase {
  public:

    static_assert(!tt::is_table_fragment<T>::value, NO_NESTED_TABLE_FRAGMENTS );
    static_assert(!tt::is_optional_parameter<T>::value, NO_OPTIONAL_TYPES );

    using ftype = std::vector< tt::fhicl_type <T> >;
    using rtype = std::vector< tt::return_type<T> >;

    explicit Sequence(Name && name, Comment && cmt);
    explicit Sequence(Name && name, Comment && cmt, Sequence<T,-1>  const & dflt);

    explicit Sequence(Name && name) : Sequence{std::move(name), Comment("")} {}
    explicit Sequence(Name && name, Sequence<T,-1> const & dflt) : Sequence{std::move(name), Comment(""), dflt} {}
    explicit Sequence(Name && name, Sequence<T,-1> const & dflt, Comment && cmt) : Sequence{std::move(name), std::move(cmt), dflt} {}

    /* Default value support

       Two properties regarding the following constructor are
       motivated due to nested default value support (e.g.):

       Sequence< Sequence< int > > { { 1, 2}, { 3, 4} };

       1. The constructor cannot be 'explicit' since the inner-nested
       initializer lists need to be implicitly converted to type
       std::initializer_list.

       2. The template argument must be 'T' and cannot be
       'tt::return_type<T>'; otherwise the return type for
       Sequence<int> is an std::vector<int> and an additional
       'Sequence(std::vector<T> const & vec)' constructor is
       necessary.
    */
    Sequence(std::initializer_list<T> args);

    Sequence();

    static Sequence<T> make_empty() {
      Sequence<T> result;
      result.resize_sequence(0ul);
      return result;
    }

    auto operator()() const {
      rtype result;
      cet::transform_all(value_, std::back_inserter(result),
                         [](auto const& e){
                           return e();
                         } );
      return result;
    }
    auto operator()(std::size_t i) const {
      return value_.at(i)();
    }

    auto const & get_ftype() const { return value_; }
    auto       & get_ftype()       { return value_; }

  private:

    ftype value_ {};

    void finalize_elements()
    {
      std::size_t i{};
      clear_elements();
      for ( auto & elem : value_ ) {
        set_elements( elem.get_ftype(), elem, key(), i++ );
        append_to_elements( ptr_to_base(elem) );
      }
    }

    // To be used only for reassigning keys when ParameterSet
    // validation is being performed.
    void do_resize_sequence(std::size_t i) override
    {
      value_.clear();
      value_.resize(i);

      finalize_elements();
    }

    void do_set_value(fhicl::ParameterSet const&, bool /*trimParents*/) override {}

  };

}

#include "fhiclcpp/types/detail/Sequence.icc"

#endif

// Local variables:
// mode: c++
// End:
