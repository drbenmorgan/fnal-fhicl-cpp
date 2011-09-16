#ifndef FHICLCPP_MAKE_PARAMETERSET_H
#define FHICLCPP_MAKE_PARAMETERSET_H

// ======================================================================
//
// make_ParameterSet - alternate representations -> ParameterSet
//
// ======================================================================

#include "cetlib/filepath_maker.h"
#include "cpp0x/string"
#include "fhiclcpp/fwd.h"

// ----------------------------------------------------------------------

namespace fhicl {

  void
    make_ParameterSet( intermediate_table const & tbl
                     , ParameterSet             & ps
                     );

  void
    make_ParameterSet( extended_value const & xval
                     , ParameterSet         & ps
                     );

  void
    make_ParameterSet( std::string const & str
                     , ParameterSet      & ps
                     );

  void
    make_ParameterSet( std::string const   & filename
                     , cet::filepath_maker & maker
                     , ParameterSet        & ps
                     );

}  // fhicl

// ======================================================================

#endif
