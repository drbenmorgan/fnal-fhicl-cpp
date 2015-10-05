#ifndef fhiclcpp_Tuple_h
#define fhiclcpp_Tuple_h

#include "fhiclcpp/types/Atom.h"
#include "fhiclcpp/types/detail/NameStackRegistry.h"
#include "fhiclcpp/types/detail/ParameterBase.h"
#include "fhiclcpp/types/detail/ParameterRegistrySentry.h"
#include "fhiclcpp/types/detail/ParameterReferenceRegistry.h"
#include "fhiclcpp/types/detail/set_element_keys.h"
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
  class Tuple final : public detail::ParameterBase {
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
    ftype value_;

    //=================================================================
    // aliases
    using TUPLE  = std::tuple<tt::fhicl_type<TYPES>...>;
    using UTUPLE = std::tuple<TYPES...>;

    // finalizing tuple elements
    template <size_t I>
    typename std::enable_if<(I >= std::tuple_size<TUPLE>::value)>::type
    finalize_tuple_elements(){}

    template <size_t I>
    typename std::enable_if<(I < std::tuple_size<TUPLE>::value)>::type
    finalize_tuple_elements()
    {
      auto & elem = std::get<I>(value_);
      set_element_keys(elem.get_ftype(), elem, key(), I);
      finalize_tuple_elements<I+1>();
    }

    void finalize_elements()
    {
      finalize_tuple_elements<0>();
    }

    // filling tuple elements from default
    template <size_t I, typename UTUPLE>
    typename std::enable_if<(I >= std::tuple_size<TUPLE>::value)>::type
    fill_tuple_element(UTUPLE const&)
    {}

    template <size_t I, typename UTUPLE>
    typename std::enable_if<(I < std::tuple_size<TUPLE>::value)>::type
    fill_tuple_element(UTUPLE const& utuple)
    {

      /*
        One of the downsides of automatic registration of
        default-c'tored elements is that if you want to override
        the values later (as in the case with std::array or
        std::tuple), you need to first remove the entries of the
        registered empty elements and reassign.  Ideally, I
        wouldn't have to do this, but could reuse those map
        entries, but at the moment, I can't see a straightforward
        way without rewriting a significant portion of the entire
        system.

        Remove registered elements from default-c'tored value_
      */
      using elem_utype = typename std::tuple_element<I,UTUPLE>::type;
      static_assert(!tt::is_table<elem_utype>::value, NO_DEFAULTS_FOR_TABLE);

      auto & elem      = std::get<I>(value_);
      elem = tt::fhicl_type<elem_utype>( Name::anonymous(), std::get<I>(utuple) );
      fill_tuple_element<I+1>(utuple);
    }

    void fill_tuple_elements(UTUPLE const& utuple)
    {
      fill_tuple_element<0>( utuple );
    }

    // filling return type
    template <size_t I, typename rtype>
    typename std::enable_if<(I >= std::tuple_size<TUPLE>::value)>::type
    fill_return_element(rtype &) const
    {}

    template <size_t I, typename rtype>
    typename std::enable_if<(I < std::tuple_size<TUPLE>::value)>::type
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
    : ParameterBase(std::move(name),comment,false,par_type::TUPLE,this)
    , value_()
  {
    detail::ParameterRegistrySentry s;
    finalize_elements();
  }

  template<typename ... TYPES>
  Tuple<TYPES...>::Tuple(Name && name,
                         Comment && comment,
                         Tuple<TYPES...> const& dflt )
    : ParameterBase(std::move(name),comment,true,par_type::TUPLE,this)
    , value_(dflt.value_)
  {
    detail::ParameterRegistrySentry s;
    finalize_elements();
  }

  template<typename ... TYPES>
  Tuple<TYPES...>::Tuple()
    : ParameterBase(Name::anonymous(),Comment(""),false,par_type::TUPLE,this)
    , value_()
  {
    detail::ParameterRegistrySentry s;
    finalize_elements();
  }

  template<typename ... TYPES>
  Tuple<TYPES...>::Tuple(TYPES const & ... args)
    : ParameterBase(Name::anonymous(),Comment(""),false,par_type::TUPLE,this)
    , value_()
  {
    detail::ParameterRegistrySentry s;
    fill_tuple_elements( std::forward_as_tuple(args...) );
  }

  template<typename ... TYPES>
  auto
  Tuple<TYPES...>::operator()() const
  {
    rtype result;
    assemble_rtype(result);
    detail::ParameterReferenceRegistry::instance().clear();
    return result;
  }

}

#endif

// Local variables:
// mode: c++
// End:
