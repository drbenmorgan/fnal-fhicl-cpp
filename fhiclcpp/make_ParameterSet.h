#ifndef FHICLCPP__MAKE_PARAMETERSET_H
#define FHICLCPP__MAKE_PARAMETERSET_H

// ======================================================================
//
// make_ParameterSet - intermediate_table -> ParameterSet
//
// ======================================================================

#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/extended_value.h"
#include "fhiclcpp/intermediate_table.h"

// ----------------------------------------------------------------------

namespace fhicl {

  bool
    make_ParameterSet( intermediate_table const & tbl
                     , ParameterSet             & ps
                     );

  bool
    make_ParameterSet( extended_value const & xval
                     , ParameterSet         & ps
                     );

}  // fhicl

// ======================================================================

#endif  // FHICLCPP__MAKE_PARAMETERSET_H
