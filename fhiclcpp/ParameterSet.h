#ifndef fhiclcpp_ParameterSet_h
#define fhiclcpp_ParameterSet_h

// ======================================================================
//
// ParameterSet
//
// ======================================================================

#include "boost/algorithm/string.hpp"
#include "boost/any.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/numeric/conversion/cast.hpp"
#include "cetlib/demangle.h"
#include "cpp0x/string"
#include "fhiclcpp/ParameterSetID.h"
#include "fhiclcpp/coding.h"
#include "fhiclcpp/detail/encode_extended_value.h"
#include "fhiclcpp/exception.h"
#include "fhiclcpp/extended_value.h"
#include "fhiclcpp/fwd.h"
#include <cctype>
#include <map>
#include <sstream>
#include <typeinfo>
#include <unordered_map>
#include <vector>

// ----------------------------------------------------------------------

class fhicl::ParameterSet {
public:
  typedef fhicl::detail::ps_atom_t ps_atom_t;
  typedef fhicl::detail::ps_sequence_t ps_sequence_t;
  typedef std::unordered_map<std::string,std::string> annot_t;


  // compiler generates default c'tor, d'tor, copy c'tor, copy assignment

  // observers:
  bool is_empty() const;
  ParameterSetID id() const;
  std::string to_string() const;
  std::string to_compact_string() const;
  std::string to_indented_string(unsigned initial_indent_level = 0,
                                 bool annotate = true) const;
  std::vector<std::string> get_keys() const;
  std::vector<std::string> get_pset_keys() const;
  // Key must be local to this parameter set: no nesting.
  bool has_key(std::string const & key) const;
  bool is_key_to_table(std::string const & key) const;
  bool is_key_to_sequence(std::string const & key) const;
  bool is_key_to_atom(std::string const & key) const;

  // retrievers (nested key OK):
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
  typedef std::map<std::string, boost::any> map_t;
  typedef map_t::const_iterator map_iter_t;

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

  // Local retrieval only.
  template< class T >
  bool get_one_(std::string const & key, T & value) const;

  bool
  key_is_type_(std::string const & key,
               std::function<bool (boost::any const &)> func) const;

  class Prettifier;

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
has_key(std::string const & key) const
{
  if (key.find('.') != std::string::npos) {
    throw fhicl::exception(unimplemented, "has_key() for nested key.");
  }
  return mapping_.find(key) != mapping_.end();
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
fhicl::ParameterSet::put(std::string const & key, T const & value)
  try
    {
      using detail::encode;
      insert_(key, boost::any(encode(value)));
    }
  catch (boost::bad_lexical_cast const & e)
    {
      throw fhicl::exception(cant_insert, key) << e.what();
    }
  catch (boost::bad_numeric_cast const & e)
    {
      throw fhicl::exception(cant_insert, key) << e.what();
    }
  catch (fhicl::exception const & e)
    {
      throw fhicl::exception(cant_insert, key, e);
    }
  catch (std::exception const & e)
    {
      throw fhicl::exception(cant_insert, key) << e.what();
    }

template< class T >
void
fhicl::ParameterSet::put_or_replace(std::string const & key, T const & value)
try
{
  using detail::encode;
  insert_or_replace_(key, boost::any(encode(value)));
  srcMapping_.erase(key);
}
catch (boost::bad_lexical_cast const & e)
{
  throw fhicl::exception(cant_insert, key) << e.what();
}
catch (boost::bad_numeric_cast const & e)
{
  throw fhicl::exception(cant_insert, key) << e.what();
}
catch (fhicl::exception const & e)
{
  throw fhicl::exception(cant_insert, key, e);
}
catch (std::exception const & e)
{
  throw fhicl::exception(cant_insert, key) << e.what();
}

template< class T >
void
fhicl::ParameterSet::put_or_replace_compatible(std::string const & key,
                                               T const & value)
try
{
  using detail::encode;
  insert_or_replace_compatible_(key, boost::any(encode(value)));
  srcMapping_.erase(key);
}
catch (boost::bad_lexical_cast const & e)
{
  throw fhicl::exception(cant_insert, key) << e.what();
}
catch (boost::bad_numeric_cast const & e)
{
  throw fhicl::exception(cant_insert, key) << e.what();
}
catch (fhicl::exception const & e)
{
  throw fhicl::exception(cant_insert, key, e);
}
catch (std::exception const & e)
{
  throw fhicl::exception(cant_insert, key) << e.what();
}

// ----------------------------------------------------------------------

template< class T >
bool
fhicl::ParameterSet::get_if_present(std::string const & key
                                    , T & value
                                   ) const
{
  typedef std::vector<std::string> Keys;
  Keys keys;
  boost::algorithm::split(keys
                          , key
                          , boost::algorithm::is_any_of(".")
                         );
  Keys::iterator b = keys.begin()
                     , e = keys.end()
                           , r = b;
  for (; b != e; ++b)
    if (! b->empty())
    { *r++ = *b; }
  keys.erase(r, e);
  if (keys.empty())
  { throw fhicl::exception(cant_find, "vacuous key"); }
  ParameterSet const * p = this;
  ParameterSet pset;
  for (b = keys.begin(), e = keys.end() - 1; b != e; ++b) {
    std::string const & this_key = *b;
    if (std::isdigit(this_key[0]))
    { throw fhicl::exception(unimplemented, "lookup in a sequence"); }
    if (! p->get_one_(this_key, pset))
    { return false; }
    p = & pset;
  }
  return p->get_one_(keys.back(), value);
} // get_if_present<>()

template< class T, class Via >
bool
fhicl::ParameterSet::get_if_present(std::string const & key
                                    , T & result
                                    , T convert(Via const &)
                                   ) const
{
  Via go_between;
  if (! get_if_present(key, go_between))
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
fhicl::ParameterSet::get(std::string const & key
                         , T convert(Via const &)
                        ) const
{
  T result;
  return get_if_present(key, result, convert)
         ? result
         : throw fhicl::exception(cant_find, key);
}

template< class T >
T
fhicl::ParameterSet::get(std::string const & key
                         , T const & default_value
                        ) const
{
  T result;
  return get_if_present(key, result) ? result
         : default_value;
}

template< class T, class Via >
T
fhicl::ParameterSet::get(std::string const & key
                         , T const & default_value
                         , T convert(Via const &)
                        ) const
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

template< class T >
bool
fhicl::ParameterSet::get_one_(std::string const & key
                              , T & value
                             ) const
try
{
  using detail::decode;
  map_iter_t it = mapping_.find(key);
  if (it == mapping_.end())
  { return false; }
  decode(it->second, value);
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
