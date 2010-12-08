#ifndef FHICLCPP__MAKE_PARAMETERSET_H
#define FHICLCPP__MAKE_PARAMETERSET_H

// ======================================================================
//
// make_ParameterSet - intermediate_table -> ParameterSet
//
// ======================================================================

#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/intermediate_table.h"

// ----------------------------------------------------------------------

namespace fhicl {

  bool
    make_ParameterSet( intermediate_table const & tbl
                     , ParameterSet             & ps
                     );

}  // fhicl

// ======================================================================

#endif  // FHICLCPP__MAKE_PARAMETERSET_H
