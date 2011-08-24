#ifndef FHICLCPP_PARAMETERSETREGISTRY_H
#define FHICLCPP_PARAMETERSETREGISTRY_H

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

#endif
