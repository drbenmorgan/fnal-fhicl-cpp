#ifndef FHICLCPP__PARSE_H
#define FHICLCPP__PARSE_H

// ======================================================================
//
// parse
//
// ======================================================================

#include "fhiclcpp/extended_value.h"
#include "fhiclcpp/intermediate_table.h"
#include <istream>
#include <string>

namespace fhicl
{

  bool
    parse_value( std::string const & s
               , extended_value    & v
               );

  bool
    parse_document( std::string const  & s
                  , intermediate_table & result
                  );

  bool
    parse_document( std::istream       & in
                  , intermediate_table & result
                  );

}  // namespace fhicl

// ======================================================================

#endif  // FHICLCPP__PARSE_H
