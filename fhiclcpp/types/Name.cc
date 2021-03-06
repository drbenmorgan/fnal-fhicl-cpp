#include <regex>
#include <string>
#include "fhiclcpp/types/Name.h"

namespace fhicl {

    std::string const get_regex_replace_str(std::string const& istr)
    {
      static std::regex const r("\\[|\\]");
      auto ostr = std::regex_replace(istr, r, "\\$&");
      return ostr;
    }

}

// Local variables:
// mode: c++
// End:
