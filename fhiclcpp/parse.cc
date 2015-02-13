// ======================================================================
//
// parse
//
// ======================================================================

#include "cpp0x/detail/config"

#include "fhiclcpp/parse.h"

#include "boost/any.hpp"
#ifdef __ICC
#pragma warning(push, disable:780)
#endif
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include "boost/spirit/include/phoenix_bind.hpp"
#include "boost/spirit/include/phoenix_operator.hpp"
#include "boost/spirit/include/qi.hpp"
#include "boost/spirit/include/qi_no_skip.hpp"
#include "boost/spirit/include/support_istream_iterator.hpp"
#pragma GCC diagnostic ignored "-pedantic"
#include "boost/spirit/repository/home/qi/primitive/iter_pos.hpp"
#pragma GCC diagnostic pop
#ifdef __ICC
#pragma warning(pop)
#endif

#include "cetlib/canonical_number.h"
#include "cetlib/canonical_string.h"
#include "cetlib/include.h"
#include "cetlib/includer.h"
#include "cpp0x/string"
#include "fhiclcpp/exception.h"
#include "fhiclcpp/extended_value.h"
#include "fhiclcpp/intermediate_table.h"
#include "fhiclcpp/tokens.h"
#include <vector>

namespace ascii = ::boost::spirit::ascii;
namespace phx   = ::boost::phoenix;
namespace qi    = ::boost::spirit::qi;

using ascii::char_;
using ascii::digit;
using ascii::graph;
using ascii::space;

using phx::ref;

using boost::spirit::repository::qi::iter_pos;
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

namespace {

  std::string
  canon_nil(std::string const &)
  {
    static std::string const canon_nil(9, '\0');
    return canon_nil;
  }

  std::string
  canon_inf(std::string const & inf)
  {
    return inf[0] == 'i' ? ('+' + inf)
           : inf;
  }

  std::string
  canon_num(std::string const & num)
  {
    std::string result;
    if (! cet::canonical_number(num, result))
    { result = "####"; }
    return result;
  }

  std::string
  canon_str(std::string const & str)
  {
    std::string result;
    if (! cet::canonical_string(str, result))
    { result = "oops"; }
    return result;
  }

  void
  rebool(bool & b, bool value)
  {
    b = value;
  }

  extended_value
  xvalue(bool b, fhicl::value_tag t, boost::any v)
  {
    return fhicl::extended_value(b, t, v);
  }

  complex_t
  cplx(atom_t const & c1, atom_t const & c2)
  {
    return std::make_pair(c1, c2);
  }

  template <typename FwdIter>
  fhicl::extended_value
  local_lookup(std::string const & name,
               fhicl::intermediate_table const & tbl,
               bool in_prolog,
               FwdIter pos,
               cet::includer const & s)
  try
  {
    fhicl::extended_value result = tbl.find(name);
    result.set_prolog(in_prolog);
    return result;
  }
  catch (fhicl::exception const & e)
  {
    throw fhicl::exception(fhicl::error::parse_error, "Local lookup error", e)
      << "at "
      << s.highlighted_whereis(pos)
      << "\n";
  }

  template <typename FwdIter>
  fhicl::extended_value
  database_lookup(std::string const &,
                  fhicl::intermediate_table const &,
                  bool,
                  FwdIter pos,
                  cet::includer const & s)
  {
    throw fhicl::exception(fhicl::error::unimplemented, "Database lookup error")
      << "at "
      << s.highlighted_whereis(pos)
      << "\nFHiCL-cpp database lookup not yet available.\n";
  }

  void
  tbl_insert(std::string const & name,
             extended_value const & value,
             fhicl::intermediate_table & t)
  {
    t.insert(name, value);
  }

  void
  tbl_erase(std::string const & name,
            fhicl::intermediate_table & t)
  {
    t.erase(name);
  }

  void
  map_insert(std::string const & name
             , extended_value const & value
             , table_t & t
            )
  {
    t[name] = value;
  }

