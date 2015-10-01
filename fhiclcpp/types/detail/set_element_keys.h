#ifndef fhicpp_types_detail_set_element_keys_h
#define fhicpp_types_detail_set_element_keys_h

/*
  ----------------------------
  EXPERT-LEVEL code
  (not to be called by users)
  ----------------------------
*/

#include "fhiclcpp/types/Name.h"
#include "fhiclcpp/types/detail/ParameterBase.h"
#include "fhiclcpp/types/detail/remove_stale_keys.h"

#include <array>
#include <vector>

namespace fhicl {
  namespace detail {

    template <typename T>
    void set_element_keys(T& t,
                          ParameterBase & pb,
                          std::string const& key_prefix,
                          std::size_t const i)
    {
      auto & registry = ParameterReferenceRegistry::instance();
      std::string const old_key = pb.key();
      std::string const new_key = key_prefix + Name::array_index(i);

      remove_stale_keys( t, pb.key() );
      pb.set_key( new_key );
      registry.emplace( pb.key(), &pb );

      /*
        Tables are special.  It is not possible to descend into the
        individual member parameters and adjust their respective keys.
        We must retrieve them manually through the registry interface.
      */
      if ( is_table( pb.parameter_type() ) ) {

        std::string const key_format = Name::regex_safe( old_key ) + std::string("(.*)");
        std::string const new_format = new_key + std::string("$1");

        for ( auto const & pr : registry.get_parameters_by_key( old_key ) ) {
          remove_stale_key( pr.first );
          pr.second->set_key( std::regex_replace( pr.second->key(),
                                                  std::regex( key_format ),
                                                  new_format ) );
          registry.emplace( pr.second->key(), pr.second );
        }
      }

    }

    template <typename T,std::size_t SZ>
    void set_element_keys(std::array<T,SZ> & arr,
                          ParameterBase & pb,
                          std::string const& key_prefix,
                          std::size_t const i)
    {
      auto & registry = ParameterReferenceRegistry::instance();
      std::string const new_key = key_prefix + Name::array_index(i);

      remove_stale_keys( arr, pb.key() );
      pb.set_key( new_key );
      registry.emplace( pb.key(), &pb );
      std::size_t j{};
      for ( auto & elem : arr ) {
        set_element_keys(elem.get_ftype(), elem, new_key,j++);
      }
    }

    template <typename T>
    void set_element_keys(std::vector<T> & vec,
                          ParameterBase & pb,
                          std::string const& key_prefix,
                          std::size_t const i)
    {
      auto & registry = ParameterReferenceRegistry::instance();

      std::string const new_key = key_prefix + Name::array_index(i);
      remove_stale_keys( vec, pb.key() );
      pb.set_key( new_key );
      registry.emplace( pb.key(), &pb );
      std::size_t j{};
      for ( auto & elem : vec ) {
        set_element_keys(elem.get_ftype(), elem, new_key,j++);
      }
    }

    template <size_t I, typename ... TYPES>
    typename std::enable_if<(I >= std::tuple_size<std::tuple<TYPES...> >::value)>::type
    set_tuple_element_key(std::tuple<TYPES...> &,
                          ParameterBase&,
                          std::string const&){}

    template <size_t I, typename ... TYPES>
    typename std::enable_if<(I < std::tuple_size<std::tuple<TYPES...> >::value)>::type
    set_tuple_element_key(std::tuple<TYPES...> & tuple,
                          ParameterBase & pb,
                          std::string const& key_prefix)
    {
      auto & elem = std::get<I>(tuple);
      set_element_keys( elem.get_ftype(), elem, key_prefix, I );
      set_tuple_element_key<I+1>(tuple, pb, key_prefix);
    }

    template <typename ... TYPES>
    void set_element_keys(std::tuple<TYPES...> & tuple,
                          ParameterBase & pb,
                          std::string const& key_prefix,
                          std::size_t const i)
    {
      auto & registry = ParameterReferenceRegistry::instance();
      std::string const new_key = key_prefix + Name::array_index(i);
      remove_stale_keys( tuple, pb.key() );
      pb.set_key( new_key );
      registry.emplace( pb.key(), &pb );
      set_tuple_element_key<0>( tuple, pb, new_key);
    }

  }
}

#endif

// Local variables:
// mode: c++
// End:
