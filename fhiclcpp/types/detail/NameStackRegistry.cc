#include "fhiclcpp/exception.h"
#include "fhiclcpp/types/detail/NameStackRegistry.h"
#include <iostream>
#include <regex>
#include <string>
#include <vector>

namespace fhicl {

  std::string NameStackRegistry::full_key(std::string const& name)
  {
    if (names_.empty() ||
        std::regex_match(name,std::regex{R"(\[\d+\])"})) // Sequence elements
      names_.emplace_back(name);
    else
      names_.emplace_back("."+name);
    return std::accumulate(names_.begin(), names_.end(), std::string{});
  }

}

// Local variables:
// mode: c++
// End:
