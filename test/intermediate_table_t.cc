#define BOOST_TEST_MODULE ( intermediate_table_t )
#include "boost/test/auto_unit_test.hpp"

#include "fhiclcpp/intermediate_table.h"
#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/make_ParameterSet.h"

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

using namespace fhicl;

BOOST_AUTO_TEST_SUITE(intermediate_table_t)

BOOST_AUTO_TEST_CASE(main)
{
  intermediate_table table;
  table.put("string1", "string");
  table.put("string2", std::string("string"));
  table.put("complex", std::complex<double>(3,0.5));
  table.put("sequence", std::vector<int> { 5, 10, 15, 20, 25 } );
  table.put("double", 28.5);
  table.put("sequence[6]", 35);
  table.put("table.val1", 92);
  table.put("table.t1.val", "oops");
  BOOST_CHECK_NO_THROW(table.erase("table.t1.val"));
  BOOST_CHECK_THROW(table.erase("table.val1.valX"), fhicl::exception);
  BOOST_CHECK(!table.exists("table.t1.val"));
  BOOST_CHECK_EQUAL(table.get<std::string>("string1"), "string");
  BOOST_CHECK_EQUAL(table.get<std::string>("string2"), "string");
  ParameterSet pset;
  make_ParameterSet(table, pset);
  std::cout << pset.to_indented_string() << std::endl;
}

BOOST_AUTO_TEST_SUITE_END()
