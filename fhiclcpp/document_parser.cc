// ======================================================================
//
// document_parser
//
// ======================================================================


#include "fhiclcpp/document_parser.h"

#include "boost/spirit/include/phoenix_bind.hpp"
#include "boost/spirit/include/phoenix_operator.hpp"
#include "fhiclcpp/exception.h"
#include "fhiclcpp/tokens.h"  // uint
#include <fstream>

namespace qi    = ::boost::spirit::qi;
namespace ascii = ::boost::spirit::ascii;
namespace phx   = ::boost::phoenix;

using ascii::char_;
using ascii::digit;
using ascii::space;

using phx::bind;

using qi::_1;
using qi::_2;
using qi::_3;
using qi::_val;
using qi::eol;
using qi::lexeme;
using qi::no_skip;
using qi::lit;
using qi::raw;
using qi::skip;

typedef  fhicl::extended_value  extended_value;

// ----------------------------------------------------------------------

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
  fhicl::parse_document( std::string const  & s
                       , fhicl::intermediate_table & result
                       )
{
  typedef  std::string::const_iterator  iter;

  typedef  qi::rule<iter>  ws_t;
  ws_t  whitespace = space
                   | lit('#')  >> *(char_ - eol) >> eol
                   | lit("//") >> *(char_ - eol) >> eol;

  document_parser<iter, ws_t> p;
  iter                        begin = s.begin();
  iter const                  end  =  s.end();

  bool b =  qi::phrase_parse( begin, end
                            , p >> *whitespace
                            , whitespace
                            )
           && begin == end;

  if( b )
    result = p.tbl;
  return b;

}  // parse_document()

// ======================================================================
