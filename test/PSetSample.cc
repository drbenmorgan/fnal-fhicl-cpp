#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/intermediate_table.h"
#include "fhiclcpp/make_ParameterSet.h"
#include "fhiclcpp/parse.h"

#include <fstream>
#include <iostream>


int main()
{
  // parse a configuration file; obtain intermediate form
  fhicl::intermediate_table tbl;
  std::fstream in("Sample.cfg");
  if( ! fhicl::parse_document(in, tbl) )
    throw "parse_document() failure!";

  // convert to ParameterSet
  fhicl::ParameterSet pset;
  if( ! fhicl::make_ParameterSet(tbl, pset) )
    throw "make_ParameterSet() failure!";

#if 0
  std::string s;
  s = pset.hashString();
  std::cout << s << "\n";
#endif

  assert(pset.get<int>("a") == 1);
  assert(pset.get<unsigned int>("a") == 1);
  assert(pset.get<int>("b") == -1);
  assert(pset.get<unsigned int>("b", 0) == 0);

  assert(pset.get<double>("c") == 0.0);
  assert(pset.get<double>("d") == 0.5);

  assert(pset.get<std::string>("e").compare("rain") == 0);
  assert(pset.get<std::string>("f").compare("32") == 0);
  assert(pset.get<int>("f") == 32);

  assert(pset.get<std::vector<int> >("g")[2] == 3);

  assert(pset.get<std::vector<fhicl::ParameterSet> >("h")[1]
             .get<std::string>("h2").compare("h2") == 0);

  assert(pset.get<fhicl::ParameterSet>("i")
             .get<fhicl::ParameterSet>("i1")
             .get<std::string>("i1_1").compare("test") == 0);

  return 0;
}
