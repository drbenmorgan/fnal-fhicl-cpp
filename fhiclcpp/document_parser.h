#ifndef FHICLCPP__DOCUMENT_PARSER_H
#define FHICLCPP__DOCUMENT_PARSER_H

// ======================================================================
//
// document_parser
//
// ======================================================================

#include "boost/any.hpp"
#include "boost/spirit/include/qi.hpp"
#include "fhiclcpp/extended_value.h"
#include "fhiclcpp/intermediate_table.h"
#include "fhiclcpp/value_parser.h"
#include <string>
#include <vector>

namespace fhicl
{
  namespace qi    = ::boost::spirit::qi;
  namespace ascii = ::boost::spirit::ascii;

  template< class FwdIter, class Skip >
  struct document_parser
  : qi::grammar<FwdIter, void(), Skip>
  {
    typedef  fhicl::value_parser<FwdIter,Skip>   value_parser;

    typedef  typename value_parser::atom_token   atom_token;
    typedef  typename value_parser::value_token  value_token;
    typedef  qi::rule<FwdIter, void(), Skip>     nothing_token;

    // default c'tor:
    document_parser( );

    // data members:
    bool                in_prolog;
    bool                prolog_allowed;
    intermediate_table  tbl;
    value_parser        vp;
    // parser rules:
    atom_token     name, ref;
    value_token    value;
    nothing_token  prolog, document;

  };  // document_parser

  bool
    parse_document( std::string const  & s
                  , intermediate_table & result
                  );

}  // namespace fhicl

// ======================================================================

#endif  // FHICLCPP__DOCUMENT_PARSER_H
