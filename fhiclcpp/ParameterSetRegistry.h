#ifndef FHICLCPP__PARAMETERSETREGISTRY_H
#define FHICLCPP__PARAMETERSETREGISTRY_H

// ======================================================================
//
// ParameterSetRegistry
//
// ======================================================================

#include "cetlib/registry_via_id.h"
#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/ParameterSetID.h"

namespace fhicl {

  typedef  cet::registry_via_id<ParameterSetID, ParameterSet>
           ParameterSetRegistry;

}

// ======================================================================

#endif  // FHICLCPP__PARAMETERSETREGISTRY_H
