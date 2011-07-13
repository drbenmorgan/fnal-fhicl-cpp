#define BOOST_TEST_MODULE ( document test )

#include "boost/test/auto_unit_test.hpp"

#include "fhiclcpp/intermediate_table.h"
#include "fhiclcpp/parse.h"
#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/make_ParameterSet.h"
#include <string>
#include <fstream>
#include <iostream>
using namespace fhicl;
using namespace std;

// Contents from file
string content("");

// Parsed parameter sets
ParameterSet ps1;
ParameterSet ps2;

// Function to read contents from a file
   void readFile(string fileName, string & content)
   {
      ifstream in;
      string line("");
      in.open(fileName.c_str());
      while(!in.eof() && in.good())
      {
         getline(in, line);

         // Append newline since getline is delimeted by newline.
         content.append("\n");
         content.append(line);
     }
   } // readFile()

BOOST_AUTO_TEST_SUITE( document_test )
BOOST_AUTO_TEST_CASE( doc )
{
   ofstream out("temp.txt");
   readFile(boost::unit_test::framework::master_test_suite().argv[1], content);
   intermediate_table table;
   parse_document(content, table);
   make_ParameterSet(table, ps1);
   make_ParameterSet(table, ps2);
   cout << ps1.to_string() << endl;
   BOOST_CHECK_EQUAL(ps1, ps2);
} // main()
BOOST_AUTO_TEST_SUITE_END()

