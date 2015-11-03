#ifndef fhicpp_types_detail_remove_stale_keys_h
#define fhicpp_types_detail_remove_stale_keys_h

/*
  ----------------------------
  EXPERT-LEVEL code
  (not to be called by users)
  ----------------------------

*/

#include "fhiclcpp/types/detail/ParameterBase.h"

#include <array>
#include <vector>

namespace fhicl {
  namespace detail {

    inline void remove_stale_key(std::string const& key)
    {
      detail::ParameterSchemaRegistry::instance().erase( key );
    }

    template <typename T>
    void remove_stale_keys(T const& , std::string const& key)
    {
      remove_stale_key(key);
    }

    template <typename T,std::size_t SZ>
    void remove_stale_keys(std::array<T,SZ> const & arr, std::string const & key )
    {
      ParameterSchemaRegistry::instance().erase( key );
      for ( auto const & elem : arr ) {
        remove_stale_keys( elem.get_ftype(), elem.key() );
      }
    }

    template <typename T>
    void remove_stale_keys(std::vector<T> const & vec, std::string const & key)
    {
      ParameterSchemaRegistry::instance().erase( key );
      for ( auto const & elem : vec ) {
        remove_stale_keys( elem.get_ftype(), elem.key() );
      }
    }

    //=================================================================
    template <size_t I, typename ... TYPES>
    typename std::enable_if<(I >= std::tuple_size<std::tuple<TYPES...>>::value)>::type
    remove_stale_tuple_key(std::tuple<TYPES...> const &){}

    template <size_t I, typename ... TYPES>
    typename std::enable_if<(I < std::tuple_size<std::tuple<TYPES...>>::value)>::type
    remove_stale_tuple_key(std::tuple<TYPES...> const &  tuple)
    {
      auto & elem = std::get<I>( tuple );
      remove_stale_keys( elem.get_ftype(), elem.key() );
      remove_stale_tuple_key<I+1>( tuple );
    }

    template <typename ... TYPES>
    void remove_stale_keys(std::tuple<TYPES...> const& tuple, std::string const& key)
    {
      ParameterSchemaRegistry::instance().erase( key );
      remove_stale_tuple_key<0>( tuple );
    }

  }
}

#endif

// Local variables:
// mode: c++
// End:
