#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/Parser.h"
#include <iostream>


int main()
{
  fhicl::ParameterSet pset;

  // parse a configuration file
  fhicl::Parser::ParseFile("Sample.cfg", pset);

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
