#include <string>
#include <vector>
#include <iostream>

#include "ParameterSet.h"

int main()
{
  fhicl::ParameterSet pset;

  pset.addInt("a", 1);
  pset.addString("b", "string");
  pset.addDouble("c", 2.3);
  pset.addBool("d", true);

  std::vector<int> vi;
  vi.push_back(1);
  vi.push_back(2);
  vi.push_back(3);
  pset.addVInt("e", vi);

  std::vector<double> vd;
  vd.push_back(0.1);
  vd.push_back(0.2);
  vd.push_back(0.3);
  pset.addVDouble("f", vd);

  std::vector<std::string> vs;
  vs.push_back("str1");
  vs.push_back("str2");
  vs.push_back("str3");
  pset.addVString("g", vs);

  fhicl::ParameterSet pset2;
  pset2.addInt("a2", 1);
  pset2.addString("b2", "bstring");
  pset.addPSet("h", pset2);

  pset.print();

  return 0;
}
