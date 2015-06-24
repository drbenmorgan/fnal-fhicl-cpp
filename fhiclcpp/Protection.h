#ifndef fhiclcpp_Proection_h
#define fhiclcpp_Proection_h

#include <string>

namespace fhicl {
  enum class Protection : unsigned char { NONE, PROTECT_IGNORE, PROTECT_ERROR };

  std::string to_string(Protection p);
}

#endif /* fhiclcpp_Protection_h */

// Local Variables:
// mode: c++
// End:
