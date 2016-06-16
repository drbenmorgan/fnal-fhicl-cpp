#ifndef fhiclcpp_types_Comment_h
#define fhiclcpp_types_Comment_h

#include <string>

namespace fhicl {

  struct Comment{
    explicit Comment(const char* comment = "") : value(comment) {}
    std::string value;
  };

}

#endif /* fhiclcpp_types_Comment_h */

// Local variables:
// mode: c++
// End:
