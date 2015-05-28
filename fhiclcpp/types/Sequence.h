#ifndef fhiclcpp_Sequence_h
#define fhiclcpp_Sequence_h

#include "fhiclcpp/types/Atom.h"
#include "fhiclcpp/types/Table.h"
#include "fhiclcpp/types/detail/KeyStackRegistry.h"
#include "fhiclcpp/types/detail/ParameterArgumentTypes.h"
#include "fhiclcpp/types/detail/ParameterBase.h"
#include "fhiclcpp/types/detail/ParameterMetadata.h"
#include "fhiclcpp/types/detail/ParameterReferenceRegistry.h"
#include "fhiclcpp/types/detail/ParameterRegistrySentry.h"
#include "fhiclcpp/types/detail/ostream_helpers.h"
#include "fhiclcpp/types/detail/SeqVectorBase.h"
#include "fhiclcpp/types/detail/set_element_keys.h"
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
  class Sequence final : public detail::ParameterBase {
  public:

    using ftype = std::array< tt::fhicl_type <T>, SIZE >;
    using rtype = std::array< tt::return_type<T>, SIZE >;

    explicit Sequence(Key && key, Comment && cmt );
    explicit Sequence(Key && key, Comment && cmt, Sequence<T,SIZE> const& dflt );

    explicit Sequence(Key && key) : Sequence( std::move(key), Comment("") ) {}
    explicit Sequence(Key && key, Sequence<T,SIZE> const & dflt ) : Sequence( std::move(key), Comment(""), dflt ){}
    explicit Sequence(Key && key, Sequence<T,SIZE> const & dflt, Comment && cmt ) : Sequence( std::move(key), std::move(cmt), dflt ){}

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
    Sequence(std::initializer_list<T> const & args);

    Sequence();

    auto operator()() const {

      rtype result = { tt::return_type<T>() };
      std::size_t i{};

      for ( auto const& elem : value_ ) {
        result.at(i++) = elem();
      }

      // FIXME: If the rtype contains a Table<>, then additional keys
      // will be added to the registry whenever 'rtype result;' above
      // is called.  In principle, these should be removed whenever
      // the destructors are called.  But that may not be until the
      // end of some scope in which another registering call is made.
      detail::ParameterReferenceRegistry::instance().clear();

      return result;
    }

    auto operator()(std::size_t i) const {
      auto val = value_.at(i)();
      detail::ParameterReferenceRegistry::instance().clear();
      return val; }

    auto & get_ftype() { return value_; }
    auto const & get_ftype() const { return value_; }

  private:

    ftype value_;

    void finalize_elements()
    {
      std::size_t i{};
      for ( auto & elem : value_ ) {
        set_element_keys( elem.get_ftype(), elem, key(), i++ );
      }

    }

  };

  //==================================================================
  // e.g. Sequence<int> ====> std::vector<int>
  //
  template<typename T>
  class Sequence<T,-1> final : public detail::SeqVectorBase {
  public:

    using ftype = std::vector< tt::fhicl_type <T> >;
    using rtype = std::vector< tt::return_type<T> >;

    explicit Sequence(Key && key, Comment && cmt );
    explicit Sequence(Key && key, Comment && cmt, Sequence<T,-1>  const & dflt );

    explicit Sequence(Key && key) : Sequence( std::move(key), Comment("") ) {}
    explicit Sequence(Key && key, Sequence<T,-1> const & dflt) : Sequence( std::move(key), Comment(""), dflt ){}
    explicit Sequence(Key && key, Sequence<T,-1> const & dflt, Comment && cmt ) : Sequence( std::move(key), std::move(cmt), dflt ){}

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
    Sequence(std::initializer_list<T> const & args);

    Sequence();

    static Sequence<T> empty() {
      Sequence<T> result;
      result.resize_sequence(0ul);
      return result;
    }

    auto operator()() const {
      rtype result;
      for ( auto const& elem : value_ )
        result.push_back( elem() );

      // FIXME: If the rtype contains a Table<>, then additional keys
      // will be added to the registry whenever 'push_back' is called.
      // In principle, these should be removed whenever the
      // destructors are called.  But that may not be until the end of
      // some scope in which another registering call is made.
      detail::ParameterReferenceRegistry::instance().clear();

      return result;
    }
    auto operator()(std::size_t i) const {
      auto val = value_.at(i)();
      detail::ParameterReferenceRegistry::instance().clear();
      return val;
    }

    auto & get_ftype() { return value_; }
    auto const & get_ftype() const { return value_; }

  private:
    ftype value_;

    void finalize_elements()
    {
      std::size_t i{};
      for ( auto & elem : value_ )
        set_element_keys( elem.get_ftype(), elem, key(), i++ );
    }

    // To be used only for reassigning keys when ParameterSet
    // validation is being performed.
    void do_resize_sequence(std::size_t i) override
    {
      value_.clear();
      value_.resize(i);

      finalize_elements();
    }

  };

}

#include "fhiclcpp/types/detail/Sequence.icc"

#endif

// Local variables:
// mode: c++
// End:
