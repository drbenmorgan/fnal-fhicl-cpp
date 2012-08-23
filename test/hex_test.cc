#include "fhiclcpp/make_ParameterSet.h"
#include "fhiclcpp/ParameterSet.h"
#include <cassert>
#include <string>

int main()
{
  fhicl::ParameterSet ps;
  std::string text("small: 0xF large: 0X1000");
  fhicl::make_ParameterSet(text, ps);
  assert(ps.get<int>("small", 15));
  assert(ps.get<unsigned long>("large", 0x1000));
}
