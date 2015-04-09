// ======================================================================
//
// test ParameterSet::to_indented_string()
//
// ======================================================================

#define BOOST_TEST_MODULE ( to_indented_string test )

#include "boost/test/auto_unit_test.hpp"
#include "fhiclcpp/ParameterSet.h"
#include <functional>
#include <string>

using namespace fhicl;
using namespace std;

namespace{
  auto to_ind_str =
    [](auto pset){return pset.to_indented_string(0,false);};
}

BOOST_AUTO_TEST_SUITE( values_test )

BOOST_AUTO_TEST_CASE( atoms )
{
  ParameterSet pset;
  BOOST_CHECK( pset.is_empty() );
  BOOST_CHECK_EQUAL( to_ind_str(pset)
                   , ""
                   );

  pset.put<std::string>("a", "string1");
  BOOST_CHECK_EQUAL( to_ind_str(pset)
                   , "a: \"string1\"\n"
                   );

  pset.put<int>("b", -1234);
  BOOST_CHECK_EQUAL( to_ind_str(pset)
                   , "a: \"string1\"\n"
                     "b: -1234\n"
                   );

  pset.put<bool>("c", false);
  BOOST_CHECK_EQUAL( to_ind_str(pset)
                   , "a: \"string1\"\n"
                     "b: -1234\n"
                     "c: false\n"
                   );
}

BOOST_AUTO_TEST_CASE( sequences )
{
  typedef  std::vector<int> intv;
  intv v;
  ParameterSet pset;
  pset.put<intv>("a", v);
  BOOST_CHECK_EQUAL( to_ind_str(pset)
                   , "a: []\n"
                   );

  v.push_back( 11 );
  pset.put<intv>("b", v);
  BOOST_CHECK_EQUAL( to_ind_str(pset)
                   , "a: []\n"
                     "b: [ 11 ]\n"
                   );


  v.push_back( 12 );
  v.push_back( 13 );
  pset.put<intv>("c", v);
  BOOST_CHECK_EQUAL( to_ind_str(pset)
                   , "a: []\n"
                     "b: [ 11 ]\n"
                     "c: [ 11\n"
                     "   , 12\n"
                     "   , 13\n"
                     "   ]\n"
                   );
}

BOOST_AUTO_TEST_CASE( tables )
{
  ParameterSet pset;

  ParameterSet p;
  pset.put<ParameterSet>("p1", p);
  BOOST_CHECK_EQUAL( to_ind_str(pset)
                   , "p1: {}\n"
                   );

  p.put<std::string>("a", "string1");
  pset.put<ParameterSet>("p2", p);
  BOOST_CHECK_EQUAL( to_ind_str(pset)
                   , "p1: {}\n"
                     "p2: { a: \"string1\"\n"
                     "    }\n"
                   );

  p.put<int>("b", -1234);
  pset.put<ParameterSet>("p3", p);
  BOOST_CHECK_EQUAL( to_ind_str(pset)
                   , "p1: {}\n"
                     "p2: { a: \"string1\"\n"
                     "    }\n"
                     "p3: { a: \"string1\"\n"
                     "      b: -1234\n"
                     "    }\n"
                   );

  p.put<bool>("c", false);
  pset.put<ParameterSet>("p4", p);
  BOOST_CHECK_EQUAL( to_ind_str(pset)
                   , "p1: {}\n"
                     "p2: { a: \"string1\"\n"
                     "    }\n"
                     "p3: { a: \"string1\"\n"
                     "      b: -1234\n"
                     "    }\n"
                     "p4: { a: \"string1\"\n"
                     "      b: -1234\n"
                     "      c: false\n"
                     "    }\n"
                   );
}

BOOST_AUTO_TEST_CASE( combo )
{
  typedef  std::vector<int> intv;
  intv v;
  v.push_back( 11 );
  v.push_back( 12 );
  v.push_back( 13 );

  ParameterSet p;
  p.put<intv>("v", v);

  ParameterSet pset;
  pset.put<ParameterSet>("p", p);
  BOOST_CHECK_EQUAL( to_ind_str(pset)
                   , "p: { v: [ 11\n"
                     "        , 12\n"
                     "        , 13\n"
                     "        ]\n"
                     "   }\n"
                   );
}

BOOST_AUTO_TEST_SUITE_END()
