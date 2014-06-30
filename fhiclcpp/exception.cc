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
    case cant_happen        : return "Can't happen";
    case cant_insert        : return "Can't insert key";
    case number_is_too_large: return "Excessive value";
    case parse_error        : return "Parse error";
    case type_mismatch      : return "Type mismatch";
    case cant_open_db       : return "Can't open DB";
    case sql_error          : return "SQL error";
    case unimplemented      : return "Unimplemented feature";
    default                 : return "Unknown code";
  }
}

// ======================================================================
