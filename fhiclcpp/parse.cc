// ======================================================================
//
// parse
//
// ======================================================================

#include "fhiclcpp/parse.h"

#include "boost/any.hpp"
#include "boost/spirit/include/phoenix_bind.hpp"
#include "boost/spirit/include/phoenix_operator.hpp"
#include "boost/spirit/include/qi.hpp"
#include "boost/spirit/include/support_istream_iterator.hpp"
#include "cetlib/canonical_number.h"
#include "cetlib/canonical_string.h"
#include "cetlib/include.h"
#include "fhiclcpp/exception.h"
#include "fhiclcpp/extended_value.h"
#include "fhiclcpp/intermediate_table.h"
#include "fhiclcpp/tokens.h"
#include <string>
#include <vector>

namespace ascii = ::boost::spirit::ascii;
namespace phx   = ::boost::phoenix;
namespace qi    = ::boost::spirit::qi;

using ascii::char_;
using ascii::digit;
using ascii::graph;
using ascii::space;

using phx::bind;

using qi::_1;
using qi::_2;
using qi::_3;
using qi::_val;
using qi::eol;
using qi::lexeme;
using qi::lit;
using qi::no_skip;
using qi::raw;
using qi::skip;

typedef  fhicl::extended_value       extended_value;
typedef  extended_value::atom_t      atom_t;
typedef  extended_value::complex_t   complex_t;
typedef  extended_value::sequence_t  sequence_t;
typedef  extended_value::table_t     table_t;

// ----------------------------------------------------------------------

static  std::string const &
  fhicl_env_var( )
{
  static  std::string const  fhicl_env_var("FHICL_FILE_PATH");
  return fhicl_env_var;
}

// ----------------------------------------------------------------------

static  std::string
  canon_inf( std::string const & inf )
{
  return inf[0] == 'i'  ?  ('+' + inf)
                        :  inf;
}

static  std::string
  canon_num( std::string const & num )
{
  std::string result;
  if( ! cet::canonical_number(num, result) )
    result = "####";
  return result;
}

static  std::string
  canon_str( std::string const & str )
{
  std::string result;
  if( ! cet::canonical_string(str, result) )
    result = "oops";
  return result;
}

static  void
  rebool( bool & b, bool value )
{
  b = value;
}

static  extended_value
  xvalue( bool b, fhicl::value_tag t, boost::any v )
{
  return fhicl::extended_value(b, t, v );
}

static  extended_value
  lookup( std::string               const & name
        , fhicl::intermediate_table const & tbl
        )
{
  if( name.substr(0,4) == "db::" )
    throw fhicl::exception(fhicl::cant_find, name);

  return tbl.find(name.substr(7));
}

static  void
  tbl_insert( std::string           const & name
            , extended_value        const & value
            , fhicl::intermediate_table   & t
            )
{
  t.insert(name, value);
}

static  void
  map_insert( std::string    const & name
            , extended_value const & value
            , table_t              & t
            )
{
  t.insert( std::make_pair(name, value) );
}

// ----------------------------------------------------------------------

namespace fhicl
{
  template< class FwdIter, class Skip >
    struct value_parser;

  template< class FwdIter, class Skip >
    struct document_parser;

}  // namespace fhicl

// ----------------------------------------------------------------------

template< class FwdIter, class Skip >
  struct fhicl::value_parser
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

// ----------------------------------------------------------------------

template< class FwdIter, class Skip >
  struct fhicl::document_parser
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

// ----------------------------------------------------------------------

template< class FwdIter, class Skip >
  fhicl::value_parser<FwdIter,Skip>::value_parser( )
: value_parser::base_type( value )
, v                      ( )
{
  nil      = qi::string("nil")
           >> !(graph - char_(",]}"));
  boolean  = (qi::string("true") | qi::string("false"))
           >> !(graph - char_(",]}"));

  inf      = lexeme[ -(qi::string("+") | qi::string("-"))
                   >> qi::string("infinity")
                   >> !(graph - char_("),]}"))
                   ];

  squoted  = lexeme[ char_('\'')
                   >> *(char_ - char_('\''))
                   >> char_('\'')
                   >> !(graph - char_(",]}"))
                   ];
  dquoted  = lexeme[ char_('\"')
                   >> *( (char_ - char_('\"'))
                       | qi::string("\\\"")
                       )
                   >> char_('\"')
                   >> !(graph - char_(",]}"))
                   ];

  number   = ( fhicl::uint [ _val = bind(&canon_num, _1) ]
             | inf         [ _val = bind(&canon_inf, _1) ]
             | fhicl::real [ _val = _1 ]
             );
  string   = ( fhicl::ass | fhicl::dss | squoted | dquoted )
             [ _val = bind(&canon_str, _1) ];
  name     = fhicl::ass [ _val = _1 ];
  complex  = (  lit('(') >> number
             >> lit(',') >> number >> lit(')')
             ) [ _val = bind( &std::make_pair<std::string,std::string>
                                 , _1 , _2 )
               ];

  sequence = lit('[') >> -(value % ',') >> lit(']');
  table    = lit('{')
           >> *((name >> lit(':') >> value) [ bind(map_insert, _1, _2, _val) ]
               )
           >> lit('}');

  value    = ( nil      [ _val = bind(xvalue, false, NIL     , _1) ]
             | boolean  [ _val = bind(xvalue, false, BOOL    , _1) ]
             | number   [ _val = bind(xvalue, false, NUMBER  , _1) ]
             | complex  [ _val = bind(xvalue, false, COMPLEX , _1) ]
             | string   [ _val = bind(xvalue, false, STRING  , _1) ]
             | sequence [ _val = bind(xvalue, false, SEQUENCE, _1) ]
             | table    [ _val = bind(xvalue, false, TABLE   , _1) ]
             );

  nil     .name("nil token");
  boolean .name("boolean token");
  inf     .name("inf token");
  squoted .name("squoted token");
  dquoted .name("dquoted token");
  number  .name("number atom");
  string  .name("string atom");
  name    .name("name atom");
  complex .name("complex atom");
  sequence.name("sequence");
  table   .name("table");
  value   .name("value");

}  // value_parser c'tor

