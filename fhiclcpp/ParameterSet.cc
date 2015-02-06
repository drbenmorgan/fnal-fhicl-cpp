// ======================================================================
//
// ParameterSet
//
// ======================================================================

#include "fhiclcpp/ParameterSet.h"

#include "cpp0x/cstddef"
#include "fhiclcpp/ParameterSetRegistry.h"
#include <cassert>

using namespace fhicl;
using namespace fhicl::detail;
using namespace std;

using boost::any;
using boost::any_cast;

typedef ParameterSet::ps_atom_t ps_atom_t;
typedef ParameterSet::ps_sequence_t ps_sequence_t;

typedef long double ldbl;

// ======================================================================

string
ParameterSet::stringify_(any const & a, bool compact) const
{
  string result;
  if (is_table(a)) {
    ParameterSetID const & psid = any_cast<ParameterSetID>(a);
    result = '{' + ParameterSetRegistry::get(psid).to_string() + '}';
    if (compact && result.size() > (5 + ParameterSetID::max_str_size())) {
      // Replace with a reference to the ParameterSetID;
      result = std::string("@id::") + psid.to_string();
    }
  }
  else if (is_sequence(a)) {
    ps_sequence_t const & seq = any_cast<ps_sequence_t>(a);
    result = '[';
    if (! seq.empty()) {
      result.append(stringify_(*seq.begin(), compact));
      for (ps_sequence_t::const_iterator it = seq.begin()
                                              , e = seq.end(); ++it != e;) {
        result.append(1, ',')
        .append(stringify_(*it, compact));
      }
    }
    result.append(1, ']');
  }
  else { // is_atom(a)
    ps_atom_t str = any_cast<ps_atom_t>(a);
    result = str == string(9, '\0') ? "@nil" : str;
  }
  return result;
} // stringify_()

// ----------------------------------------------------------------------

bool
ParameterSet::is_empty() const
{ return mapping_.empty(); }

ParameterSetID
ParameterSet::id() const
{
  if (! id_.is_valid())
  { id_.reset(*this); }
  return id_;
}

string
ParameterSet::to_string_(bool compact) const
{
  string result;
  if (mapping_.empty())
  { return result; }
  map_iter_t it = mapping_.begin();
  result.append(it->first)
  .append(1, ':')
  .append(stringify_(it->second, compact))
  ;
  for (map_iter_t const e = mapping_.end(); ++it != e;)
    result.append(1, ' ')
    .append(it->first)
    .append(1, ':')
    .append(stringify_(it->second, compact))
    ;
  return result;
}

vector<string>
ParameterSet::get_keys() const
{
  vector<string> keys; keys.reserve(mapping_.size());
  for (map_iter_t it = mapping_.begin()
                       , e = mapping_.end(); it != e; ++it)
  { keys.push_back(it->first); }
  return keys;
}

vector<string>
ParameterSet::get_pset_keys() const
{
  vector<string> keys;
  for (map_iter_t it = mapping_.begin()
                       , e = mapping_.end(); it != e; ++it)
    if (is_table(it->second))
    { keys.push_back(it->first); }
  return keys;
}

// ----------------------------------------------------------------------

void
ParameterSet::put(std::string const & key)
{
  void * const value = nullptr;
  put(key, value);
}

void
ParameterSet::put_or_replace(std::string const & key)
{
  void * const value = nullptr;
  put_or_replace(key, value); // Replace with nil is always OK.
}

// ----------------------------------------------------------------------

namespace {
  inline
  void
  check_put_local_key(std::string const & key)
  {
    if (key.find('.') != std::string::npos) {
      throw fhicl::exception(unimplemented,
                             "putXXX() for nested key.");
    }
  }
}

void
ParameterSet::insert_(string const & key, any const & value)
{
  check_put_local_key(key);
  if (!mapping_.emplace(key, value).second) {
    throw exception(cant_insert) << "key " << key << " already exists.";
  }
  id_.invalidate();
}

void
ParameterSet::insert_or_replace_(string const & key, any const & value)
{
  check_put_local_key(key);
  mapping_[key] = value;
  id_.invalidate();
}