  template <typename TABLEISH, typename FwdIter>
  void
  insert_table(std::string const & name,
               fhicl::intermediate_table & tbl,
               bool in_prolog,
               TABLEISH & t,
               FwdIter pos,
               cet::includer const & s)
  {
    fhicl::extended_value const & xval =
      local_lookup(name, tbl, false, pos, s);
    if (!xval.is_a(fhicl::TABLE)) {
      throw fhicl::exception(fhicl::error::type_mismatch, "@table::")
          << "key \""
          << name
          << "\" does not refer to a table at "
          << s.highlighted_whereis(pos)
          << "\n";
    }
    table_t const & incoming = boost::any_cast<table_t const &>(xval.value);
    for (auto i = incoming.cbegin(), e = incoming.cend(); i != e; ++i) {
      auto & element = t[i->first];
      element = i->second;
      element.set_prolog(in_prolog);
    }
  }

  template <typename FwdIter>
  void
  seq_insert_sequence(std::string const & name,
                      fhicl::intermediate_table & tbl,
                      bool in_prolog,
                      sequence_t & v,
                      FwdIter pos,
                      cet::includer const & s)
  {
    fhicl::extended_value const & xval =
      local_lookup(name, tbl, false, pos, s);
    if (!xval.is_a(fhicl::SEQUENCE)) {
      throw fhicl::exception(fhicl::error::type_mismatch, "@sequence::")
          << "key \""
          << name
          << "\" does not refer to a sequence at "
          << s.highlighted_whereis(pos)
          << "\n";
    }
    sequence_t const & incoming = boost::any_cast<sequence_t const &>(xval.value);
#if 0 /* Compiler supports C++2011 signature for ranged vector::insert() */
    auto it = v.insert(v.end(), incoming.cbegin(), incoming.cend());
#else
    v.insert(v.end(), incoming.cbegin(), incoming.cend());
    auto it = v.end() - incoming.size();
#endif
    for (auto const e = v.end(); it != e; ++it) {
      it->set_prolog(in_prolog);
    }
  }

  void
  seq_insert_value(fhicl::extended_value const & xval,
                   sequence_t & v)
  {
    v.push_back(xval);
  }

  void
  map_erase(std::string const & name,
            table_t & t)
  {
    t.erase(name);
  }
}
// ----------------------------------------------------------------------

namespace fhicl {
  template< class FwdIter, class Skip >
  struct value_parser;

  template< class FwdIter, class Skip >
  struct document_parser;

}  // namespace fhicl

// ----------------------------------------------------------------------

template< class FwdIter, class Skip >
struct fhicl::value_parser
: qi::grammar<FwdIter, extended_value(), Skip> {
  typedef  qi::rule<FwdIter, atom_t (), Skip>  atom_token;
  typedef  qi::rule<FwdIter, complex_t (), Skip>  complex_token;
  typedef  qi::rule<FwdIter, sequence_t (), Skip>  sequence_token;
  typedef  qi::rule<FwdIter, table_t (), Skip>  table_token;
  typedef  qi::rule<FwdIter, extended_value(), Skip>  value_token;

  // default c'tor:
  value_parser();

  // data member:
  extended_value  v;

  // parser rules:
  atom_token      nil, boolean;
  atom_token      inf;
  atom_token      squoted, dquoted;
  atom_token      number, string, name;
  atom_token      id;
  complex_token   complex;
  sequence_token  sequence;
  table_token     table;
  value_token     value;

};  // value_parser

// ----------------------------------------------------------------------

template< class FwdIter, class Skip >
struct fhicl::document_parser
: qi::grammar<FwdIter, void(), Skip> {
  typedef  fhicl::value_parser<FwdIter, Skip>   value_parser;

  typedef  typename value_parser::atom_token      atom_token;
  typedef  typename value_parser::sequence_token  sequence_token;
  typedef  typename value_parser::table_token     table_token;
  typedef  typename value_parser::value_token     value_token;
  typedef  qi::rule<FwdIter, void(), Skip>        nothing_token;

  document_parser(cet::includer const & s);

  // data members:
  bool                in_prolog;
  intermediate_table  tbl;
  value_parser        vp;

  // parser rules:
  atom_token      name, qualname, noskip_qualname, localref, dbref;
  sequence_token  sequence;
  table_token     table;
  value_token     value;
  nothing_token   prolog, document;

};  // document_parser

