#ifndef FHICLCPP_PARSE_H
#define FHICLCPP_PARSE_H

// ======================================================================
//
// parse
//
// ======================================================================

#include "cetlib/filepath_maker.h"
#include "fhiclcpp/fwd.h"
#include <string>

namespace fhicl {

  bool
    parse_value( std::string const & s
               , extended_value    & v
               , std::string       & unparsed
               );

  void
    parse_document( std::string const  & s
                  , intermediate_table & result
                  );

  void
    parse_document( std::string const   & filename
                  , cet::filepath_maker & maker
                  , intermediate_table  & result
                  );

}  // namespace fhicl

// ======================================================================

#endif
