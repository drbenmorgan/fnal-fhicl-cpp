#include "cetlib/exception.h"
#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/types/detail/validate_ParameterSet.h"
#include "fhiclcpp/types/detail/ParameterBase.h"
#include "fhiclcpp/types/detail/ParameterReferenceRegistry.h"
#include "fhiclcpp/types/detail/print_allowed_configuration.h"
#include "fhiclcpp/types/detail/ostream_helpers.h"
#include "fhiclcpp/types/detail/SeqVectorBase.h"
#include "fhiclcpp/types/detail/validationException.h"
#include "fhiclcpp/types/Name.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <vector>
#include <set>
#include <sstream>
#include <string>

namespace {

  using fhicl::detail::ParameterBase;
  using key_string = std::string;
  using key_set    = std::set<key_string>;

  key_set
  getKeysFromParams( fhicl::detail::ref_map const & params,
                     std::string & parentKey,
                     key_set & seqKeys)
  {
    // Will remove the top-level key since the keys as provided by the
    // ParameterSet do not contain it.
    key_set result;
    for ( auto const & keyPtrPr : params ) {
      std::string const & key = keyPtrPr.first;
      if ( key.find(".") == std::string::npos ) {
        parentKey = key;
        continue;
      }
      else if ( parentKey.empty() ) {
        // FIXME: Not sure why the above 'if' block is not being
        // triggered
        parentKey = key.substr( 0, key.find_first_of('.'));
      }
      result.emplace( key.substr( key.find_first_of('.')+1 ) );

      auto ptr = keyPtrPr.second;
      if ( ptr->parameter_type() == fhicl::par_type::SEQ_VECTOR)
        seqKeys.emplace( key );

    }
    return result;
  }

  void
  validateUserKeys( key_set const & allowed,
                    key_set & user,
                    key_set & missing )
  {
    for ( auto const & akey : allowed ) {
      auto key_it = user.find( akey );
      if ( key_it != user.cend() )
        user.erase( key_it );
      else
        missing.emplace( akey );
    }
  }

  void
  removeIgnorableKeys( key_set const& ignorable,
                       key_set & extra,
                       key_set & missing )
  {
    for ( auto const& key : ignorable ) {

      // If find_first_of returns std::string::npos, then 'pos' will
      // be 0 and the call to substr below will return the full key.
      std::size_t const pos    = key.find_first_of('.')+1ul;
      std::string const subkey = key.substr( pos );

      // Allow erasure globbing for extra keys (if "parameter" is an
      // ignorable key, then "parameter.a" is also ignorable)
      auto it = extra.find( subkey );
      if ( it != extra.cend() ) {
        auto match = [&subkey](std::string const& key){ return key.find(subkey) == 0ul; };
        auto const end = std::find_if_not( it, extra.cend(), match );
        extra.erase( it, end );
      }

      // Since all ignorable missing keys are set explicitly, we do
      // not glob erasures.
      missing.erase( subkey );
    }
  }

  void
  adjustSeqElems ( key_set const& userKeys,
                   key_set const& seqKeys,
                   key_set const& missingKeys )
  {
    for ( auto & raw_key : seqKeys ) {

      std::string const& key = raw_key.substr( raw_key.find_first_of('.')+1 );

      // Don't adjust the sequence elements if the key is missing!
      if ( missingKeys.find(key) != missingKeys.cend() )
        continue;

      auto & registry = fhicl::detail::ParameterReferenceRegistry::instance();

      std::regex const r( fhicl::Name::regex_safe(key) + "\\[\\d+\\]" );

      std::size_t nElems{};
      for( auto const& uk : userKeys ) {
        if ( std::regex_match( uk, r ) )
          ++nElems;
      }
      auto base_ptr = registry.get_parameter_by_key( raw_key ).second;
      auto ptr      = dynamic_cast<fhicl::detail::SeqVectorBase*>( base_ptr.get() );
      if ( ptr ) ptr->resize_sequence( nElems );
    }

  }

}

//=========================================================================================

/*
  The validation process is as follows:

  - Get all user-provided configuration keys.

  - Retrieve all ignorable keys (including those with defaults)
    vis-a-vis the allowed configuration.

  - Loop through all allowed keys, erasing the corresponding element
    on the user-provided key set if it is present, and adding the key
    to the missing key set if it is not.

  - If the user key list is non-empty after the validation procedure,
    then the existing elements of that list correspond to extra keys
    that are not supported.
*/

void
fhicl::detail::
validate_ParameterSet( base_ptr param,
                       ParameterSet const& pset,
                       key_set const& keysToIgnore)
{
  auto providedKeys = pset.get_all_keys();
  key_set userKeys( providedKeys.begin(), providedKeys.end() );

  auto & registry = detail::ParameterReferenceRegistry::instance();

  key_set ignorableKeys(keysToIgnore);
  registry.get_keys_with_defaults(&*param, ignorableKeys);

  key_set missingKeys;
  validate( registry.get_child_parameters(&*param),
            ignorableKeys,
            userKeys,
            missingKeys);

  std::string errmsg;
  errmsg += fillMissingKeysMsg( param, missingKeys );
  errmsg += fillExtraKeysMsg( pset, userKeys );
  if ( errmsg.size() ) {
    registry.clear();
    throw validationException( errmsg.c_str() );
  }

  registry.set_values( pset, true );
  registry.clear();
}

//=========================================================================================
void
fhicl::detail::
validate(fhicl::detail::ref_map const & declaredParams,
         key_set const & ignorableKeys,
         key_set & userKeys,
         key_set & missingKeys)
{

  key_string parentKey;
  key_set seqParams;
  key_set const allowedKeys = getKeysFromParams( declaredParams, parentKey, seqParams );

  validateUserKeys( allowedKeys, userKeys, missingKeys );

  adjustSeqElems( userKeys, seqParams, missingKeys );

  for ( auto const& key : allowedKeys ) {

    // Don't look up the nested keys for top-level keys that are
    // already missing.
    if ( missingKeys.find(key) != missingKeys.end() ) continue;
    validate( ParameterReferenceRegistry::get_child_parameters_by_key( parentKey+std::string(".")+key ),
              ignorableKeys,
              userKeys,
              missingKeys );
  }

  removeIgnorableKeys( ignorableKeys, userKeys, missingKeys );

}

std::string
fhicl::detail::fillMissingKeysMsg( base_ptr param, key_set const & missingKeys )
{

  if ( missingKeys.empty() ) return "";

  std::string const prefix = " - ";

  std::ostringstream oss;
  oss << "Missing parameters:\n";
  for ( auto const & key : missingKeys ) {

    // If the key is nested (e.g. pset1.pset2[0] ), show the
    // parents
    bool const showParents = key.find_first_of(".[") != std::string::npos;
    print_allowed_configuration( param->key()+"."+key,
                                 oss,
                                 showParents,
                                 prefix );
  }
  oss << "\n";

  return oss.str();
}

std::string
fhicl::detail::fillExtraKeysMsg( fhicl::ParameterSet const& pset, key_set const& extraKeys )
{
  if ( extraKeys.empty() ) return "";

  std::ostringstream oss;
  oss << "Unsupported parameters:\n\n";
  for ( auto const & key : extraKeys )
    oss << " + " << std::setw(30) << std::left << key
        << " [ "
        << pset.get_src_info( key )
        << " ]"
        << std::endl;
  oss << '\n';

  return oss.str();
}

// Local variables:
// mode: c++
// End:
