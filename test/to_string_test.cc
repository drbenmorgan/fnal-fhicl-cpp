#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/intermediate_table.h"
#include "fhiclcpp/make_ParameterSet.h"
#include "fhiclcpp/parse.h"

#include <fstream>
#include <iostream>


int
  main()
{
  std::fstream in("Sample.cfg");
  if( ! in )
    throw "input file not found!";
  fhicl::intermediate_table tbl1;
  if( ! fhicl::parse_document(in, tbl1) )
    throw "parse_document() failure!";
  fhicl::ParameterSet pset1;
  if( ! fhicl::make_ParameterSet(tbl1, pset1) )
    throw "make_ParameterSet() failure!";

  std::string str;
  str = pset1.to_string();
  std::cerr << str << "\n";
  fhicl::intermediate_table tbl2;
  if( ! fhicl::parse_document(str, tbl2) )
    throw "parse_document() failure!";
  fhicl::ParameterSet pset2;
  if( ! fhicl::make_ParameterSet(tbl2, pset2) )
    throw "make_ParameterSet() failure!";

  assert(pset2.get<int>("a") == 1);
  assert(pset2.get<unsigned int>("a") == 1);
  assert(pset2.get<int>("b") == -1);
  assert(pset2.get<unsigned int>("b", 0) == 0);

  assert(pset2.get<double>("c") == 0.0);
  assert(pset2.get<double>("d") == 0.5);

  assert(pset2.get<std::string>("e").compare("rain") == 0);
  assert(pset2.get<std::string>("f").compare("32") == 0);
  assert(pset2.get<int>("f") == 32);

  assert(pset2.get<std::vector<int> >("g")[2] == 3);

  assert(pset2.get<std::vector<fhicl::ParameterSet> >("h")[1]
             .get<std::string>("h2").compare("h2") == 0);

  assert(pset2.get<fhicl::ParameterSet>("i")
             .get<fhicl::ParameterSet>("i1")
             .get<std::string>("i1_1").compare("test") == 0);

  return 0;
}  // main()
