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
             , cant_happen
             , cant_insert
             , number_is_too_large
             , parse_error
             , type_mismatch
             };

  namespace detail {
    std::string translate( error );
  }

  typedef  cet::coded_exception<error,detail::translate>
           exception;
}

// ======================================================================

#endif
