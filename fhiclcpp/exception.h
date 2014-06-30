#ifndef fhiclcpp_exception_h
#define fhiclcpp_exception_h

// ======================================================================
//
// exception
//
// ======================================================================

#include "cetlib/coded_exception.h"
#include "cpp0x/string"

namespace fhicl {
  enum error { cant_find
             , cant_happen
             , cant_insert
             , number_is_too_large
             , parse_error
             , type_mismatch
             , cant_open_db
             , sql_error
             , unimplemented
             };

  namespace detail {
    std::string translate( error );
  }

  typedef  cet::coded_exception<error,detail::translate>
           exception;
}

// ======================================================================

#endif /* fhiclcpp_exception_h */

// Local Variables:
// mode: c++
// End:
