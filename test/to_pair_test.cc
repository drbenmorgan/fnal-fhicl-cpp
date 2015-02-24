// ======================================================================
//
// test
//
// template<typename ... ARGS>
// void decode( boost::any const & a, std::tuple<ARGS...> & result );
//
// ======================================================================

#include "cetlib/demangle.h"

#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/intermediate_table.h"
#include "fhiclcpp/make_ParameterSet.h"
#include "fhiclcpp/parse.h"

#include <iomanip>
#include <iostream>
#include <cstdlib>
#include <string>
#include <typeinfo>
#include <utility>

using namespace fhicl;

int main()
{

  putenv(const_cast<char*>("FHICL_FILE_PATH=./test:."));
  cet::filepath_lookup policy("FHICL_FILE_PATH");
  std::string const in("to_pair_test.fcl");

  fhicl::intermediate_table tbl1;
  fhicl::parse_document(in, policy, tbl1);
  fhicl::ParameterSet pset;
  fhicl::make_ParameterSet(tbl1, pset);

  // warmup
  auto const vs             = pset.get<std::vector<std::string>>( "vec" );
  auto const tup_as_strings = pset.get<std::vector<std::string>>( "pair1" );

  // to pair
  auto const tup = pset.get<std::pair<unsigned,std::string>>( "pair1" );

  // to vector of pairs
  auto const vofp = pset.get<std::vector<std::pair<std::string,unsigned> > >("pair2");
  std::cout << std::endl;
  for ( const auto& entry: vofp ) {
    std::cout << entry.first << " " << entry.second << std::endl;
  }

  // to pair of pairs
  auto const pofp = pset.get<std::pair<std::pair<std::string,unsigned>,std::pair<std::string,unsigned> > >("pair2");
  std::cout << std::endl;
  std::cout << pofp.first.first << " " << pofp.first.second << std::endl
            << pofp.second.first << " " << pofp.second.second << std::endl;
  
  // test error
  try {
    auto const err1 = pset.get<std::pair<std::string,std::string> >("pair3");
  }
  catch(const fhicl::exception& e){
    std::cout << e.what() << std::endl;
  }

}
