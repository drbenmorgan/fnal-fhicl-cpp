#ifndef fhiclcpp_types_detail_ParameterRegistrySentry_h
#define fhiclcpp_types_detail_ParameterRegistrySentry_h

#include "fhiclcpp/types/detail/KeyStackRegistry.h"

namespace fhicl {
  namespace detail {

    class ParameterRegistrySentry {
    public:

      ParameterRegistrySentry(){}

      ~ParameterRegistrySentry()
      {
        KeyStackRegistry::instance().pop_back();
      }

    };
  }
}

#endif

// Local variables:
// mode: c++
// End:
