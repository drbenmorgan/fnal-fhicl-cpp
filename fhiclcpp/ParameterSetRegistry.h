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
#include <map>


namespace fhicl {
  class ParameterSetRegistry;
}


// ======================================================================


class fhicl::ParameterSetRegistry
  : public cet::registry<ParameterSetID, ParameterSet>
{
  typedef  ParameterSet                 ps_t;
  typedef  ParameterSetID               psid_t;
  typedef  cet::registry<psid_t, ps_t>  base_t;

public:
  using base_t::put;

  static psid_t
    put( ps_t const &  value )
  {
    psid_t key(value);
    base_t::put(key, value);
    return key;
  }

};  // ParameterSetRegistry


// ======================================================================


#endif  // FHICLCPP__PARAMETERSETREGISTRY_H
