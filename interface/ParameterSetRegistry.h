#ifndef FHICLCPP_REGISTRY_H
#define FHICLCPP_REGISTRY_H

// ======================================================================
//
// ParameterSetRegistry
//
// Usage:
//   ParameterSetRegistry::put( key, pset );
//   if( ParameterSetRegistry::get( key, pset ) ) ...
//
// ======================================================================


#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/ParameterSetID.h"

#include <map>

namespace fhicl {
  class ParameterSetRegistry;
}

// ======================================================================


class fhicl::ParameterSetRegistry
{
  typedef  fhicl::ParameterSet     pset_t;
  typedef  fhicl::ParameterSetID   psetid_t;
  typedef  std::map<psetid, pset>  registry_t;

public:
  static  void  put( psetid_t const &  key
                   , pset_t   const &  value
                   )
  {
    instance().insert( std::make_pair(key, value) );
  }

  static  pset const &  get( psetid_t const & key )
  {
    registry::iterator it = instance().find(name);
    return it == instance().end() ?  throw NOT_FOUND_EXCEPTION
                                  :  it->second;
  }

  static  bool  get( psetid_t const & key
                   , pset           & ps
                   )
  {
    registry::iterator it = instance().find(name);
    return it == instance().end() ?  (         false       )
                                  :  (ps = it->second, true);
  }

private:
  ParameterSetRegistry() { }
  ParameterSetRegistry( ParameterSetRegistry const & );
  void  operator = ( ParameterSetRegistry const & );

  static registry_t & instance() {
    static  registry_t  the_instance;
    return the_instance;
  }

};  // ParameterSetRegistry

// ======================================================================


#endif  // FHICLCPP_REGISTRY_H
