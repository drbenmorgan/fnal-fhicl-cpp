#define BOOST_TEST_MODULE ( ParameterSet test)
#include "boost/test/auto_unit_test.hpp"

#include "fhiclcpp/make_ParameterSet.h"
#include "fhiclcpp/ParameterSet.h"
#include <string>
#include <fstream>

using namespace fhicl;

struct SampleConfigFixture {
   SampleConfigFixture();

   ParameterSet pset;
};

SampleConfigFixture::SampleConfigFixture() {
   intermediate_table tbl;
   std::ifstream cfg_in("Sample.cfg");
   parse_document(cfg_in, tbl);
   make_ParameterSet(tbl, pset);
}

BOOST_FIXTURE_TEST_SUITE ( sampleConfig, SampleConfigFixture )

BOOST_AUTO_TEST_CASE ( DoubleStringMismatchDefaulted ) {
   try {
      pset.get<double>("e", 2.0);
      BOOST_FAIL ("Failed to throw an exception as expected");
   }
   catch (fhicl::exception &e) {
      BOOST_CHECK_MESSAGE ( e.categoryCode() == type_mismatch, \
                           "Category code should be type_mismatch" );
   }
   catch (...) {
      BOOST_FAIL ("Wrong exception type thrown");
   }
}

BOOST_AUTO_TEST_CASE ( DoubleStringMismatchNoDefault ) {
   try {
      pset.get<double>("e");
      BOOST_FAIL ("Failed to throw an exception as expected");
   }
   catch (fhicl::exception &e) {
      BOOST_CHECK_MESSAGE ( e.categoryCode() == type_mismatch, \
                           "Category code should be type_mismatch" );
   }
   catch (...) {
      BOOST_FAIL ("Wrong exception type thrown");
   }
}

BOOST_AUTO_TEST_CASE ( StringSuccess ) {
   BOOST_CHECK ( "rain" == pset.get<std::string>("e") );
}

BOOST_AUTO_TEST_CASE ( NotFound ) {
   try {
      pset.get<double>("zz");
      BOOST_FAIL ("Failed to throw an exception as expected");
   }
   catch (fhicl::exception &e) {
      BOOST_CHECK_MESSAGE ( e.categoryCode() == cant_find, \
                           "Category code should be cant_find" );
   }
   catch (...) {
      BOOST_FAIL ("Wrong exception type thrown");
   }
}


BOOST_AUTO_TEST_SUITE_END()
