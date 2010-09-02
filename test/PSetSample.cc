#include <iostream>
#include "ParameterSet.h"
#include "ParameterSetParser.h"

int main()
{
  fhicl::ParameterSet pset;

  // parse a configuration file
  fhicl::Parser::Parse("Sample.cfg", pset);

  // retrieve the parameter "partition", with the default value 0 if not found
  assert (pset.getInt("partition",0) == 101);

  // parse a configuration string
  std::string cnf = "MessageFacility : { partition : 3}";
  fhicl::Parser::ParseString(cnf, pset);
  // retrieve the parameter "partition", with the default value 0 if not found
  assert (pset.getInt("partition",0) == 3);

  return 0;
}
