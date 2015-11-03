#include "fhiclcpp/types/Name.h"
#include "fhiclcpp/types/detail/ParameterBase.h"
#include "fhiclcpp/types/detail/ParameterSchemaRegistry.h"

#include <iostream>
#include <regex>

namespace fhicl {
  namespace detail {

    using key_string = ParameterSchemaRegistry::key_string;
    using base_ptr   = ParameterSchemaRegistry::base_ptr;
    using ref_pair   = ParameterSchemaRegistry::ref_pair;
    using ref_map    = ParameterSchemaRegistry::ref_map;

    ref_map::const_iterator
    ParameterSchemaRegistry::find_parameter_by_key(key_string const& key)
    {
      auto const & params = instance().parameters_;
      return std::find_if( params.cbegin(), params.cend(),
                           [&key](auto const& p) {
                             return p.first == key;
                           } );
    }

    ref_map
    ParameterSchemaRegistry::get_parameters(ParameterBase const * pb)
    {
      return get_parameters_by_key( pb->key() );
    }

    ref_pair
    ParameterSchemaRegistry::get_parameter_by_key(key_string const& key)
    {
      ref_pair emptyResult;
      for ( auto & keyPtrPr : instance().parameters_ ) {
        if ( keyPtrPr.first == key ) {
          return keyPtrPr;
        }
      }
      return emptyResult;
    }

    ref_map
    ParameterSchemaRegistry::get_child_parameters(ParameterBase const * pb)
    {
      return get_child_parameters_by_key( pb->key() );
    }

    ref_map
    ParameterSchemaRegistry::get_child_parameters_by_key(key_string const& key)
    {
      ref_map result;

      auto const p = instance().get_parameter_by_key(key);
      if (p.second == nullptr) return result;

      par_type const pt = p.second->parameter_type();

      std::regex const r =
        is_sequence(pt) ? std::regex( Name::regex_safe(key) + R"(\[\d+\])") :
        is_table(pt)    ? std::regex( Name::regex_safe(key) + R"(\.\w+)" )  : std::regex();

      for ( auto const & keyPtrPr : instance().get_parameters_by_key(key) ) {
        key_string const k = keyPtrPr.first;
        if ( std::regex_match(k,r) )
          result.emplace_back( keyPtrPr );
      }

      return result;
    }

    ref_map
    ParameterSchemaRegistry::get_parameters_by_key(key_string const& key)
    {
      ref_map result;
      for ( auto const & keyPtrPr : instance().parameters_ ) {
        if ( keyPtrPr.first.find(key) != key_string::npos )
          result.emplace_back( keyPtrPr );
      }
      return result;
    }

    std::vector<key_string>
    ParameterSchemaRegistry::get_parameter_keys(ParameterBase const * pb)
    {
      std::vector<key_string> result;
      for ( auto & keyPtrPr : instance().get_parameters(pb) )
        result.push_back( keyPtrPr.first );
      return result;
    }

    void
    ParameterSchemaRegistry::print_keys()
    {
      std::cout << " Current ParameterSchemaRegistry keys " << std::endl;
      for ( auto const& param : instance().get_parameters_by_key("") )
        std::cout << " " << param.first << std::endl;
    }

    bool ParameterSchemaRegistry::empty() const
    {
      return parameters_.empty();
    }

    void ParameterSchemaRegistry::clear()
    {
      parameters_.clear();
    }

    void
    ParameterSchemaRegistry::emplace(key_string const& key, base_ptr ptr)
    {
      parameters_.emplace_back(key, ptr);
    }

    void
    ParameterSchemaRegistry::erase(key_string const& key)
    {
      auto it = find_parameter_by_key(key);
      if ( it == parameters_.cend() ) return;
      parameters_.erase(it);
    }

  } // detail
} // fhiclcpp
