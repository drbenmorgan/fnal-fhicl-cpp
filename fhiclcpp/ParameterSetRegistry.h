#ifndef fhiclcpp_ParameterSetRegistry_h
#define fhiclcpp_ParameterSetRegistry_h

// ======================================================================
//
// ParameterSetRegistry
//
// ======================================================================

#include "cetlib/registry_via_id.h"
#include "fhiclcpp/ParameterSetID.h"
#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/fwd.h"

#include "sqlite3.h"

#include <unordered_map>

namespace fhicl {

  class ParameterSetRegistry;

  namespace detail {
    class HashParameterSetID;
    void throwOnSQLiteFailure(sqlite3 * db, char *msg = nullptr);
  }
}

class fhicl::detail::HashParameterSetID {
public:
  size_t operator () (ParameterSetID const & id) const;
private:
  std::hash<std::string> hash_;
};


class fhicl::ParameterSetRegistry {
public:
  ParameterSetRegistry(ParameterSet const &) = delete;
  ParameterSetRegistry(ParameterSet &&) = delete;
  ParameterSetRegistry & operator = (ParameterSet const &) = delete;
  ParameterSetRegistry & operator = (ParameterSet &&) = delete;
  ~ParameterSetRegistry();

  // Typedefs.
  typedef std::unordered_map<ParameterSetID, ParameterSet, detail::HashParameterSetID> collection_type;
  typedef typename collection_type::key_type key_type;
  typedef typename collection_type::mapped_type mapped_type;
  typedef typename collection_type::value_type value_type;
  typedef typename collection_type::size_type size_type;
  typedef typename collection_type::const_iterator const_iterator;

  // DB interaction.
  static void importFrom(sqlite3 * db);
  static void exportTo(sqlite3 * db);
  static void stageIn();

  // Observers.
  static bool empty();
  static size_type size();

  // Iterators.
  static const_iterator begin();
  static const_iterator end();
  static const_iterator cbegin();
  static const_iterator cend();

  // Put:
  // 1. A single ParameterSet.
  static ParameterSetID const & put(ParameterSet const & ps);
  // 2. A range of iterator to ParameterSet.
  template <class FwdIt>
  static
  typename std::enable_if<std::is_same<typename std::iterator_traits<FwdIt>::value_type,
                                       mapped_type>::value, void>::type
  put(FwdIt begin, FwdIt end);
  // 3. A range of iterator to pair<ParameterSetID, ParameterSet>. For
  // each pair, first == second.id() is a prerequisite.
  template <class FwdIt>
  static
  typename std::enable_if<std::is_same<typename std::iterator_traits<FwdIt>::value_type,
                                       value_type>::value, void>::type
  put(FwdIt begin, FwdIt end);
  // 4. A collection_type. For each value_type, first == second.id() is
  // a prerequisite.
  static void put(collection_type const &c);

  // Accessors.
  static collection_type const & get() noexcept;
  static ParameterSet const & get(ParameterSetID const & id);
  static bool get(ParameterSetID const & id, ParameterSet & ps);

private:
  ParameterSetRegistry();
  static ParameterSetRegistry & instance_();
  const_iterator find_(ParameterSetID const & id);

  sqlite3 * primaryDB_;
  sqlite3_stmt * stmt_;
  collection_type registry_;
};

inline
bool
fhicl::ParameterSetRegistry::
empty()
{
  return instance_().registry_.empty();
}

inline
auto
fhicl::ParameterSetRegistry::
size()
-> size_type
{
  return instance_().registry_.size();
}

inline
auto
fhicl::ParameterSetRegistry::
begin()
-> const_iterator
{
  return instance_().registry_.begin();
}

inline
auto
fhicl::ParameterSetRegistry::
end()
-> const_iterator
{
  return instance_().registry_.end();
}

inline
auto
fhicl::ParameterSetRegistry::
cbegin()
-> const_iterator
{
  return instance_().registry_.cbegin();
}

inline
auto
fhicl::ParameterSetRegistry::
cend()
-> const_iterator
{
  return instance_().registry_.cend();
}

// 1.
inline
auto
fhicl::ParameterSetRegistry::
put(ParameterSet const & ps)
-> ParameterSetID const &
{
  return instance_().registry_.emplace(ps.id(), ps).first->first;
}

// 2.
template <class FwdIt>
inline
auto
fhicl::ParameterSetRegistry::
put(FwdIt b, FwdIt e)
-> typename std::enable_if<std::is_same<typename std::iterator_traits<FwdIt>::value_type,
                                        mapped_type>::value, void>::type
{
  for (; b != e; ++b) {
    (void) put(*b);
  }
}

// 3.
template <class FwdIt>
inline
auto
fhicl::ParameterSetRegistry::
put(FwdIt b, FwdIt e)
-> typename std::enable_if<std::is_same<typename std::iterator_traits<FwdIt>::value_type,
                                        value_type>::value, void>::type
{
  instance_().registry_.insert(b, e);
}

// 4.
inline
void
fhicl::ParameterSetRegistry::
put(collection_type const & c)
{
  put(c.cbegin(), c.cend());
}

inline
auto
fhicl::ParameterSetRegistry::
get() noexcept
-> collection_type const &
{
  return instance_().registry_;
}

inline
auto
fhicl::ParameterSetRegistry::
get(ParameterSetID const & id)
-> ParameterSet const &
{
  const_iterator it = instance_().find_(id);
  if (it == cend() ) {
    throw exception(error::cant_find, "Can't find ParameterSet")
      << "with ID " << id.to_string() << " in the registry.";
  }
  return it->second;
}

inline
bool
fhicl::ParameterSetRegistry::
get(ParameterSetID const & id, ParameterSet & ps)
{
  bool result;
  const_iterator it = instance_().find_(id);
  if (it == cend()) {
    result = false;
  } else {
    ps =  it->second;
    result = true;
  }
  return result;
}

inline
auto
fhicl::ParameterSetRegistry::
ParameterSetRegistry::instance_()
-> ParameterSetRegistry &
{
  static ParameterSetRegistry s_registry;
  return s_registry;
}

inline
size_t
fhicl::detail::HashParameterSetID::
operator () (ParameterSetID const & id) const
{
  return hash_(id.to_string());
}

#endif /* fhiclcpp_ParameterSetRegistry_h */

// Local Variables:
// mode: c++
// End:
