// ======================================================================
//
// ParameterSetRegistry
//
// ======================================================================


#include "fhiclcpp/ParameterSetRegistry.h"

using namespace fhicl;

// ======================================================================

ParameterSetID
  ParameterSetRegistry::put( ParameterSet const & value )
{
  ParameterSetID key(value);
  base_t::put(key, value);
  return key;
}

// ======================================================================
