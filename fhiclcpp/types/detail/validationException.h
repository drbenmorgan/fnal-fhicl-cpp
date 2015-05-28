#ifndef fhiclcpp_types_detail_validationException_h
#define fhiclcpp_types_detail_validationException_h

#include "cetlib/exception.h"

#include <exception>

namespace fhicl {
  namespace detail {

    class validationException : public std::exception {
    public:

      validationException(const char* msg) : msg_(msg) {}

      const char* what() const noexcept override { return msg_; }

    private:
      const char* msg_;
    };

  }
}

#endif

// Local variables:
// mode:c++
// End:
