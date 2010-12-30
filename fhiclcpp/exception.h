#ifndef FHICLCPP_EXCEPTION_H
#define FHICLCPP_EXCEPTION_H

// ======================================================================
//
// exception
//
// ======================================================================


#include "cetlib/coded_exception.h"
#include <string>


namespace fhicl {
  enum error { cant_find
             , cant_insert
             , cant_happen
             , number_is_too_large
             , type_mismatch
             , parse_error
             };

  namespace detail {
    std::string translate( error );
  }

  typedef  cet::coded_exception<error,detail::translate>
           exception;
}

// ======================================================================

#endif
