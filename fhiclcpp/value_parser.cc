// ======================================================================
//
// value_parser
//
// ======================================================================

#include "fhiclcpp/value_parser.h"

#include "boost/spirit/include/phoenix_bind.hpp"
#include "boost/spirit/include/phoenix_operator.hpp"
#include "cetlib/canonical_number.h"
#include "cetlib/canonical_string.h"
#include "fhiclcpp/exception.h"
#include "fhiclcpp/tokens.h"

namespace qi    = ::boost::spirit::qi;
namespace ascii = ::boost::spirit::ascii;
namespace phx   = ::boost::phoenix;

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
using qi::no_skip;
using qi::lit;
using qi::raw;
using qi::skip;

typedef  fhicl::extended_value       extended_value;
typedef  extended_value::atom_t      atom_t;
typedef  extended_value::complex_t   complex_t;
typedef  extended_value::sequence_t  sequence_t;
typedef  extended_value::table_t     table_t;

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

  number   = ( fhicl::uint [ _val = phx::bind(&canon_num, _1) ]
             | inf         [ _val = phx::bind(&canon_inf, _1) ]
             | fhicl::real [ _val = _1 ]
             );
  string   = ( fhicl::ass | fhicl::dss | squoted | dquoted )
             [ _val = phx::bind(&canon_str, _1) ];
  name     = fhicl::ass [ _val = _1 ];
  complex  = (  lit('(') >> number
             >> lit(',') >> number >> lit(')')
             ) [ _val = phx::bind( &std::make_pair<std::string,std::string>
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

bool
  fhicl::parse_value( std::string const & s
                    , extended_value    & result
                    )
{
  typedef  std::string::const_iterator  iter;

  typedef  qi::rule<iter>  ws_t;
  ws_t  whitespace = space
                   | lit('#')  >> *(char_ - eol) >> eol
                   | lit("//") >> *(char_ - eol) >> eol;

  value_parser<iter, ws_t> p;
  iter                     begin = s.begin();
  iter const               end   = s.end();

  return qi::phrase_parse( begin, end
                         , p >> *whitespace
                         , whitespace
                         , result
                         )
         && begin == end;

}  // parse_value()

// ======================================================================
