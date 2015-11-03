#ifndef fhiclcpp_types_OptionalTuple_h
#define fhiclcpp_types_OptionalTuple_h

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
  // e.g. OptionalTuple<int,double,bool> ====> std::tuple<int,double,bool>
  //

  template<typename ... TYPES>
  class OptionalTuple final : public detail::SequenceBase {
  public:

    using ftype = std::tuple< tt::fhicl_type <TYPES>... >;
    using rtype = std::tuple< tt::return_type<TYPES>... >;

    explicit OptionalTuple(Name&& name, Comment&& cmt );
    explicit OptionalTuple(Name&& name) : OptionalTuple( std::move(name), Comment("") ) {}

    OptionalTuple();

    bool operator()(rtype&) const;

    auto const & get_ftype() const { return value_; }
    auto       & get_ftype()       { return value_; }

  private:

    ftype value_ {};
    bool  has_value_ { false };

    //=================================================================
    // aliases
    using TUPLE  = std::tuple<tt::fhicl_type<TYPES>...>;

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

    void do_set_value(fhicl::ParameterSet const&, bool const /*trimParents*/) override
    {
      // We do not explicitly set the sequence values here as the
      // individual elements are set one at a time.  However, this
      // function is reached in the ValidateThenSet algorithm if the
      // optional parameter is present.  Otherwise, this override is
      // skipped.
      has_value_ = true;
    }

  }; // class OptionalTuple

  //================= IMPLEMENTATION =========================
  //
  template<typename ... TYPES>
  OptionalTuple<TYPES...>::OptionalTuple(Name&& name,
                                         Comment&& comment)
    : SequenceBase{std::move(name),std::move(comment),value_type::OPTIONAL,par_type::TUPLE,this}
  {
    finalize_elements();
    NameStackRegistry::end_of_ctor();
  }

  template<typename ... TYPES>
  OptionalTuple<TYPES...>::OptionalTuple()
    : SequenceBase{Name::anonymous(),Comment(""),value_type::OPTIONAL,par_type::TUPLE,this}
  {
    finalize_elements();
    NameStackRegistry::end_of_ctor();
  }

  template<typename ... TYPES>
  bool
  OptionalTuple<TYPES...>::operator()(rtype& r) const
  {
    if (!has_value_) return false;
    rtype result;
    assemble_rtype(result);
    std::swap(result, r);
    detail::ParameterSchemaRegistry::instance().clear();
    return true;
  }

}

#endif

// Local variables:
// mode: c++
// End:
