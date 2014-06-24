#ifndef fhiclcpp_ParameterSetRegistry_h
#define fhiclcpp_ParameterSetRegistry_h

// ======================================================================
//
// ParameterSetRegistry
//
// ======================================================================

#include "cetlib/registry_via_id.h"
#include "fhiclcpp/fwd.h"

namespace fhicl {

  typedef  cet::registry_via_id<ParameterSetID, ParameterSet>
           ParameterSetRegistry;

}

// ======================================================================

#endif /* fhiclcpp_ParameterSetRegistry_h */

// Local Variables:
// mode: c++
// End:
