#ifndef fhiclcpp_ParameterSet_h
#define fhiclcpp_ParameterSet_h

// ======================================================================
//
// ParameterSet
//
// ======================================================================

#include "boost/any.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/numeric/conversion/cast.hpp"
#include "cetlib/demangle.h"
#include "cetlib/split_by_regex.h"
#include "fhiclcpp/ParameterSetID.h"
#include "fhiclcpp/coding.h"
#include "fhiclcpp/detail/ParameterSetWalker.h"
#include "fhiclcpp/detail/deprecation_msgs.h"
#include "fhiclcpp/detail/encode_extended_value.h"
#include "fhiclcpp/detail/get_names.h"
#include "fhiclcpp/detail/print_mode.h"
#include "fhiclcpp/detail/try_blocks.h"
#include "fhiclcpp/exception.h"
#include "fhiclcpp/extended_value.h"
#include "fhiclcpp/fwd.h"

#include <cctype>
#include <regex>
#include <map>
#include <sstream>
#include <string>
#include <typeinfo>
#include <unordered_map>
#include <vector>

// ----------------------------------------------------------------------

class fhicl::ParameterSet {
public:

  using ps_atom_t     = fhicl::detail::ps_atom_t;
  using ps_sequence_t = fhicl::detail::ps_sequence_t;
  using annot_t       = std::unordered_map<std::string,std::string>;

  // compiler generates default c'tor, d'tor, copy c'tor, copy assignment

  // observers:
  bool is_empty() const;
  ParameterSetID id() const;

  std::string to_string() const;
  std::string to_compact_string() const;

  std::string to_indented_string() const;
  std::string to_indented_string(unsigned initial_indent_level) const;
  std::string to_indented_string(unsigned initial_indent_level, bool annotate) const;
  std::string to_indented_string(unsigned initial_indent_level, detail::print_mode pm) const;

  std::vector<std::string> get_names() const;
  std::vector<std::string> get_pset_names() const;

  [[deprecated(GET_KEYS_MSG)     ]] std::vector<std::string> get_keys()      const { return get_names();      }
  [[deprecated(GET_PSET_KEYS_MSG)]] std::vector<std::string> get_pset_keys() const { return get_pset_names(); }

  std::vector<std::string> get_all_keys() const;
  // Key must be local to this parameter set: no nesting.
  bool is_key_to_table(std::string const & key) const;
  bool is_key_to_sequence(std::string const & key) const;
  bool is_key_to_atom(std::string const & key) const;

  // retrievers (nested key OK):
  bool has_key(std::string const & key) const;
  template< class T >
  bool get_if_present(std::string const & key, T & value) const;
  template< class T, class Via >
  bool get_if_present(std::string const & key, T & value
                      , T convert(Via const &)) const;
  template< class T >
  T get(std::string const & key) const;
  template< class T, class Via >
  T get(std::string const & key
        , T convert(Via const &)) const;
  template< class T >
  T get(std::string const & key, T const & default_value) const;
  template< class T, class Via >
  T get(std::string const & key, T const & default_value
        , T convert(Via const &)) const;

  std::string get_src_info(std::string const& key) const;

  // inserters (key must be local: no nesting):
  void put(std::string const & key); // Implicit nil value.
  template< class T > // Fail on preexisting key.
  void put(std::string const & key, T const & value);
  void put_or_replace(std::string const & key); // Implicit nil value.
  template< class T > // Succeed.
  void put_or_replace(std::string const & key, T const & value);
  template< class T > // Fail if preexisting key of incompatible type.
  void put_or_replace_compatible(std::string const & key, T const & value);

  // deleters:
  bool erase(std::string const & key);

  // comparators:
  bool operator == (ParameterSet const & other) const;
  bool operator != (ParameterSet const & other) const;

private:
  using map_t      = std::map<std::string, boost::any>;
  using map_iter_t = map_t::const_iterator;

  map_t mapping_;
  annot_t srcMapping_;
  mutable ParameterSetID id_;

  // Private inserters.
  void insert_(std::string const & key, boost::any const & value);
  void insert_or_replace_(std::string const & key, boost::any const & value);
  void insert_or_replace_compatible_(std::string const & key,
                                     boost::any const & value);

  std::string to_string_(bool compact = false) const;
  std::string stringify_(boost::any const & a,
                         bool compact = false) const;

  void assemble_(boost::any const & a,
                 std::string const& key,
                 std::vector<std::string> & keys ) const;

 void assemble_table_(boost::any const & a,
                      std::string const& key,
                      std::vector<std::string> & keys ) const;

  void assemble_sequence_(boost::any const & a,
                          std::string const& key,
                          std::vector<std::string> & keys ) const;

  // Local retrieval only.
  template< class T >
  bool get_one_(std::string const & key, T & value) const;

  bool find_one_(std::string const & key) const;

  bool
  key_is_type_(std::string const & key,
               std::function<bool (boost::any const &)> func) const;

  void walk_( detail::ParameterSetWalker& psw) const;

}; // ParameterSet

// ======================================================================

inline
std::string
fhicl::ParameterSet::
to_string() const
{
  return to_string_();
}

inline
std::string
fhicl::ParameterSet::
to_compact_string() const
{
  return to_string_(true);
}

inline
bool
fhicl::ParameterSet::
is_key_to_table(std::string const & key) const
{
  return key_is_type_(key, &detail::is_table);
}

inline
bool
fhicl::ParameterSet::
is_key_to_sequence(std::string const & key) const
{
  return key_is_type_(key, &detail::is_sequence);
}

inline
bool
fhicl::ParameterSet::
is_key_to_atom(std::string const & key) const
{
  return key_is_type_(key, [](boost::any const & a) {
    return !(detail::is_sequence(a) ||
             detail::is_table(a));
  });
}

template< class T >
void
fhicl::ParameterSet::put(std::string const & key, T const & value){

  auto insert = [this,&key,&value](){
    using detail::encode;
    insert_(key, boost::any(encode(value)));
  };

  detail::try_insert(insert, key);

}

template< class T >
void
fhicl::ParameterSet::put_or_replace(std::string const & key, T const & value)
{

  auto insert_or_replace = [this,&key,&value](){
    using detail::encode;
    insert_or_replace_(key, boost::any(encode(value)));
    srcMapping_.erase(key);
  };

  detail::try_insert(insert_or_replace, key);

}

template< class T >
void
fhicl::ParameterSet::put_or_replace_compatible(std::string const & key,
                                               T const & value)
{

  auto insert_or_replace_compatible = [this,&key,&value](){
    using detail::encode;
    insert_or_replace_compatible_(key, boost::any(encode(value)));
    srcMapping_.erase(key);
  };

  detail::try_insert(insert_or_replace_compatible, key);

}

// ----------------------------------------------------------------------

template< class T >
bool
fhicl::ParameterSet::get_if_present(std::string const & key,
                                    T & value) const
{
  auto keys = detail::get_names(key);

  ParameterSet const* p {this};
  ParameterSet pset;

  for (auto const& table : keys.tables()) {
    if (!p->get_one_(table, pset)){
      return false;
    }
    p = &pset;
  }
  return p->get_one_(keys.last(), value);
} // get_if_present<>()

template< class T, class Via >
bool
fhicl::ParameterSet::get_if_present(std::string const & key,
                                    T & result,
                                    T convert(Via const &)) const
{
  Via go_between;
  if (!get_if_present(key, go_between))
  { return false; }
  result = convert(go_between);
  return true;
} // get_if_present<>()

template< class T >
T
fhicl::ParameterSet::get(std::string const & key) const
{
  T result;
  return get_if_present(key, result) ? result
         : throw fhicl::exception(cant_find, key);
}

template< class T, class Via >
T
fhicl::ParameterSet::get(std::string const & key,
                         T convert(Via const &) )const
{
  T result;
  return get_if_present(key, result, convert)
         ? result
         : throw fhicl::exception(cant_find, key);
}

template< class T >
T
fhicl::ParameterSet::get(std::string const & key,
                         T const & default_value) const
{
  T result;
  return get_if_present(key, result) ? result
         : default_value;
}

template< class T, class Via >
T
fhicl::ParameterSet::get(std::string const & key,
                         T const & default_value,
                         T convert(Via const &) ) const
{
  T result;
  return get_if_present(key, result, convert)
         ? result
         : default_value;
}

// ----------------------------------------------------------------------

inline bool
fhicl::ParameterSet::operator == (ParameterSet const & other) const
{
  return id() == other.id();
}

inline bool
fhicl::ParameterSet::operator != (ParameterSet const & other) const
{
  return ! operator==(other);
}

// ----------------------------------------------------------------------

namespace fhicl {
  namespace detail {

    using cit_size_t = std::vector<std::size_t>::const_iterator;

    template< class T >
    void
    get_parameter(cit_size_t const cend,
                  cit_size_t const subscript_it,
                  boost::any const a,
                  T & value)
    {
      if ( subscript_it == cend ){
        decode(a,value);
        return;
      }

      auto const seq = boost::any_cast<ps_sequence_t>(a);
      get_parameter( cend, std::next(subscript_it), seq[*subscript_it], value );

    }

  }
}

template< class T >
bool
fhicl::ParameterSet::get_one_(std::string const & key,
                              T & value) const
try
{
  using detail::decode;

  // Replace (e.g.) "name[2]" with "name,2", then split by ','
  static std::regex const r("\\[(\\d+)\\]");
  auto tokens = cet::split_by_regex( std::regex_replace(key, r, ",$1" ), ",");

  std::string const name = tokens.front();

  map_iter_t it = mapping_.find(name);
  if (it == mapping_.end()) {
    return false;
  }

  std::vector<std::size_t> seq_indices;
  std::for_each( tokens.cbegin()+1, tokens.cend(),
                 [&seq_indices](std::string const& str_ind)
                 { seq_indices.push_back( std::stoul(str_ind) ); }
                 );

  detail::get_parameter( seq_indices.cend(),
                         seq_indices.cbegin(),
                         it->second,
                         value );

  return true;
}
catch (fhicl::exception const & e)
{
  std::ostringstream errmsg;
  errmsg << "\nUnsuccessful attempt to convert FHiCL parameter '"
         << key
         << "' to type '"
         << cet::demangle_symbol( typeid(value).name() ) << "'.\n\n"
         << "[Specific error:]";
  throw fhicl::exception(type_mismatch, errmsg.str(), e);
}
catch (std::exception const & e)
{
  std::ostringstream errmsg;
  errmsg << "\nUnsuccessful attempt to convert FHiCL parameter '"
         << key
         << "' to type '"
         << cet::demangle_symbol( typeid(value).name() ) << "'.\n\n"
         << "[Specific error:]\n" << e.what() << "\n\n";
  throw fhicl::exception(type_mismatch, errmsg.str() );
}

// ----------------------------------------------------------------------

namespace fhicl {

  template<>
  void
  ParameterSet::put(std::string const & key, fhicl::extended_value const & value);

}

// ======================================================================

#endif /* fhiclcpp_ParameterSet_h */

// Local Variables:
// mode: c++
// End:
