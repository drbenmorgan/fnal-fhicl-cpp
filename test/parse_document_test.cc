// ======================================================================
//
// test ParameterSet values
//
// ======================================================================

#define BOOST_TEST_MODULE ( document test )

#include "boost/test/auto_unit_test.hpp"
#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/intermediate_table.h"
#include "fhiclcpp/make_ParameterSet.h"
#include "fhiclcpp/parse.h"
#include <string>
#include <iostream>
using namespace fhicl;
using namespace std;

BOOST_AUTO_TEST_SUITE( document_test )

BOOST_AUTO_TEST_CASE( empty_document )
{
  std::string document;
  intermediate_table tbl;
  BOOST_REQUIRE_NO_THROW(parse_document(document, tbl));
  ParameterSet pset;
  make_ParameterSet(tbl, pset);
  BOOST_CHECK( pset.is_empty() );
}

BOOST_AUTO_TEST_CASE( nonempty_document )
{
  std::string document = "a : 1\n"
                         "b : 2\n"
                         ;
  intermediate_table tbl;
  BOOST_REQUIRE_NO_THROW( parse_document(document, tbl) );
  ParameterSet pset;
  make_ParameterSet(tbl, pset);
  BOOST_CHECK( ! pset.is_empty() );
  BOOST_CHECK_EQUAL( pset.get<int>("a"), 1 );
  BOOST_CHECK_EQUAL( pset.get<int>("b"), 2 );
  BOOST_CHECK_EQUAL( pset.get<unsigned>("a"), 1u );
  BOOST_CHECK_EQUAL( pset.get<unsigned>("b"), 2u );
  BOOST_CHECK_EQUAL( pset.get<string>("a"), "1" );
  BOOST_CHECK_EQUAL( pset.get<string>("b"), "2" );
}

BOOST_AUTO_TEST_CASE( nested_document )
{
  std::string document = "x.a : 1\n"
                         "x.b : 2\n"
                         ;
  intermediate_table tbl;
  BOOST_REQUIRE_NO_THROW( parse_document(document, tbl) );
  ParameterSet pset;
  make_ParameterSet(tbl, pset);
  BOOST_CHECK( ! pset.is_empty() );
  BOOST_REQUIRE_NO_THROW( pset.get<ParameterSet>("x") );
  BOOST_REQUIRE( ! pset.get<ParameterSet>("x").is_empty() );
  BOOST_CHECK_EQUAL( pset.get<int>("x.a"), 1 );
  BOOST_CHECK_EQUAL( pset.get<int>("x.b"), 2 );
  BOOST_CHECK_EQUAL( pset.get<unsigned>("x.a"), 1u );
  BOOST_CHECK_EQUAL( pset.get<unsigned>("x.b"), 2u );
  BOOST_CHECK_EQUAL( pset.get<string>("x.a"), "1" );
  BOOST_CHECK_EQUAL( pset.get<string>("x.b"), "2" );
}

BOOST_AUTO_TEST_CASE( badly_nested_document )
{
  std::string document = "{ x.a : 1\n"
                         "  x.b : 2\n"
                         "}\n"
                         ;
  intermediate_table tbl;
  BOOST_CHECK_THROW( parse_document(document, tbl), cet::exception );
}

BOOST_AUTO_TEST_CASE( overridden_prolog_document )
{
  std::string document = "BEGIN_PROLOG\n"
                         "  a: 1\n"
                         "  t: { a : 11\n"
                         "       b : 12\n"
                         "     }\n"
                         "END_PROLOG\n"
                         "a   : 2\n"
                         "t.a : @local::t.b\n"
                         ;
  intermediate_table tbl;
  BOOST_REQUIRE_NO_THROW(parse_document(document, tbl));
  ParameterSet pset;
  make_ParameterSet(tbl, pset);
  BOOST_CHECK_EQUAL( pset.get<int>("a"), 2 );
  BOOST_REQUIRE_NO_THROW( pset.get<ParameterSet>("t") );
  BOOST_REQUIRE( ! pset.get<ParameterSet>("t").is_empty() );
  BOOST_CHECK_EQUAL( pset.get<int>("t.a"), 12 );
  BOOST_CHECK_THROW( pset.get<int>("t.b"), cet::exception );
}

BOOST_AUTO_TEST_CASE ( contiguous_prolog )
{
  std::string document = "BEGIN_PROLOG\n"
                         "  a: 1\n"
                         "  t: { a : 11\n"
                         "       b : 12\n"
                         "     }\n"
                         "END_PROLOG\n"
                         "BEGIN_PROLOG\n"
                         " c: 47\n"
                         "END_PROLOG\n";
  intermediate_table tbl;
  BOOST_REQUIRE_NO_THROW(parse_document(document, tbl));
  BOOST_CHECK(tbl.exists("c"));
}

