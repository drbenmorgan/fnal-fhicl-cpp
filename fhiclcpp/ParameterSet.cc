// ======================================================================
//
// ParameterSet
//
// ======================================================================

#include "fhiclcpp/ParameterSet.h"

#include "cetlib/container_algorithms.h"
#include "cpp0x/cstddef"
#include "fhiclcpp/ParameterSetRegistry.h"
#include <cassert>
#include <regex>

using namespace fhicl;
using namespace fhicl::detail;
using namespace std;
using namespace std::string_literals;

using boost::any;
using boost::any_cast;

using ps_atom_t     = ParameterSet::ps_atom_t;
using ps_sequence_t = ParameterSet::ps_sequence_t;

using ldbl = long double;

// ======================================================================

namespace {

  // See notes for 'put' specialization below
  void
  fill_src_info(extended_value const& value,
                std::string const & key,
                ParameterSet::annot_t& src_map)
  {
    src_map[key]=value.src_info;
    if ( !value.is_a(SEQUENCE) ) return;
    std::size_t i(0);
    for ( auto const& xval : extended_value::sequence_t(value) ) {
      std::ostringstream oss;
      oss << key << "." << i++;
      src_map[oss.str()] = xval.src_info;
      fill_src_info(xval,oss.str(),src_map);
    }
  }

}

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

std::string
ParameterSet::get_src_info(std::string const& key) const
{
  auto result = srcMapping_.find(key);
  return result != srcMapping_.cend() ? result->second : "";
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
// 'put' specialization for extended_value
//
// With this specialization, the free function 'fill_src_info' is
// called, which fills an std::unordered_map whose key-value pairs
// correspond to the ParameterSet key and the location
// (filename:line#) where the key was last overridden.
//
// The main benefit of 'fill_src_info' is that it appropriately tracks
// the source information for individual sequence entries.  This is
// possible because each entry from a 'sequence_t' in the intermediate
// table is an extended_value that has a data member 'src_info'.  Note
// that whenever a printout is provided, the 'Prettifier::stringify()'
// function is used, which no longer uses the extended_value
// instances, but only the mapping_ key-value pairs, which are the
// ParameterSet names and associated boost::any objects.
//
// ParameterSet instances therefore do not have a natural way of
// storing source information for sequence entries because the
// extended-value information is lost.  In other words,  simply doing
//
//      srcMapping_[key] = value.src_info;
//
// whenever 'put' is called will insert the source information for the
// sequence, but not for each sequence entry.  To get around this, the
// 'fill_src_info' function appends an index number to the end of the
// sequence key (e.g. "sequence_key.1").  It is called recursively to
// allow for nested sequences.
//
// In order to access the correct source information for individual
// sequence entries in 'Prettifier::stringify()', the sequence
// index(es) must be appended to the sequence key.  Care must be taken
// to ensure that the correct ParameterSet is used to lookup the
// source information.  This is accomplished by using a stack of the
// form:
//
//     std::vector<ParameterSet const *>

namespace fhicl {

  template<>
  void
  ParameterSet::put(std::string const & key, fhicl::extended_value const & value)
    try
      {
        using detail::encode;
        insert_(key, boost::any(encode(value)));
        fill_src_info(value,key,srcMapping_);
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

}

// ======================================================================

namespace {

  std::string const UNKNOWN_SRC {"-:1"};

  using namespace fhicl;

  inline bool
  allowed_info(std::string const& src_info){
    return !src_info.empty() && src_info != UNKNOWN_SRC;
  }

  inline bool
  allowed_seq_entry(std::string const& key) {
    std::regex const must_be_seq_entry{".*\\..*"};
    std::regex const but_not_first_one{".*\\.0"};
    return
      std::regex_match(key,must_be_seq_entry) &&
      !std::regex_match(key,but_not_first_one);
  }

  std::string
  get_printed_info (any const& a,
                    std::string const& key,
                    ParameterSet const * ps,
                    std::string& curr_info)
  {
    std::string const src_info = ps->get_src_info(key);
    std::string printed_info = src_info;

    // For sequence entries, if the source info is the same as the one
    // before it, set the printed_info to "".  The exception is the
    // first entry of a sequence.
    if ( allowed_info(src_info) &&
         !is_table(a) &&
         allowed_seq_entry(key) &&
         curr_info == src_info ) {
      printed_info = "\"\"";
    }

    curr_info = src_info;

    // Look ahead to see if there is a sequence whose first entry
    // source location is different than that of the sequence source
    // location.  If it is the same, then suppress the source
    // information for the sequence.
    if ( is_sequence(a) ) {
      std::string const next_src_info = ps->get_src_info(key+".0"s);
      if ( next_src_info == src_info && !next_src_info.empty() )
        printed_info = "";
    }

    return printed_info;
  }

}

// ======================================================================

class ParameterSet::Prettifier {
public:

  Prettifier(unsigned const initial_indent_level = 0,
             bool const annotate = true)
    : result_()
    , initial_indent_(initial_indent_level * INDENT_PER)
    , annotate_(annotate)
    , curr_info_()
    , ps_stack_()
  {}

  string
  operator()( ParameterSet const * ps )
  {
    ps_stack_.push_back(ps);
    for (const auto& entry : ps->mapping_ ) {
      assert(next_col() == 1u);
      goto_col(initial_indent_ + 1);
      append(entry.first,": ");
      stringify(entry.second,entry.first);
      append("\n");
    }
    return std::move(result_);
  }

private:
  static constexpr unsigned INDENT_PER = 2;

  std::string result_;
  unsigned initial_indent_;
  bool annotate_;
  std::string curr_info_;
  std::vector<ParameterSet const *> ps_stack_;

  void
  stringify(any const & a, std::string const& key)
  {
    auto ps = ps_stack_.back();
    std::string const printed_info = get_printed_info(a, key, ps, curr_info_);

    size_t const col = next_col();

    if (is_table(a))
      stringify_table(col,a);
    else if (is_sequence(a))
      stringify_sequence(col,a,key,ps);
    else
      stringify_atom(a);

    if ( annotate_ && allowed_info( printed_info ) )
      append("  # ",printed_info);

  } // stringify()

  size_t next_col() const
  {
    size_t const ans = result_.size() + 1u;
    size_t const last_nl = result_.find_last_of('\n') + 1u;
    return ans - last_nl;
  }

  void
  goto_col(size_t const dest)
  {
    size_t now_at = next_col();
    if (now_at > dest) {
      result_.append("\n");
      now_at = 1;
    }
    result_.append(dest - now_at,' ');
  }

  template<typename ... ARGS>
  void append(ARGS ... args)
  {
    cet::for_all(std::vector<std::string>{args...},
                 [this](auto arg){result_.append(arg);});
  }

  void
  stringify_table(std::size_t const col, any const & a)
  {
    ParameterSetID const & psid = any_cast<ParameterSetID>(a);
    ParameterSet const * ps = &ParameterSetRegistry::get(psid);
    ps_stack_.push_back(ps);
    map_t const & mapping = ps->mapping_;
    append("{");
    if (!mapping.empty()) {
      // Emit 1st pair:
      auto it = mapping.begin();
      append(" ",it->first,": ");
      stringify(it->second,it->first);
      // Emit remaining pairs:
      for (auto e = mapping.end(); ++it != e;) {
        goto_col(col + INDENT_PER);
        append(it->first,": ");
        stringify(it->second,it->first);
      }
      goto_col(col);
    }
    append("}");
    ps_stack_.pop_back();
  }

  void
  stringify_sequence(std::size_t const col,
                     any const & a,
                     std::string const & key,
                     ParameterSet const * ps)
  {
    ps_sequence_t const & seq = any_cast<ps_sequence_t>(a);
    append("[");
    if (! seq.empty()) {
      append(" ");
      std::string const first_key = key+".0"s;
      std::string const first_elem_src_info = ps->get_src_info(first_key);
      stringify( *seq.begin(), first_key );
      auto const b = seq.cbegin();
      for (auto it = b, e = seq.end(); ++it != e;) {
        goto_col(col);
        append(", ");
        stringify(*it,key+"."s+std::to_string( std::distance(b,it) ));
      }
      if ( seq.size() == 1u && ( !annotate_ || !allowed_info( first_elem_src_info ) ) )
        append(" ");
      else goto_col(col);
    }
    append("]");
  }

  void
  stringify_atom(any const & a)
  {
    std::string const str = any_cast<ps_atom_t>(a);
    append( str == string(9, '\0') ? "@nil" : str );
  }

}; // Prettifier

// ----------------------------------------------------------------------

string
ParameterSet::to_indented_string(unsigned const initial_indent_level,
                                 bool const annotate) const
{
  Prettifier p(initial_indent_level,annotate);
  return p(this);
}

// ======================================================================
