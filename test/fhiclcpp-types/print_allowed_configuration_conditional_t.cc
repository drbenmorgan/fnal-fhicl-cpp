#include "fhiclcpp/types/Table.h"
#include "test/fhiclcpp-types/CondConfig.h"
#include <iostream>

using namespace fhicl;

int main()
{
  Table<CondConfig> table { Name("pset") };
  table.print_allowed_configuration(std::cout);
}
