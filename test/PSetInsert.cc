#include <string>
#include <vector>
#include <iostream>

#include <assert.h>

#include "ParameterSet.h"

int main()
{
  fhicl::ParameterSet pset;

  pset.addInt("a", -1);
  assert(pset.getInt("a") == -1);
  assert(pset.getUInt("a", 0) == 0U);

  pset.addString("b", "string");
  assert(pset.getString("b") == "string");

  pset.addDouble("c", 2.3);
  assert(pset.getDouble("c") == 2.3);

  pset.addBool("d", true);
  assert(pset.getBool("d") == true);

  std::vector<int> vi;
  vi.push_back(-1);
  vi.push_back(-2);
  vi.push_back(-3);
  pset.addVInt("e", vi);
  assert(pset.getVInt("e")[1] == -2);
  assert(pset.getVUInt("e").empty());

  std::vector<double> vd;
  vd.push_back(0.1);
  vd.push_back(0.2);
  vd.push_back(0.3);
  pset.addVDouble("f", vd);
  assert(pset.getVDouble("f")[1] == 0.2);

  std::vector<std::string> vs;
  vs.push_back("str1");
  vs.push_back("str2");
  vs.push_back("str3");
  pset.addVString("g", vs);

  fhicl::ParameterSet pset2;
  pset2.addInt("a2", 1);
  pset2.addString("b2", "bstring");
  pset.addPSet("h", pset2);

  pset.addUInt("u1", 12);
  assert (pset.getUInt("u1") == 12);

  pset.addUInt("u2", 0);
  assert (pset.getUInt("u2") == 0);

  std::vector<unsigned int> vui;
  vui.push_back(1);
  vui.push_back(2);
  vui.push_back(3);
  pset.addVUInt("vu", vui);
  assert(pset.getVInt("vu")[1] == 2);
  assert(pset.getVUInt("vu")[1] == 2);

  pset.addBool("b1", true);
  assert(pset.getBool("b1") == true);

  pset.addBool("b2", false);
  assert(pset.getBool("b2") == false);

  pset.addString("b3", "true");
  assert(pset.getBool("b3") == true);
  pset.addString("b4", "false");
  assert(pset.getBool("b4") == false);

  //pset.print();

  return 0;
}
