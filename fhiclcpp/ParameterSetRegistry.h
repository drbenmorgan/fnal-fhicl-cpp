#ifndef FHICLCPP__PARAMETERSETREGISTRY_H
#define FHICLCPP__PARAMETERSETREGISTRY_H

// ======================================================================
//
// ParameterSetRegistry
//
// ======================================================================

#include "cetlib/registry.h"
#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/ParameterSetID.h"

namespace fhicl {
  class ParameterSetRegistry;
}

// ----------------------------------------------------------------------

class fhicl::ParameterSetRegistry
  : public cet::registry<ParameterSetID, ParameterSet>
{
  typedef  ParameterSet                   pset_t;
  typedef  ParameterSetID                 psid_t;
  typedef  cet::registry<psid_t, pset_t>  base_t;

public:
  using base_t::empty;
  using base_t::size;
  using base_t::get;

  static  ParameterSetID
    put( ParameterSet const &  );

};  // ParameterSetRegistry

// ======================================================================

#endif  // FHICLCPP__PARAMETERSETREGISTRY_H
