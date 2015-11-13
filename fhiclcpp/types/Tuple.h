#ifndef fhiclcpp_types_Tuple_h
#define fhiclcpp_types_Tuple_h

#include "fhiclcpp/types/Atom.h"
#include "fhiclcpp/types/detail/NameStackRegistry.h"
#include "fhiclcpp/types/detail/ParameterSchemaRegistry.h"
#include "fhiclcpp/types/detail/SequenceBase.h"
#include "fhiclcpp/types/detail/type_traits_error_msgs.h"
#include "fhiclcpp/type_traits.h"

#include <string>
#include <utility>

namespace fhicl {

  class ParameterSet;

  //==================================================================
  // e.g. Tuple<int,double,bool> ====> std::tuple<int,double,bool>
  //

  template<typename ... TYPES>
  class Tuple final : public detail::SequenceBase {
  public:

    using ftype = std::tuple< tt::fhicl_type <TYPES>... >;
    using rtype = std::tuple< tt::return_type<TYPES>... >;

    explicit Tuple(Name && name, Comment && cmt );
    explicit Tuple(Name && name, Comment && cmt, Tuple<TYPES...> const& dflt );

    explicit Tuple(Name && name) : Tuple( std::move(name), Comment("") ) {}
    explicit Tuple(Name && name, Tuple<TYPES...> const& dflt ) : Tuple( std::move(name), Comment(""), dflt ) {}
    explicit Tuple(Name && name, Tuple<TYPES...> const& dflt, Comment && cmt ) : Tuple( std::move(name), std::move(cmt), dflt ) {}

    Tuple(TYPES const & ... args);

    Tuple();

    auto operator()() const;

    template<std::size_t I>
    auto get() const { return std::get<I>(value_)(); }

    auto const & get_ftype() const { return value_; }
    auto       & get_ftype()       { return value_; }

  private:

    //=================================================================
    // aliases
    using TUPLE  = std::tuple<tt::fhicl_type<TYPES>...>;
    using UTUPLE = std::tuple<TYPES...>;

    ftype value_;

    void do_set_value(fhicl::ParameterSet const&, bool /*trimParents*/) override {}

    // finalizing tuple elements
    template <size_t I>
    std::enable_if_t<(I >= std::tuple_size<TUPLE>::value)>
    finalize_tuple_elements(){}

    template <size_t I>
    std::enable_if_t<(I < std::tuple_size<TUPLE>::value)>
    finalize_tuple_elements()
    {
      using elem_ftype = std::tuple_element_t<I,TUPLE>;
      static_assert(!tt::is_table_fragment<elem_ftype>::value, NO_NESTED_TABLE_FRAGMENTS);
      static_assert(!tt::is_optional_parameter<elem_ftype>::value, NO_OPTIONAL_TYPES );

      auto & elem = std::get<I>(value_);
      set_elements(elem.get_ftype(), elem, key(), I);
      append_to_elements(&elem);
      finalize_tuple_elements<I+1>();
    }

    void finalize_elements()
    {
      finalize_tuple_elements<0>();
    }

    // filling tuple elements from default
    template <size_t I, typename UTUPLE>
    std::enable_if_t<(I >= std::tuple_size<TUPLE>::value)>
    fill_tuple_element(UTUPLE const&)
    {}

    template <size_t I, typename UTUPLE>
    std::enable_if_t<(I < std::tuple_size<TUPLE>::value)>
    fill_tuple_element(UTUPLE const& utuple)
    {
      using elem_utype = std::tuple_element_t<I,UTUPLE>;
      static_assert(!tt::is_table<elem_utype>::value, NO_DEFAULTS_FOR_TABLE);
      static_assert(!tt::is_sequence_type<elem_utype>::value, NO_STD_CONTAINERS);

      auto & elem = std::get<I>(value_);
      elem = tt::fhicl_type<elem_utype>( Name::anonymous(), std::get<I>(utuple) );
      fill_tuple_element<I+1>(utuple);
    }

    void fill_tuple_elements(UTUPLE const& utuple)
    {
      fill_tuple_element<0>( utuple );
    }

    // filling return type
    template <size_t I, typename rtype>
    std::enable_if_t<(I >= std::tuple_size<TUPLE>::value)>
    fill_return_element(rtype &) const
    {}

    template <size_t I, typename rtype>
    std::enable_if_t<(I < std::tuple_size<TUPLE>::value)>
    fill_return_element(rtype & result) const
    {
      std::get<I>(result) = std::get<I>(value_)();
      fill_return_element<I+1>(result);
    }

    void assemble_rtype(rtype & result) const
    {
      fill_return_element<0>( result );
    }

  }; // class Tuple

  //================= IMPLEMENTATION =========================
  //
  template<typename ... TYPES>
  Tuple<TYPES...>::Tuple(Name && name,
                         Comment && comment)
    : SequenceBase{std::move(name),std::move(comment),value_type::REQUIRED,par_type::TUPLE,this}
    , value_()
  {
    finalize_elements();
    NameStackRegistry::end_of_ctor();
  }

  template<typename ... TYPES>
  Tuple<TYPES...>::Tuple(Name && name,
                         Comment && comment,
                         Tuple<TYPES...> const& dflt )
    : SequenceBase{std::move(name),std::move(comment),value_type::DEFAULT,par_type::TUPLE,this}
    , value_(dflt.value_)
  {
    finalize_elements();
    NameStackRegistry::end_of_ctor();
  }

  template<typename ... TYPES>
  Tuple<TYPES...>::Tuple()
    : SequenceBase{Name::anonymous(),Comment(""),value_type::REQUIRED,par_type::TUPLE,this}
    , value_()
  {
    // do NOT finalize elements for default sequence!
    NameStackRegistry::end_of_ctor();
  }

  template<typename ... TYPES>
  Tuple<TYPES...>::Tuple(TYPES const & ... args)
    : SequenceBase{Name::anonymous(),Comment(""),value_type::REQUIRED,par_type::TUPLE,this}
    , value_()
  {
    fill_tuple_elements( std::forward_as_tuple(args...) );
    NameStackRegistry::end_of_ctor();
  }

  template<typename ... TYPES>
  auto
  Tuple<TYPES...>::operator()() const
  {
    rtype result;
    assemble_rtype(result);
    return result;
  }

}

#endif

// Local variables:
// mode: c++
// End:
