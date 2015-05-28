#ifndef fhiclcpp_types_detail_validate_ParameterSet_h
#define fhiclcpp_types_detail_validate_ParameterSet_h

#include "cetlib/exempt_ptr.h"

#include <set>
#include <string>
#include <vector>

namespace fhicl {

  class ParameterSet;

  namespace detail {
    class ParameterBase;
  }

  using base_ptr   = cet::exempt_ptr<detail::ParameterBase>;
  using key_string = std::string;
  using key_set    = std::set<key_string>;

  namespace detail {

    using ref_map = std::list<std::pair<key_string,base_ptr>>;

    void validate_ParameterSet(base_ptr pbPtr,
                               ParameterSet const& pset,
                               key_set const& ignorableKeys = {});

    void validate(ref_map const & ref_keys,
                  key_set const& ignorableKeys,
                  key_set & userKeys,
                  key_set & missingKeys);


  }
}

#endif

// Local variables:
// mode: c++
// End:
