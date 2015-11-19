#include "cetlib/container_algorithms.h"
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
    ParameterSchemaRegistry::get_parameters(ParameterBase const* pb)
    {
      return get_parameters_by_key( pb->key() );
    }

    ref_pair
    ParameterSchemaRegistry::get_parameter_by_key(key_string const& key)
    {
      auto it = instance().find_parameter_by_key(key);
      return it != instance().parameters_.end() ? *it : ref_pair{};
    }

    ref_map
    ParameterSchemaRegistry::get_child_parameters(ParameterBase const* pb)
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
        is_sequence(pt) ? std::regex{Name::regex_safe(key) + R"(\[\d+\])"} :
        is_table(pt)    ? std::regex{Name::regex_safe(key) + R"(\.\w+)" }  : std::regex{};

      cet::copy_if_all( instance().get_parameters_by_key(key),
                        std::inserter(result, result.begin()),
                        [&r](auto const& pr) {
                          return std::regex_match(pr.first, r);
                        } );
      return result;
    }

    ref_map
    ParameterSchemaRegistry::get_parameters_by_key(key_string const& key)
    {
      ref_map result;
      cet::copy_if_all( instance().parameters_,
                        std::inserter(result, result.begin()),
                        [&key](auto const & pr) {
                          return pr.first.find(key) != key_string::npos;
                        } );
      return result;
    }

    std::vector<key_string>
    ParameterSchemaRegistry::get_parameter_keys(ParameterBase const * pb)
    {
      std::vector<key_string> result;
      cet::transform_all( instance().get_parameters(pb),
                          std::back_inserter(result),
                          [](auto const& p) {
                            return p.first;
                          } );
      return result;
    }

    void
    ParameterSchemaRegistry::print_keys()
    {
      std::cout << " Current ParameterSchemaRegistry keys " << std::endl;
      cet::transform_all( instance().get_parameters_by_key(""),
                          std::ostream_iterator<std::string>(std::cout,"\n"),
                          [](auto const& p) {
                            return " "+p.first;
                          } );
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
    ParameterSchemaRegistry::emplace(ParameterBase* p)
    {
      parameters_.emplace_back(p->key(), p);
    }

    void
    ParameterSchemaRegistry::update_parameter(ParameterBase& pb, std::string const& key)
    {
      auto p = ptr_to_base(pb);
      p->set_key(key);
      parameters_.emplace_back(p->key(), p);
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