BOOST_AUTO_TEST_CASE ( noncontiguous_prolog )
{
  std::string document = "BEGIN_PROLOG\n"
                         "  a: 1\n"
                         "  t: { a : 11\n"
                         "       b : 12\n"
                         "     }\n"
                         "END_PROLOG\n"
                         "d: 27\n"
                         "BEGIN_PROLOG\n"
                         " c: 47\n"
                         "END_PROLOG\n";
  intermediate_table tbl;
  BOOST_REQUIRE_THROW(parse_document(document, tbl), cet::exception);
}

BOOST_AUTO_TEST_CASE( overridden_toplevel_document )
{
  std::string document = "a : 1\n"
                         "b : 2\n"
                         "a : 3\n"
                         "c : 4\n"
                         "b : 5\n"
                         "a : 6\n"
                         ;
  intermediate_table tbl;
  BOOST_REQUIRE_NO_THROW( parse_document(document, tbl) );
  ParameterSet pset;
  make_ParameterSet(tbl, pset);
  BOOST_CHECK_EQUAL( pset.get<int>("a"), 6 );
  BOOST_CHECK_EQUAL( pset.get<int>("b"), 5 );
  BOOST_CHECK_EQUAL( pset.get<int>("c"), 4 );
}

BOOST_AUTO_TEST_CASE( overridden_nested_document )
{
  std::string document = "t : { a : 1\n"
                         "      b : 2\n"
                         "      a : 3\n"
                         "      c : 4\n"
                         "      b : 5\n"
                         "      a : 6\n"
                         "    }\n"
                         ;
  intermediate_table tbl;
  BOOST_REQUIRE_NO_THROW( parse_document(document, tbl) );
  ParameterSet pset;
  make_ParameterSet(tbl, pset);
  BOOST_CHECK_EQUAL( pset.get<int>("t.a"), 6 );
  BOOST_CHECK_EQUAL( pset.get<int>("t.b"), 5 );
  BOOST_CHECK_EQUAL( pset.get<int>("t.c"), 4 );
}

BOOST_AUTO_TEST_CASE( nil_value )
{
  std::string document = "a : @nil\n"
                         "b : nil\n"
                         "c : \"@nil\"\n"
                         "d : \"nil\"\n"
                         "t : { a : @nil\n"
                         "      b : nil\n"
                         "      c : \"@nil\"\n"
                         "      d : \"nil\"\n"
                         "    }\n"
                         ;
  intermediate_table tbl;
  BOOST_REQUIRE_NO_THROW( parse_document(document, tbl) );
  ParameterSet pset;
  make_ParameterSet(tbl, pset);

  typedef void * nil_t;
  nil_t nil_value = 0;

  BOOST_CHECK_EQUAL( pset.get<nil_t >("a"  ), nil_value );
  BOOST_CHECK_EQUAL( pset.get<nil_t >("t.a"), nil_value );
  BOOST_CHECK_THROW( pset.get<string>("a"  ), fhicl::exception );
  BOOST_CHECK_THROW( pset.get<string>("t.a"), fhicl::exception );
  BOOST_CHECK_EQUAL( pset.get<string>("b"  ), "nil");
  BOOST_CHECK_EQUAL( pset.get<string>("t.b"), "nil");
  BOOST_CHECK_EQUAL( pset.get<string>("c"  ), "@nil");
  BOOST_CHECK_EQUAL( pset.get<string>("t.c"), "@nil");
  BOOST_CHECK_EQUAL( pset.get<string>("d"  ), "nil");
  BOOST_CHECK_EQUAL( pset.get<string>("t.d"), "nil");
}

BOOST_AUTO_TEST_CASE ( erase_value )
{
  std::string document = "a: 27\n"
                         "b: { x: 7 y: 12 }\n"
                         "c: { x: 7 y: 12 x: @erase }\n"
                         "a: @erase\n"
                         "b.x: @erase\n";
  intermediate_table tbl;
  BOOST_REQUIRE_NO_THROW(parse_document(document, tbl));
  BOOST_CHECK(!tbl.exists("a"));
  BOOST_CHECK(tbl.exists("b"));
  BOOST_CHECK(!tbl.exists("b.x"));
  BOOST_CHECK(tbl.exists("b.y"));
  BOOST_CHECK(tbl.exists("c"));
  BOOST_CHECK(!tbl.exists("c.x"));
  BOOST_CHECK(tbl.exists("c.y"));
}

BOOST_AUTO_TEST_CASE( expand_table )
{
  std::string document = "BEGIN_PROLOG\n"
                         "fred: { bill: twelve charlie: 27 }\n"
                         "END_PROLOG\n"
                         "x: { ethel: 14 bill: 12 @table::fred }\n"
                         "@table::fred\n";
  intermediate_table tbl;
  parse_document(document, tbl);
  BOOST_CHECK(tbl.exists("x.ethel"));
  BOOST_CHECK(tbl.exists("x.charlie"));
  BOOST_CHECK(tbl.exists("bill"));
  BOOST_CHECK(tbl.exists("charlie"));
  BOOST_CHECK_EQUAL(tbl.get<std::string>("x.bill"), std::string("twelve"));
  BOOST_CHECK_EQUAL(tbl.get<std::string>("bill"), std::string("twelve"));
}

BOOST_AUTO_TEST_CASE( expand_sequence )
{
  std::string document = "BEGIN_PROLOG\n"
                         "fred: [ three, four, five ]\n"
                         "END_PROLOG\n"
                         "bill: [ one, two, @sequence::fred, six ]\n"
                         "charlie: @local::fred\n"
                         "ethel: [ @sequence::fred, six ]\n";
  intermediate_table tbl;
  parse_document(document, tbl);
  BOOST_CHECK(tbl.exists("fred"));
  BOOST_CHECK(tbl.exists("bill"));
  BOOST_CHECK(tbl.exists("charlie"));
  BOOST_CHECK(tbl.exists("ethel"));

  ParameterSet pset;
  make_ParameterSet(tbl, pset);

  BOOST_CHECK_EQUAL(pset.get<std::vector<std::string> >("charlie").size(), 3ul);

  std::vector<std::string> const billref { "one", "two", "three", "four", "five", "six" };
  std::vector<std::string> const ethelref { "three", "four", "five", "six" };

  auto cmp = [] (std::vector<std::string> const & seq, std::vector<std::string> const & ref)
  {
    BOOST_CHECK_EQUAL(seq.size(), ref.size());
    for (auto i = seq.cbegin(), e = seq.cend(), iref = ref.cbegin(); i != e; ++i, ++iref) {
      BOOST_CHECK_EQUAL(*i, *iref);
    }
  };

  cmp(pset.get<std::vector<std::string> >("bill"), billref);
  cmp(pset.get<std::vector<std::string> >("ethel"), ethelref);
}

BOOST_AUTO_TEST_CASE( bad_lookup )
{
  std::string document = "x: @local::dead\n";
  intermediate_table tbl;
  BOOST_CHECK_THROW(parse_document(document, tbl), cet::exception);
}

BOOST_AUTO_TEST_CASE( bad_expand_table )
{
  std::string document = "BEGIN_PROLOG\n"
                         "bad: John\n"
                         "END_PROLOG\n"
                         "@table::bad\n";
  intermediate_table tbl;
  BOOST_CHECK_THROW(parse_document(document, tbl), cet::exception);
}

BOOST_AUTO_TEST_CASE( bad_expand_sequence )
{
  std::string document = "BEGIN_PROLOG\n"
                         "bad: John\n"
                         "END_PROLOG\n"
                         "f: [ @sequence::bad ]\n";
  intermediate_table tbl;
  BOOST_CHECK_THROW(parse_document(document, tbl), cet::exception);
}

BOOST_AUTO_TEST_CASE(colon_spacing)
{
  std::string const prolog = "BEGIN_PROLOG\n"
                             "t: { a: 7 b: 6}\n"
                             "s: [ 7, 6, 7 ]\n"
                             "END_PROLOG\n";
  intermediate_table tbl;
  std::vector<std::string> refs { "t1: @local::t\n",
      "@table::t\n",
      "s2: [ 1, 2, @sequence::s ]\n",
      "a: @id::0001020304050607080910111213141516171819\n",
      "t1: { t2: @local::t }\n",
      "t1: { @table::t }\n",
      "t1: { s1: [ 1, 2, 3, @sequence::s ] }\n",
      "t1: { a: @id::0001020304050607080910111213141516171819 }\n",
      };
  for (auto const & ref : refs) {
//    BOOST_CHECK_NO_THROW(parse_document(prolog + ref, tbl));
    std::cerr << ref;
    parse_document(prolog + ref, tbl);
    auto const cpos = ref.find("::");
    BOOST_REQUIRE(cpos != std::string::npos);
    std::string bad1 { ref };
    std::string bad2 { ref };
    bad1.insert(cpos, " ");
    BOOST_CHECK_THROW(parse_document(prolog + bad1, tbl), cet::exception);
    bad2.insert(cpos + 2, " ");
    BOOST_CHECK_THROW(parse_document(prolog + bad2, tbl), cet::exception);
  }
}

BOOST_AUTO_TEST_SUITE_END()