void
ParameterSet::insert_or_replace_compatible_(string const & key, any const & value)
{
  check_put_local_key(key);
  auto item = mapping_.find(key);
  if (item == mapping_.end()) {
    insert_(key, value);
    return;
  } else {
    if (!detail::is_nil(value)) {
      auto is_non_nil_atom =
        [](any const & v) -> bool { return ! (detail::is_sequence(v) ||
                                              detail::is_table(v) ||
                                              detail::is_nil(v)); };
      if (detail::is_sequence(item->second) && ! detail::is_sequence(value)) {
        throw exception(cant_insert) << "can't use non-sequence to replace sequence.";
      } else if (detail::is_table(item->second) && ! detail::is_table(value)) {
        throw exception(cant_insert) << "can't use non-table to replace table.";
      } else if (is_non_nil_atom(item->second) &&
                 (detail::is_sequence(value) || detail::is_table(value))) {
        throw exception(cant_insert) << "can't use non-atom to replace non-nil atom.";
      }
    }
    item->second = value;
  }
  id_.invalidate();
}

bool
ParameterSet::erase(string const & key)
{
  bool const did_erase = (1u == mapping_.erase(key));
  id_.invalidate();
  return did_erase;
}

bool
ParameterSet::key_is_type_(std::string const & key,
                           std::function<bool (boost::any const &)> func) const
{
  if (key.find('.') != std::string::npos) {
    throw fhicl::exception(unimplemented,
                           "is_{table,sequence,atom}() for nested key.");
  }
  map_iter_t it = mapping_.find(key);
  if (it == mapping_.end())
  { throw exception(error::cant_find, key); }
  return func(it->second);
}

// ======================================================================

class ParameterSet::Prettifier {
public:
  Prettifier(map_t const & mapping,
             unsigned initial_indent_level = 0)
    : mapping_(mapping)
    , result_()
    , initial_indent_(initial_indent_level * INDENT_PER)
  { }

  string
  operator()()
  {
    for (map_iter_t it = mapping_.begin()
                         , e = mapping_.end(); it != e; ++it) {
      assert(next_col() == 1u);
      goto_col(initial_indent_ + 1);
      result_.append(it->first)
      .append(": ")
      ;
      stringify(it->second);
      result_.append("\n");
    }
    return std::move(result_);
  }

private:
  static constexpr unsigned INDENT_PER = 2;

  map_t const & mapping_;
  string result_;
  unsigned initial_indent_;

  size_t next_col() const
  {
    size_t ans = result_.size() + 1u;
    size_t last_nl = result_.find_last_of('\n') + 1u;
    return ans - last_nl;
  }

  void
  goto_col(size_t dest)
  {
    size_t now_at = next_col();
    if (now_at > dest) {
      result_.append("\n");
      now_at = 1;
    }
    result_.append(dest - now_at, ' ');
  }

  void
  stringify(any const & a)
  {
    size_t col = next_col();
    if (is_table(a)) {
      ParameterSetID const & psid = any_cast<ParameterSetID>(a);
      ParameterSet const & ps = ParameterSetRegistry::get(psid);
      map_t const & mapping = ps.mapping_;
      result_.append("{");
      if (! mapping.empty()) {
        // Emit 1st pair:
        map_iter_t it = mapping.begin();
        result_.append(" ")
        .append(it->first)
        .append(": ")
        ;
        stringify(it->second);
        // Emit remaining pairs:
        for (map_iter_t const e = mapping.end(); ++it != e;) {
          goto_col(col + INDENT_PER);
          result_.append(it->first)
          .append(": ")
          ;
          stringify(it->second);
        }
        goto_col(col);
      }
      result_.append("}");
    }
    else if (is_sequence(a)) {
      ps_sequence_t const & seq = any_cast<ps_sequence_t>(a);
      result_.append("[");
      if (! seq.empty()) {
        result_.append(" ");
        stringify(*seq.begin());
        for (ps_sequence_t::const_iterator it = seq.begin()
                                                , e = seq.end(); ++it != e;) {
          goto_col(col);
          result_.append(", ");
          stringify(*it);
        }
        if (seq.size() == 1u) { result_.append(" "); }
        else { goto_col(col); }
      }
      result_.append("]");
    }
    else // is_atom(a)
    { result_.append(any_cast<ps_atom_t>(a)); }
  } // stringify()

}; // Prettifier

// ----------------------------------------------------------------------

string
ParameterSet::to_indented_string(unsigned initial_indent_level) const
{
  Prettifier p(mapping_, initial_indent_level);
  return p();
}

// ======================================================================
