#ifndef fhiclcpp_types_detail_ParameterReferenceRegistry_h
#define fhiclcpp_types_detail_ParameterReferenceRegistry_h

/*
  ----------------------------
  EXPERT-LEVEL code
  (not to be called by users)
  ----------------------------

  The ParameterReferenceRegistry is a static container whose elements
  are pairs of:

     first  : key string
     second : pointer to ParameterBase

  There are two purposes for this registry:

     (1) Provide a list of keys and associated FHiCL parameter
         pointers for setting all parameter values after the
         parameters have been validated.

     (2) Provide a list of keys and appropriate corresponding
         information for describing the particular configuration of
         interest.

  The weakness of this registry is that even though the
  std::pair<T>::first is always valid, std::pair<T>::second is
  frequently not.  This is because the underlying type of every
  non-atomic parameter (Sequence<>, Tuple<>, and Table<>) contains
  atomic parameters, even though the user does not explicitly specify
  this.  All parameters (including elements of sequences and tuples)
  fundamentally contain Atom<>'s so that the individual parameters can
  be compared against user-provided configurations.

  Since Sequence<> and Tuple<> elements are frequently overwritten,
  the registry must be purged of the stale keys/pointers.  This is
  done by the RAII design of the parameters themselves (the d'tor's of
  each class template erase their corresponding entry from the
  registry), however there are some loopholes that arose during
  testing.

  If the return type of a Sequence (bounded or unbounded) contains a
  Table<>, whenever the operator() function is called default
  constructors of the individual table elements will register
  themselves, even though such registration is no longer necessary.
  Should a user then wish to create another Table<> and set the values
  of the corresponding elements, the pointers of the registry from the
  previous table could be invalid.  This should not be a problem, as
  long as those pointers are never resolved.  Unfortunately, the
  current ParameterReferenceRegistry::set_values() function loops over
  all registered entries, introducing an opportunity for memory read
  problems.

  I have not found a good way to solve this problem, other than to
  clear the registry for each {Sequence,Tuple}<>::operator() call.
  That is ugly, and the best fix for this would be to have individual
  parameters register themselves only when the user specifies their
  key-required construction (i.e. not during default/copy/move
  construction or assignment).  Long story short:

    FIXME: Specify 'set_values' for specific ParameterBase objects;
           not looping over all keys.

    FIXME: revamp system so that only c'tor calls that require a Name()
           argument register the parameter.
 */

#include "fhiclcpp/types/detail/ParameterBase.h"
#include "cetlib/exempt_ptr.h"

#include <list>
#include <set>
#include <string>
#include <utility>
#include <vector>

namespace fhicl {

  class ParameterSet;

  namespace detail {

    class ParameterReferenceRegistry {
    public:

      static ParameterReferenceRegistry& instance()
      {
        static ParameterReferenceRegistry registry;
        return registry;
      }

      // Type aliases

      using key_string = std::string;
      using base_ptr   = cet::exempt_ptr<ParameterBase>;
      using ref_pair   = std::pair<key_string,base_ptr>;
      using ref_map    = std::list<ref_pair>;

      // Static observers

      static ref_map::const_iterator find_parameter_by_key(key_string const& key);

      static ref_map  get_parameters(ParameterBase const * pb);
      static ref_map  get_child_parameters(ParameterBase const * pb);
      static ref_pair get_parameter_by_key (key_string const& key);
      static ref_map  get_parameters_by_key(key_string const& key);
      static ref_map  get_child_parameters_by_key(key_string const& key);

      static std::vector<key_string> get_parameter_keys(ParameterBase const * pb);
      static void get_keys_with_defaults(ParameterBase const * pb, std::set<key_string>& keys);
      static void print_keys();

      // Non-static observers

      bool empty() const;

      // Modifiers

      void clear();
      void emplace(key_string const& key, base_ptr ptr);
      void erase  (key_string const& key);
      void set_values(fhicl::ParameterSet const& pset, bool const trimParent);

    private:

      ParameterReferenceRegistry(){}
      ref_map parameters_;

    };

  }
}

#endif

  // Local variables:
  // mode: c++
  // End:
