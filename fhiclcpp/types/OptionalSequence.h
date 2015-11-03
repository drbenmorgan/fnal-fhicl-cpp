#ifndef fhiclcpp_types_OptionalSequence_h
#define fhiclcpp_types_OptionalSequence_h

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
#include <iostream>
#include <string>
#include <type_traits>

namespace fhicl {

  class ParameterSet;

  //==================================================================
  // e.g. OptionalSequence<int,4> ====> std::array<int,4>
  //
  template<typename T, std::size_t SIZE = -1>
  class OptionalSequence final : public detail::SequenceBase {
  public:

    static_assert(!tt::is_table_fragment<T>::value, NO_NESTED_TABLE_FRAGMENTS );
    static_assert(!tt::is_optional_parameter<T>::value, NO_OPTIONAL_TYPES );

    using ftype = std::array< tt::fhicl_type <T>, SIZE >;
    using rtype = std::array< tt::return_type<T>, SIZE >;

    explicit OptionalSequence(Name && name, Comment && cmt );
    explicit OptionalSequence(Name && name) : OptionalSequence( std::move(name), Comment("") ) {}

    OptionalSequence();

    bool operator()(rtype& t) const {

      if (!has_value_) return false;

      rtype result = { tt::return_type<T>() };
      std::size_t i{};

      for ( auto const& elem : value_ )
        result.at(i++) = elem();

      std::swap(result, t);

      // FIXME: If the rtype contains a Table<>, then additional keys
      // will be added to the registry whenever 'push_back' is called.
      // In principle, these should be removed whenever the
      // destructors are called.  But that may not be until the end of
      // some scope in which another registering call is made.
      detail::ParameterSchemaRegistry::instance().clear();
      return true;
    }

    auto const & get_ftype() const { return value_; }
    auto       & get_ftype()       { return value_; }

  private:

    ftype value_;
    bool has_value_ {false};

    void finalize_elements()
    {
      std::size_t i{};
      clear_elements();
      for ( auto & elem : value_ ) {
        set_elements( elem.get_ftype(), elem, key(), i++ );
        this->append_to_elements(&elem);
      }
    }

    void do_set_value(fhicl::ParameterSet const&, bool const trimParents) override;

  };

  //==================================================================
  // e.g. OptionalSequence<int> ====> std::vector<int>
  //
  template<typename T>
  class OptionalSequence<T,-1> final : public detail::SeqVectorBase {
  public:

    static_assert(!tt::is_table_fragment<T>::value, NO_NESTED_TABLE_FRAGMENTS );
    static_assert(!tt::is_optional_parameter<T>::value, NO_OPTIONAL_TYPES );

    using ftype = std::vector< tt::fhicl_type <T> >;
    using rtype = std::vector< tt::return_type<T> >;

    explicit OptionalSequence(Name && name, Comment && cmt );
    explicit OptionalSequence(Name && name) : OptionalSequence( std::move(name), Comment("") ) {}

    OptionalSequence();

    bool operator()(rtype& t) const {

      if (!has_value_) return false;

      rtype result;
      for ( auto const& elem : value_ )
        result.push_back( elem() );

      std::swap(result, t);

      // FIXME: If the rtype contains a Table<>, then additional keys
      // will be added to the registry whenever 'push_back' is called.
      // In principle, these should be removed whenever the
      // destructors are called.  But that may not be until the end of
      // some scope in which another registering call is made.
      detail::ParameterSchemaRegistry::instance().clear();
      return true;
    }

    auto const & get_ftype() const { return value_; }
    auto       & get_ftype()       { return value_; }

  private:

    ftype value_ {};
    bool has_value_ {false};

    void finalize_elements()
    {
      std::size_t i{};
      clear_elements();
      for ( auto & elem : value_ ) {
        set_elements( elem.get_ftype(), elem, key(), i++ );
        this->append_to_elements(&elem);
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

    void do_set_value(fhicl::ParameterSet const&, bool const trimParents) override;

  };

}

#include "fhiclcpp/types/detail/OptionalSequence.icc"

#endif

// Local variables:
// mode: c++
// End:
