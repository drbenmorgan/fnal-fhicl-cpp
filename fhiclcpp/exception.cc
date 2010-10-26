// ======================================================================
//
// exception
//
// ======================================================================


#include "fhiclcpp/exception.h"

// ----------------------------------------------------------------------

std::string
  fhicl::detail::translate( error code )
{
  switch( code ) {
    case cant_find          : return "Can't find key";
    case cant_insert        : return "Can't insert key";
    case cant_happen        : return "Can't happen";
    case number_is_too_large: return "Excessive value";
    case type_mismatch      : return "Type mismatch";
    case parse_error        : return "Parse error";
    default                 : return "Unknown code";
  }
}

// ======================================================================
