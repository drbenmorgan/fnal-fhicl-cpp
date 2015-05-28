#include "cetlib/split_by_regex.h"
#include "fhiclcpp/exception.h"
#include "fhiclcpp/types/Key.h"
#include "fhiclcpp/types/detail/AtomBase.h"
#include "fhiclcpp/types/detail/ParameterBase.h"
#include "fhiclcpp/types/detail/ParameterArgumentTypes.h"
#include "fhiclcpp/types/detail/ParameterReferenceRegistry.h"
#include "fhiclcpp/types/detail/print_reference.h"

#include <cctype>
#include <regex>
#include <vector>

//===================================================================================
namespace {

  using fhicl::detail::ParameterBase;
  using base_ptr = cet::exempt_ptr<ParameterBase>;
  using namespace std::string_literals;

  inline std::string name(std::string const& key)
  {
    return std::regex_replace(key,std::regex(".*\\.(?!\\.)"), "$1" );
  }

  inline bool is_sequence_element(base_ptr param)
  {
    auto pos = param->key().find_last_of("]");
    return pos != std::string::npos && pos == param->key().size()-1;
  }

  struct Indent {

    Indent(std::string const& key, std::string const& prfx = "")
      : nIndents( std::count( key.begin(), key.end(), '.' ) +
                  std::count( key.begin(), key.end(), '[' ) )
      , prefix( prfx )
    {}

    std::size_t nIndents;
    std::string prefix;
  };

  std::ostream& operator<<(std::ostream& os, Indent && ind)
  {
    std::size_t n = ind.nIndents;
    os << ind.prefix;
    while ( --n < -1ul ){
      os << std::string(3,' ');
    }
    return os;
  }

  struct maybeName {

    maybeName(base_ptr param, std::string const& k, std::string const& prfx = "")
      : is_seq_elem( is_sequence_element(param) )
      , key( k )
      , prefix( prfx )
      , nIndents( std::count( key.begin(), key.end(), '.' ) +
                  std::count( key.begin(), key.end(), '[' ) )
    {}

    bool is_seq_elem;
    std::string key;
    std::string prefix;
    std::size_t nIndents;

  };

  std::ostream& operator<<(std::ostream& os, maybeName && mn)
  {

    if ( !mn.is_seq_elem ) {
      os << Indent(mn.key, mn.prefix) << name(mn.key) << " : ";
    }
    else
      os << Indent(mn.key,mn.prefix);

    return os;
  }

  std::regex const r_index("\\[(\\d+)\\]");

  struct MaybeParentSentry {

    MaybeParentSentry(std::ostream& os, base_ptr ptr, std::string const& k, bool const showParents )
      : os_(os)
      , show(showParents)
      , prefix(6,' ') // No prefix (just spaces), since prefixes have been validated.
      , keys( showParents ? get_parents(k) : std::vector<std::string>{} )
      , closingBraces()
    {
      if ( !is_sequence_element(ptr) )
        os_ << "\n";

      if ( show ) {

        std::size_t i{};

        for ( auto it = keys.begin(), e = keys.end()-1; it != e ; ++it ) {
          std::string const& key      = *it;
          std::string const& next_key = *std::next(it);
          if ( !std::isdigit( key[0] ) ) {
            if ( std::isdigit( next_key[0] ) ) {
              os_ << prefix << std::string(3*i,' ') << key << ": [  # index: " << next_key << "\n";
              closingBraces.push_back("\n"s + prefix + std::string(3*i,' ')+"]"s );
            }
            else {
              os_ << prefix << std::string(3*i,' ') << key << ": {\n";
              closingBraces.push_back("\n"s + prefix + std::string(3*i,' ')+"}"s );
            }
          }
          else {
            if ( !std::isdigit( next_key[0] ) ) {
              os_ << prefix << std::string(3*i,' ') << "{\n";
              closingBraces.push_back("\n"s + prefix + std::string(3*i,' ')+"}"s );
            }
            else {
              os_ << prefix << std::string(3*i,' ') << "[  # index: " << key << '\n';
              closingBraces.push_back("\n"s + prefix + std::string(3*i,' ')+"]"s );
            }
          }
          ++i;
        }
      }
      else {}

    }

    ~MaybeParentSentry()
    {
      if ( show )
        for ( auto it = closingBraces.crbegin(), re = closingBraces.crend(); it != re ; ++it )
          os_ << *it;
    }

    std::ostream & os_;
    bool newline;
    bool show;
    std::string prefix;
    std::vector<std::string> keys;
    std::vector<std::string> closingBraces;

  private:

    std::vector<std::string> get_parents(std::string const& k){

      std::vector<std::string> parents;
      for ( auto const& key : cet::split_by_regex( k, "\\." ) )
        for ( auto const& token : cet::split_by_regex( std::regex_replace(key, r_index, ",$1" ), ",") )
          parents.push_back( token );

      // Skip first key, which is something like "<0>" in the context
      // whenever this function is called.
      return std::vector<std::string>( parents.begin()+1, parents.end() );
    }

  };

}

//===================================================================================
void
fhicl::detail::
print_reference(std::string const& key,
                std::ostream & os,
                bool const showParents,
                std::string const& prefix)
{
  auto const & param = detail::ParameterReferenceRegistry::get_parameter_by_key( key );
  if ( param.second ) {
    stringify( param.second,
               os,
               showParents,
               prefix );
  }
  else {
    os << " No configuration info for: " << key << "\n";
  }
}

//===================================================================================
void
fhicl::detail::
stringify( base_ptr param, std::ostream & os,
           bool const showParents,
           std::string const& prefix,
           std::string const& suffix)
{

  if ( !param->comment().empty() ) {
    os << "\n";
    for ( auto const & line : cet::split_by_regex( param->comment(), "\n" ) )
      os << Indent(param->key(), prefix)  << "# " << line << "\n";
  }

  par_type const pt = param->parameter_type();

  if      ( pt == par_type::TABLE      ) stringify_table ( param, os, showParents, prefix );
  else if ( pt == par_type::SEQ_VECTOR ) stringify_vector( param, os, showParents, prefix );
  else if ( pt == par_type::SEQ_ARRAY  ) stringify_array ( param, os, showParents, prefix );
  else if ( pt == par_type::TUPLE      ) stringify_tuple ( param, os, showParents, prefix );
  else stringify_atom( param, os, showParents, prefix );

  os << suffix;
  if ( param->has_default() && pt == par_type::ATOM )
    os << "  # default";
  os << "\n";
}

//===================================================================================
void
fhicl::detail::
stringify_atom(base_ptr param, std::ostream & os, bool const showParents, std::string const& prefix)
{
  std::string const& key = param->key();

  MaybeParentSentry mp( os, param, key, showParents );

  auto atom_ptr = dynamic_cast<AtomBase*>( param.get() );
  if ( !atom_ptr ){
    std::ostringstream err_msg;
    err_msg << "Error when converting from ParameterBase* to AtomBase*\n"
            << "in " << __func__ << "\n"
            << "Please contact artists@fnal.gov";
    throw fhicl::exception(fhicl::error::cant_happen, err_msg.str() );
  }

  os << maybeName( param, key, prefix )
     << atom_ptr->stringified_value();
}

//===================================================================================
void
fhicl::detail::
stringify_array(base_ptr param, std::ostream& os, bool const showParents, std::string const& prefix)
{
  std::string const& key = param->key();

  MaybeParentSentry mp( os, param, key, showParents );

  os << maybeName( param, key, prefix ) << "[\n";

  std::size_t i{};
  auto const & children = ParameterReferenceRegistry::get_child_parameters_by_key(key);
  for ( auto const& child : children ) {
    auto suffix = (++i != children.size() && children.size() > 1ul) ? "," : "";
    stringify( child.second, os, false, prefix, suffix );
  }

  os << Indent(key, prefix) << "]";
}

//===================================================================================
void
fhicl::detail::
stringify_table(base_ptr param, std::ostream& os, bool const showParents, std::string const& prefix)
{
  std::string const& key = param->key();

  MaybeParentSentry mp( os, param, key, showParents );
  os << maybeName( param, key, prefix ) << "{\n";

  auto const & children = ParameterReferenceRegistry::get_child_parameters_by_key( key );
  for ( auto const& child : children )
    stringify( child.second, os, false, prefix );

  os << Indent(key, prefix) << "}";
}

//===================================================================================
void
fhicl::detail::
stringify_tuple(base_ptr param, std::ostream & os, bool const showParents, std::string const& prefix)
{
  std::string const& key = param->key();

  MaybeParentSentry mp( os, param, key, showParents );
  os << maybeName( param, key, prefix ) << "[\n";

  std::size_t i{};
  auto const& children = ParameterReferenceRegistry::get_child_parameters_by_key(key);
  for ( auto const& child : children ) {
    auto suffix = (++i != children.size() && children.size() > 1ul) ? "," : "";
    stringify( child.second, os, false, prefix, suffix );
  }

  os << Indent(key, prefix) << "]";
}

//===================================================================================
void
fhicl::detail::
stringify_vector(base_ptr param, std::ostream & os, bool const showParents, std::string const& prefix)
{
  std::string const& key = param->key();

  MaybeParentSentry mp( os, param, key, showParents );
  os << maybeName( param, key, prefix ) << "[\n";

  std::size_t i{};
  auto const & children = ParameterReferenceRegistry::get_child_parameters_by_key( key );
  for ( auto const& child : children ) {
    auto suffix = (++i != children.size() && children.size() > 1ul) ? "," : "";
    if ( i == 1ul && !param->has_default() ) suffix = ",";
    stringify( child.second, os, false, prefix, suffix );
  }
  if ( !param->has_default() )
    os << Indent(key+".", prefix) << "...\n";

  os << Indent(key, prefix) << "]";
}
