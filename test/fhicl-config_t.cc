#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/intermediate_table.h"
#include "fhiclcpp/make_ParameterSet.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

using namespace fhicl;

int main(int argc, char **argv) {
   if (argc != 2) {
      std::cerr << "ERROR: expect exactly one argument.\n";
      exit(1);
   }
   putenv((char*)"FHICL_FILE_PATH=./test:.");
   std::ifstream cfg_in(argv[1]);
   intermediate_table tbl;
   parse_document(cfg_in, tbl);
   ParameterSet pset;
   make_ParameterSet(tbl, pset);
   std::cout << pset.to_string() << std::endl;
}
