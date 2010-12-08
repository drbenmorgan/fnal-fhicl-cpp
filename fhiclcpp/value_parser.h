#ifndef FHICLCPP__VALUE_PARSER_H
#define FHICLCPP__VALUE_PARSER_H

// ======================================================================
//
// value_parser
//
// ======================================================================

#include "boost/any.hpp"
#include "boost/spirit/include/qi.hpp"
#include "fhiclcpp/extended_value.h"
#include "fhiclcpp/intermediate_table.h"
#include <string>
#include <vector>

namespace fhicl
{
  namespace qi    = ::boost::spirit::qi;
  namespace ascii = ::boost::spirit::ascii;

  template< class FwdIter, class Skip >
  struct value_parser
  : qi::grammar<FwdIter, extended_value(), Skip>
  {
    typedef  extended_value::atom_t      atom_t;
    typedef  extended_value::complex_t   complex_t;
    typedef  extended_value::sequence_t  sequence_t;
    typedef  extended_value::table_t     table_t;

    typedef  qi::rule<FwdIter, atom_t        (), Skip>  atom_token;
    typedef  qi::rule<FwdIter, complex_t     (), Skip>  complex_token;
    typedef  qi::rule<FwdIter, sequence_t    (), Skip>  sequence_token;
    typedef  qi::rule<FwdIter, table_t       (), Skip>  table_token;
    typedef  qi::rule<FwdIter, extended_value(), Skip>  value_token;

    // default c'tor:
    value_parser( );

    // data member:
    extended_value  v;
    // parser rules:
    atom_token   nil, boolean;
    atom_token   inf;
    atom_token   squoted, dquoted;
    atom_token   number, string, name;
    complex_token  complex;
    sequence_token sequence;
    table_token    table;
    value_token    value;

  };  // value_parser

  bool
    parse_value( std::string const & s
               , extended_value    & v
               );

}  // namespace fhicl

// ======================================================================

#endif  // FHICLCPP__VALUE_PARSER_H
