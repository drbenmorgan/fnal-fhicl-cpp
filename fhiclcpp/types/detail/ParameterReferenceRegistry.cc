#include "fhiclcpp/types/Key.h"
#include "fhiclcpp/types/detail/AtomBase.h"
#include "fhiclcpp/types/detail/ParameterReferenceRegistry.h"
#include "fhiclcpp/exception.h"

#include <iostream>
#include <regex>

namespace fhicl {
  namespace detail {

    using key_string = ParameterReferenceRegistry::key_string;
    using base_ptr   = ParameterReferenceRegistry::base_ptr;
    using ref_pair   = ParameterReferenceRegistry::ref_pair;
    using ref_map    = ParameterReferenceRegistry::ref_map;

    ref_map::const_iterator
    ParameterReferenceRegistry::find_parameter_by_key(key_string const& key)
    {
      auto const & params = instance().parameters_;
      for ( auto i = params.cbegin(), e = params.cend(); i!=e; ++i ) {
        if ( i->first == key ) return i;
      }
      return params.cend();
    }

    ref_map
    ParameterReferenceRegistry::get_parameters(ParameterBase const * pb)
    {
      return get_parameters_by_key( pb->key() );
    }

    ref_pair
    ParameterReferenceRegistry::get_parameter_by_key(key_string const& key)
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
    ParameterReferenceRegistry::get_child_parameters(ParameterBase const * pb)
    {
      return get_child_parameters_by_key( pb->key() );
    }

    ref_map
    ParameterReferenceRegistry::get_child_parameters_by_key(key_string const& key)
    {
      ref_map result;

      par_type const pt = instance().get_parameter_by_key(key).second->parameter_type();

      std::regex const r =
        is_sequence(pt) ? std::regex( Key::regex_safe(key) + R"(\[\d+\])") :
        is_table(pt)    ? std::regex( Key::regex_safe(key) + R"(\.\w+)" )  : std::regex();

      for ( auto const & keyPtrPr : instance().get_parameters_by_key(key) ) {
        key_string const k = keyPtrPr.first;
        if ( std::regex_match(k,r) )
          result.emplace_back( keyPtrPr );
      }

      return result;
    }

    ref_map
    ParameterReferenceRegistry::get_parameters_by_key(key_string const& key)
    {
      ref_map result;
      for ( auto const & keyPtrPr : instance().parameters_ ) {
        if ( keyPtrPr.first.find(key) != key_string::npos )
          result.emplace_back( keyPtrPr );
      }
      return result;
    }

    std::vector<key_string>
    ParameterReferenceRegistry::get_parameter_keys(ParameterBase const * pb)
    {
      std::vector<key_string> result;
      for ( auto & keyPtrPr : instance().get_parameters(pb) )
        result.push_back( keyPtrPr.first );
      return result;
    }


    void
    ParameterReferenceRegistry::get_keys_with_defaults(ParameterBase const * pb,
                                                       std::set<key_string> & result)
    {
      for ( auto & keyPtrPr : instance().get_parameters(pb) ) {
        auto const& key = keyPtrPr.first;
        auto pbPtr = &*keyPtrPr.second;

        auto children = instance().get_child_parameters( pbPtr );

        // Consider this case:
        //
        //    pset1: {
        //
        //      value : something
        //
        //      pset2: {
        //        pset3: {
        //          value : else   # value has a default
        //          foo   : bar    # foo also has a default
        //        }
        //      }
        //    }
        //
        // In this example, since all names in 'pset3' have defaults,
        // it would be an error to require that 'pset2' and 'pset3'
        // also be specified.  The following code therefore adds
        // 'pset1.pset2' and 'pset1.pset2.pset3' as defaulted keys if
        // all of its members have defaults.
        //
        // This check is made by counting the number of keys whose
        // corresponding children parameters have default values.  If
        // that number is equal to the number of children, then the
        // containing table's default flag is set to 'true'.

        std::size_t nDflts{};
        for ( auto const & child : instance().get_child_parameters( pbPtr ) ) {
          auto childPtr = &*child.second;
          get_keys_with_defaults( childPtr, result );
          if ( childPtr->has_default() ) ++nDflts;
        }

        if ( nDflts == children.size() && children.size() )
          pbPtr->set_default_flag(true);

        if ( pbPtr->has_default() )
          result.emplace( key );

      }
    }

    void
    ParameterReferenceRegistry::print_keys()
    {
      std::cout << " Current ParameterReferenceRegistry keys " << std::endl;
      for ( auto const& param : instance().get_parameters_by_key("") )
        std::cout << " " << param.first << std::endl;
    }

    bool ParameterReferenceRegistry::empty() const
    {
      return parameters_.empty();
    }

    void ParameterReferenceRegistry::clear()
    {
      parameters_.clear();
    }

    void
    ParameterReferenceRegistry::emplace(key_string const& key, base_ptr ptr)
    {
      parameters_.emplace_back(key, ptr);
    }

    void
    ParameterReferenceRegistry::erase(key_string const& key)
    {
      auto it = find_parameter_by_key(key);
      if ( it == parameters_.cend() ) return;
      parameters_.erase(it);
    }

    void
    ParameterReferenceRegistry::set_values(fhicl::ParameterSet const& pset,
                                           bool const trimParent)
    {
      for ( auto & keyPtrPr : parameters_ ) {

        if ( keyPtrPr.second->parameter_type() != par_type::ATOM )
          continue;

        auto atom_ptr = dynamic_cast<AtomBase*>( keyPtrPr.second.get() );
        if ( !atom_ptr ) {
          std::ostringstream err_msg;
          err_msg << "Error when converting from ParameterBase* to AtomBase*\n"
                  << "in " << __func__ << "\n"
                  << "Please contact artists@fnal.gov";
          throw fhicl::exception(fhicl::error::cant_happen, err_msg.str());
        }

        atom_ptr->set_value(pset, trimParent);
      }
    }

  } // detail
} // fhiclcpp
