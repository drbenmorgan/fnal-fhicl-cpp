#include "fhiclcpp/exception.h"
#include "fhiclcpp/types/detail/NameStackRegistry.h"
#include <iostream>
#include <regex>
#include <string>
#include <vector>

namespace fhicl {

    std::string NameStackRegistry::full_key(std::string const & key)
    {
      if ( names_.empty() ||
           std::regex_match(key,std::regex{R"(\[(\d+)\])"})) // Sequence elements
        names_.emplace_back(key);
      else
        names_.emplace_back("."+key);
      return full_key_();
    }


}

// Local variables:
// mode: c++
// End:
