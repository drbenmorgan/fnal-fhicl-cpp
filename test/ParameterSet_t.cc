#define BOOST_TEST_MODULE ( ParameterSet test)

#include "boost/test/auto_unit_test.hpp"
#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/make_ParameterSet.h"
#include <cstddef>
#include <cstdlib>
#include <string>
#include <vector>

using namespace fhicl;

struct SampleConfigFixture {
   SampleConfigFixture();

   ParameterSet pset;
};

SampleConfigFixture::SampleConfigFixture() {
   putenv((char*)"FHICL_FILE_PATH=./test:.");
   cet::filepath_lookup policy("FHICL_FILE_PATH");
   intermediate_table tbl;
   std::string cfg_in("Sample.cfg");
   parse_document(cfg_in, policy, tbl);
   make_ParameterSet(tbl, pset);
}

BOOST_FIXTURE_TEST_SUITE ( sampleConfig, SampleConfigFixture )

BOOST_AUTO_TEST_CASE ( Local ) {
   fhicl::ParameterSet j;
   j.put<int>("y", -1);
   fhicl::ParameterSet orig(pset.get<fhicl::ParameterSet>("j"));
   BOOST_CHECK ( j == orig );
   BOOST_CHECK ( orig.get<int>("y") == -1 );
   BOOST_CHECK ( pset.get<std::vector<int> >("m")[0] == -1 );

   std::vector<std::string> const names = pset.get_keys();
   for( std::size_t k = 0; k != names.size(); ++k )
      BOOST_CHECK ( "x" != names[k] );
}

BOOST_AUTO_TEST_CASE ( DeepInjection ) {
   fhicl::ParameterSet l; l.put<int>("zz", -2);
   fhicl::ParameterSet k; k.put<fhicl::ParameterSet>("l", l);
   fhicl::ParameterSet orig(pset.get<fhicl::ParameterSet>("k"));
   BOOST_CHECK ( k == orig );
   BOOST_CHECK ( orig.get<fhicl::ParameterSet>("l")
                     .get<int>("zz") == -2 );
}

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

BOOST_AUTO_TEST_CASE (DeepVector) {
   typedef std::vector<std::vector<uint32_t> > vv_t;
   vv_t vv;
   BOOST_CHECK_NO_THROW( vv = pset.get<vv_t>("vv") );
   BOOST_CHECK( vv.front().back() == 3 );
   BOOST_CHECK( vv.back().front() == 2 );
   BOOST_CHECK( vv.back().back() == 4 );
}

BOOST_AUTO_TEST_SUITE_END()
