#include <iostream>
#include "ParameterSet.h"
#include "ParameterSetParser.h"

int main()
{
  fhicl::ParameterSet pset;

  // parse a configuration file
  fhicl::Parser::Parse("Sample.cfg", pset);

  std::string s;
  s = pset.hashString();
  std::cout << s << "\n";

  std::cout << pset.get<fhicl::ParameterSet>("Lesson").get<int>("l2")<<"\n";

#if 0
  // retrieve the parameter "partition", with the default value 0 if not found
  assert (pset.get<int>("partition",0) == 101);

  // parse a configuration string
  std::string cnf = "MessageFacility : { partition : 3}";
  fhicl::Parser::ParseString(cnf, pset);
  // retrieve the parameter "partition", with the default value 0 if not found
  assert (pset.get<int>("partition",0) == 3);
#endif

  return 0;
}
