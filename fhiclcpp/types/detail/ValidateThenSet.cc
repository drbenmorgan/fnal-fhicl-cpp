#include "fhiclcpp/types/detail/AtomBase.h"
#include "fhiclcpp/types/detail/ParameterBase.h"
#include "fhiclcpp/types/detail/PrintAllowedConfiguration.h"
#include "fhiclcpp/types/detail/SequenceBase.h"
#include "fhiclcpp/types/detail/SeqVectorBase.h"
#include "fhiclcpp/types/detail/TableBase.h"
#include "fhiclcpp/types/detail/ValidateThenSet.h"
#include "fhiclcpp/types/detail/validationException.h"

#include <iomanip>
#include <regex>

bool
fhicl::detail::
ValidateThenSet::before_action(ParameterBase* p)
{
  // Check that key exists; allow defaulted or optional keys to be
  // absent.
  std::string const& rkey = p->key();
  std::string const& key  = rkey.substr( rkey.find_first_of(".")+1 );
  if ( !pset_.has_key(key) ) {
    if ( !p->has_default() && !p->is_optional() ) {
      missingParameters_.emplace_back(p);
    }
    return false;
  }
  userKeys_.erase(key);
  return true;
}

void
fhicl::detail::
ValidateThenSet::enter_sequence(SequenceBase* s)
{
  auto* v = dynamic_cast<SeqVectorBase*>(s);
  if ( v == nullptr ) return;

  // If the parameter is an unbounded sequence, we need to resize it
  // so that any nested parameters of the elements can be checked.
  std::string const& rkey = v->key();
  std::string const& key  = rkey.substr( rkey.find_first_of('.')+1 );
  std::regex const r { fhicl::Name::regex_safe(key) + "\\[\\d+\\]" };

  std::size_t const nElems = std::count_if( userKeys_.begin(),
                                            userKeys_.end(),
                                            [&r](auto const& uk){
                                              return std::regex_match(uk, r);
                                            } );
  v->resize_sequence(nElems);
}

void
fhicl::detail::
ValidateThenSet::after_action(ParameterBase* p)
{
  p->set_value(pset_, true);
}

namespace {

  using fhicl::detail::ParameterBase;
  using fhicl::detail::PrintAllowedConfiguration;
  using key_set = std::set<std::string>;

  void
  removeIgnorableKeys( key_set const& ignorable,
                       key_set & extra,
                       std::vector<cet::exempt_ptr<ParameterBase>> & missing )
  {
    for (auto const& key : ignorable) {

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
      auto mit = std::remove_if( missing.begin(), missing.end(),
                                 [&subkey](auto p){
                                   return p->key() == subkey;
                                 } );

      missing.erase(mit, missing.end());
    }
  }

  inline bool show_parents(std::string const& k)
  {
    std::size_t const freq =
      std::count(k.begin(),k.end(),'.') +
      std::count(k.begin(),k.end(),'[');
    return freq > 1;
  }

  std::string fillMissingKeysMsg(std::vector<cet::exempt_ptr<ParameterBase>> const & missingParams)
  {

    if ( missingParams.empty() ) return "";

    std::string const prefix {" - " + std::string(3,' ')};

    std::ostringstream oss;
    oss << "Missing parameters:\n";
    for (auto p : missingParams) {

      // If the key is nested (e.g. pset1.pset2[0] ), show the
      // parents
      PrintAllowedConfiguration pc {oss, show_parents(p->key()), prefix};
      pc(p.get());

    }
    oss << "\n";

    return oss.str();
  }

  std::string
  fillExtraKeysMsg( fhicl::ParameterSet const& pset,
                    std::set<std::string> const& extraKeys )
  {
    if ( extraKeys.empty() ) return "";

    std::ostringstream oss;
    oss << "Unsupported parameters:\n\n";
    for ( auto const & key : extraKeys ) {
      oss << " + "
          << std::setw(30) << std::left << key
          << " [ " << pset.get_src_info( key ) << " ]\n";
    }
    oss << '\n';

    return oss.str();
  }

}

void
fhicl::detail::
ValidateThenSet::check_keys()
{
  removeIgnorableKeys( ignorableKeys_, userKeys_, missingParameters_ );
  std::string errmsg;
  errmsg += fillMissingKeysMsg( missingParameters_ );
  errmsg += fillExtraKeysMsg( pset_, userKeys_ );
  if ( !errmsg.empty() ) {
    throw validationException( errmsg.c_str() );
  }
}
