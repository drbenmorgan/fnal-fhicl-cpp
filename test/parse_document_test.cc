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

using namespace fhicl;
using namespace std;

BOOST_AUTO_TEST_SUITE( document_test )

BOOST_AUTO_TEST_CASE( empty_document )
{
  std::string document;
  intermediate_table tbl;
  BOOST_CHECK_NO_THROW( parse_document(document, tbl) );
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
  BOOST_CHECK_NO_THROW( parse_document(document, tbl) );
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
  BOOST_CHECK_NO_THROW( parse_document(document, tbl) );
  ParameterSet pset;
  make_ParameterSet(tbl, pset);
  BOOST_CHECK( ! pset.is_empty() );
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

BOOST_AUTO_TEST_SUITE_END()
