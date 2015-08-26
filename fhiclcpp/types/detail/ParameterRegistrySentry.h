#ifndef fhiclcpp_types_detail_ParameterRegistrySentry_h
#define fhiclcpp_types_detail_ParameterRegistrySentry_h

#include "fhiclcpp/types/detail/NameStackRegistry.h"

namespace fhicl {
  namespace detail {

    class ParameterRegistrySentry {
    public:

      ParameterRegistrySentry(){}

      ~ParameterRegistrySentry()
      {
        NameStackRegistry::instance().pop();
      }

    };
  }
}

#endif

// Local variables:
// mode: c++
// End:
