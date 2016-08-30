// ======================================================================
//
// Test KeysToIgnore functionality of table
//
// ======================================================================

#define BOOST_TEST_MODULE (keys_to_ignore)

#include "cetlib/quiet_unit_test.hpp"

#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/make_ParameterSet.h"
#include "fhiclcpp/types/Table.h"

using namespace fhicl;
using namespace fhicl::detail;
using namespace std;

namespace {
  // Config to test against is empty.
  struct Config {
    struct KeysToIgnore {
      std::set<std::string> operator()()
      {
        return {"testing"};
      }
    };
  };
}

BOOST_AUTO_TEST_SUITE(keys_to_ignore_t)

BOOST_AUTO_TEST_CASE(simple_case)
{
  std::string const config {"testing: \"it works\""};
  ParameterSet pset;
  make_ParameterSet(config, pset);
  auto test = Table<Config,Config::KeysToIgnore>{pset};
  test.print_allowed_configuration(std::cout);
}

BOOST_AUTO_TEST_CASE(simple_case_2)
{
  std::string const config {"testing: { if: \"it works\" }"};
  ParameterSet pset;
  make_ParameterSet(config, pset);
  auto test = Table<Config>{pset, Config::KeysToIgnore{}()};
  test.print_allowed_configuration(std::cout);
}

BOOST_AUTO_TEST_SUITE_END()
