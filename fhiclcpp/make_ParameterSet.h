#ifndef FHICLCPP_MAKE_PARAMETERSET_H
#define FHICLCPP_MAKE_PARAMETERSET_H

// ======================================================================
//
// make_ParameterSet - alternate representations -> ParameterSet
//
// ======================================================================

#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/extended_value.h"
#include "fhiclcpp/intermediate_table.h"
#include <istream>
#include <string>

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

  bool
    make_ParameterSet( std::string const & str
                     , ParameterSet      & ps
                     );

  bool
    make_ParameterSet( std::istream & str
                     , ParameterSet & ps
                     );

}  // fhicl

// ======================================================================

#endif