// ----------------------------------------------------------------------

template< class FwdIter, class Skip >
  fhicl::document_parser<FwdIter,Skip>::document_parser( )
: document_parser::base_type( document )
, in_prolog           ( false )
, prolog_allowed      ( true )
, tbl                 ( )
, vp                  ( )
{
  name     = fhicl::ass                                    [ _val = _1 ]
           >> *( (char_('.') >> fhicl::ass)                [ _val += _1 + _2 ]
               | (char_('[') >> fhicl::uint >> char_(']')) [ _val += _1 + _2 + _3]
               );  // TODO: only some whitespace permitted
  ref      = ((qi::string("local::") | qi::string("db::"))
             >> name
             ) [ _val = _1 + _2 ];  // TODO: no whitespace permitted

  value    = ( vp.nil      [ _val = bind(xvalue, in_prolog, NIL     , _1) ]
             | vp.boolean  [ _val = bind(xvalue, in_prolog, BOOL    , _1) ]
             | vp.number   [ _val = bind(xvalue, in_prolog, NUMBER  , _1) ]
             | vp.complex  [ _val = bind(xvalue, in_prolog, COMPLEX , _1) ]
             | vp.string   [ _val = bind(xvalue, in_prolog, STRING  , _1) ]
             | ref         [ _val = bind(lookup, _1, tbl) ]
             | vp.sequence [ _val = bind(xvalue, in_prolog, SEQUENCE, _1) ]
             | vp.table    [ _val = bind(xvalue, in_prolog, TABLE   , _1) ]
             );

  prolog   = lit("BEGIN_PROLOG") [ bind(rebool, phx::ref(in_prolog), true) ]
           >> *((name >> lit(':') >> value)
                                 [ bind(tbl_insert, _1, _2, phx::ref(tbl)) ]
               )
           >> lit("END_PROLOG")  [ bind(rebool, phx::ref(in_prolog), false) ];
  document = (*prolog)    [ bind(rebool, phx::ref(prolog_allowed), false) ]
           >> *((name >> lit(':') >> value)
                          [ bind(tbl_insert, _1, _2, phx::ref(tbl)) ]
               );

  name    .name("name atom");
  ref     .name("ref atom");
  value   .name("value");
  prolog  .name("prolog");
  document.name("document");

}  // document_parser c'tor

// ----------------------------------------------------------------------

bool
  fhicl::parse_value( std::string const & s
                    , extended_value    & result
                    )
{
  typedef  std::string::const_iterator  iter_t;

  typedef  qi::rule<iter_t>  ws_t;
  ws_t  whitespace = space
                   | lit('#')  >> *(char_ - eol) >> eol
                   | lit("//") >> *(char_ - eol) >> eol;

  value_parser<iter_t, ws_t> p;
  iter_t                     begin = s.begin();
  iter_t const               end   = s.end();

  return qi::phrase_parse( begin, end
                         , p >> *whitespace
                         , whitespace
                         , result
                         )
         && begin == end;

}  // parse_value()

// ----------------------------------------------------------------------

bool
  fhicl::parse_document( std::string const  & s
                       , intermediate_table & result
                       )
{
  typedef  std::string::const_iterator  iter_t;

  typedef  qi::rule<iter_t>  ws_t;
  ws_t  whitespace = space
                   | lit('#')  >> *(char_ - eol) >> eol
                   | lit("//") >> *(char_ - eol) >> eol;

  document_parser<iter_t, ws_t> p;
  iter_t                        begin = s.begin();
  iter_t const                  end  =  s.end();

  bool b =  qi::phrase_parse( begin, end
                            , p >> *whitespace
                            , whitespace
                            )
           && begin == end;

  if( b )
    result = p.tbl;
  return b;

}  // parse_document()

// ----------------------------------------------------------------------

bool
  fhicl::parse_document( std::istream       & in
                       , intermediate_table & result
                       )
{
  std::string str;
  cet::include(in, fhicl_env_var(), str);
  return parse_document(str, result);
}  // parse_document()

// ======================================================================