// ----------------------------------------------------------------------

template< class FwdIter, class Skip >
fhicl::value_parser<FwdIter, Skip>::value_parser()
  : value_parser::base_type(value)
  , v()
{
  nil      = lexeme[(qi::string("@nil")
                     >> !(graph - char_(",]}"))
                    ) [ _val = phx::bind(canon_nil, qi::_1) ]
                   ];
  boolean  = lexeme[(qi::string("true") | qi::string("false"))
                    >> !(graph - char_(",]}"))
                   ];
  inf      = lexeme[ -(qi::string("+") | qi::string("-"))
                     >> qi::string("infinity")
                     >> !(graph - char_("),]}"))
                   ];
  squoted  = lexeme[ char_('\'')
                     >> *(char_ - char_('\''))
                     >> char_('\'')
                     >> !(graph - char_(",]}"))
                   ];
  dquoted  = lexeme[ raw [ char_('\"')
                           >> *(qi::string("\\\"")
                                | (char_ - char_('\"'))
                               )
                           >> char_('\"')
                           >> !(graph - char_(",]}"))
                         ] ];
  number   = (fhicl::uint [ _val = phx::bind(canon_num, qi::_1) ]
              | inf         [ _val = phx::bind(canon_inf, qi::_1) ]
              | fhicl::real [ _val = qi::_1 ]
              | fhicl::hex  [ _val = qi::_1 ]
              | fhicl::bin  [ _val = qi::_1 ]
             );
  string   = (fhicl::ass | fhicl::dss | squoted | dquoted)
             [ _val = phx::bind(canon_str, qi::_1) ];
  name     = fhicl::ass [ _val = qi::_1 ];
  complex  = (lit('(') > number
              > lit(',') > number > lit(')')
             ) [ _val = phx::bind(cplx, qi::_1 , qi::_2) ];
  sequence = lit('[') > -(value % ',') > lit(']');
  table    = lit('{')
             > *((name >> (lit(':') >> value)
                 ) [ phx::bind(map_insert, qi::_1, qi::_2, _val) ]
                 | (name >> (lit(':') > lit("@erase"))) [ phx::bind(map_erase, qi::_1, _val) ]
                )
             > lit('}');
  id    = lit("@id::") > no_skip [ fhicl::dbid ] [ _val = qi::_1 ];
  value    = (nil      [ _val = phx::bind(xvalue, false, NIL     , qi::_1) ]
              | boolean  [ _val = phx::bind(xvalue, false, BOOL    , qi::_1) ]
              | number   [ _val = phx::bind(xvalue, false, NUMBER  , qi::_1) ]
              | complex  [ _val = phx::bind(xvalue, false, COMPLEX , qi::_1) ]
              | string   [ _val = phx::bind(xvalue, false, STRING  , qi::_1) ]
              | sequence [ _val = phx::bind(xvalue, false, SEQUENCE, qi::_1) ]
              | table    [ _val = phx::bind(xvalue, false, TABLE   , qi::_1) ]
              | id    [ _val = phx::bind(xvalue, false, TABLEID , qi::_1) ]
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
  id   .name("id atom");
  value   .name("value");
}  // value_parser c'tor

// ----------------------------------------------------------------------

template< class FwdIter, class Skip >
fhicl::document_parser<FwdIter, Skip>::document_parser(cet::includer const & s)
  : document_parser::base_type(document)
  , in_prolog(false)
  , tbl()
  , vp()
{
  typedef cet::includer::const_iterator iter_t;
  name     = fhicl::ass [ _val = qi::_1 ];
  qualname = fhicl::ass                                  [ _val = qi::_1 ]
             >> *((char_('.') > fhicl::ass)               [ _val += qi::_1 + qi::_2 ]
                  | (char_('[') > fhicl::uint > char_(']')) [ _val += qi::_1 + qi::_2 + qi::_3]
                 );  // Whitespace permitted before, and around delimiters ( '.', '[', ']').
  noskip_qualname = no_skip [ fhicl::ass ]                [ _val = qi::_1 ]
             >> *((char_('.') > fhicl::ass)               [ _val += qi::_1 + qi::_2 ]
                  | (char_('[') > fhicl::uint > char_(']')) [ _val += qi::_1 + qi::_2 + qi::_3]
                 );  // Whitespace permitted around delimiters ('.', '[', ']') only.
  localref = lit("@local::") > noskip_qualname [ _val = qi::_1 ];
  dbref    = lit("@db::") > noskip_qualname [ _val = qi::_1 ];
  // Can't use simple, "list context" due to the possibility of one of
  // the list elements actually returning multiple elements.
  sequence =
    lit('[')
    > -(((value [ phx::bind(seq_insert_value, qi::_1, _val) ]) | (iter_pos >> lit("@sequence::") > noskip_qualname) [ phx::bind(&seq_insert_sequence<iter_t>, qi::_2, ref(tbl), ref(in_prolog), _val, qi::_1, s) ]))
    > *(lit(',') > ((value [ phx::bind(seq_insert_value, qi::_1, _val) ]) | (iter_pos >> lit("@sequence::") > noskip_qualname) [ phx::bind(&seq_insert_sequence<iter_t>, qi::_2, ref(tbl), ref(in_prolog), _val, qi::_1, s) ]))
    > lit(']');
  table =
    lit('{')
    > *((name >> (lit(':') >> value)
        ) [ phx::bind(map_insert, qi::_1, qi::_2, _val) ]
        | (name >> (lit(':') > lit("@erase"))
          ) [ phx::bind(map_erase, qi::_1, _val) ]
        | (iter_pos >> lit("@table::") > noskip_qualname
          ) [ phx::bind(&insert_table<table_t, iter_t>,
                        qi::_2, ref(tbl), ref(in_prolog), _val,
                        qi::_1, s) ]
       )
    > lit('}');
  value =
    (vp.nil     [ _val = phx::bind(xvalue, ref(in_prolog), NIL     , qi::_1) ] |
     vp.boolean [ _val = phx::bind(xvalue, ref(in_prolog), BOOL    , qi::_1) ] |
     vp.number  [ _val = phx::bind(xvalue, ref(in_prolog), NUMBER  , qi::_1) ] |
     vp.complex [ _val = phx::bind(xvalue, ref(in_prolog), COMPLEX , qi::_1) ] |
     vp.string  [ _val = phx::bind(xvalue, ref(in_prolog), STRING  , qi::_1) ] |
     (iter_pos >> localref)
     [ _val = phx::bind(&local_lookup<iter_t>,
                        qi::_2, ref(tbl), ref(in_prolog),
                        qi::_1, s) ] |
     (iter_pos >> dbref)
     [ _val = phx::bind(&database_lookup<iter_t>,
                        qi::_2, ref(tbl), ref(in_prolog),
                        qi::_1, s) ] |
     vp.id      [ _val = phx::bind(xvalue, ref(in_prolog), TABLEID , qi::_1) ] |
     sequence   [ _val = phx::bind(xvalue, ref(in_prolog), SEQUENCE, qi::_1) ] |
     table      [ _val = phx::bind(xvalue, ref(in_prolog), TABLE   , qi::_1) ]
    );
  prolog =
    lit("BEGIN_PROLOG") [ phx::bind(rebool, ref(in_prolog), true) ]
    >> *((qualname
          >> (lit(':') > value)
         ) [ phx::bind(tbl_insert, qi::_1, qi::_2, ref(tbl)) ]
         | (iter_pos >> lit("@table::") > noskip_qualname
           ) [ phx::bind(&insert_table<fhicl::intermediate_table, iter_t>,
                         qi::_2, ref(tbl), ref(in_prolog), ref(tbl),
                         qi::_1, s) ]
        )
    >> lit("END_PROLOG")  [ phx::bind(rebool, ref(in_prolog), false) ];
  document = (*prolog)
             >> *((qualname >> (lit(':') >> value)
                  ) [ phx::bind(tbl_insert, qi::_1, qi::_2, ref(tbl)) ]
                  | (qualname >> (lit(':') > lit("@erase"))
                    ) [ phx::bind(tbl_erase, qi::_1, ref(tbl)) ]
                  | (iter_pos >> lit("@table::") > noskip_qualname
                    ) [ phx::bind(&insert_table<fhicl::intermediate_table, iter_t>,
                                  qi::_2, ref(tbl), ref(in_prolog), ref(tbl),
                                  qi::_1, s) ]
                 );
  name    .name("name atom");
  localref.name("localref atom");
  dbref   .name("dbref atom");
  qualname.name("qualified name");
  noskip_qualname.name("qualified name (no pre-skip)");
  sequence.name("sequence");
  table   .name("table");
  value   .name("value");
  prolog  .name("prolog");
  document.name("document");
}  // document_parser c'tor

// ----------------------------------------------------------------------

bool
fhicl::parse_value_string(std::string const & s
                          , extended_value   &  result
                          , std::string    &    unparsed
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
  bool const b =  qi::phrase_parse(begin, end
                                   , p >> *whitespace
                                   , whitespace
                                   , result
                                  )
                  && begin == end;
  unparsed = std::string(begin, end);
  return b;
}  // parse_value_string()

// ----------------------------------------------------------------------

void
fhicl::parse_document(std::string const  &  filename
                      , cet::filepath_maker & maker
                      , intermediate_table  & result
                     )
{
  cet::includer s(filename, maker);
  typedef  cet::includer::const_iterator  iter_t;
  typedef  qi::rule<iter_t>  ws_t;
  ws_t  whitespace = space
                     | lit('#')  >> *(char_ - eol) >> eol
                     | lit("//") >> *(char_ - eol) >> eol;
  document_parser<iter_t, ws_t> p(s);
  iter_t                        begin = s.begin();
  iter_t const                  end  =  s.end();
  bool b = false;
  try {
    b =  qi::phrase_parse(begin, end, p, whitespace);
  }
  catch (qi::expectation_failure<iter_t> const & e) {
    begin = e.first;
  }
  std::string unparsed(begin, end);
  if (b && unparsed.empty())
  { result = p.tbl; }
  else
    throw fhicl::exception(fhicl::parse_error, "detected at or near")
        << s.highlighted_whereis(begin)
        << "\n";
}  // parse_document()

// ----------------------------------------------------------------------

void
fhicl::parse_document(std::istream     &    is
                      , cet::filepath_maker & maker
                      , intermediate_table  & result
                     )
{
  cet::includer s(is, maker);
  typedef  cet::includer::const_iterator  iter_t;
  typedef  qi::rule<iter_t>  ws_t;
  ws_t  whitespace = space
                     | lit('#')  >> *(char_ - eol) >> eol
                     | lit("//") >> *(char_ - eol) >> eol;
  document_parser<iter_t, ws_t> p(s);
  iter_t                        begin = s.begin();
  iter_t const                  end  =  s.end();
  bool b = false;
  try {
    b =  qi::phrase_parse(begin, end, p, whitespace);
  }
  catch (qi::expectation_failure<iter_t> const & e) {
    begin = e.first;
  }
  std::string unparsed(begin, end);
  if (b && unparsed.empty())
  { result = p.tbl; }
  else
    throw fhicl::exception(fhicl::parse_error, "detected at or near")
        << s.highlighted_whereis(begin)
        << "\n";
}  // parse_document()

// ======================================================================
