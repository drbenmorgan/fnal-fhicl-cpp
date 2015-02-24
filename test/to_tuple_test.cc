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
#include <tuple>
#include <typeinfo>
#include <vector>

using namespace fhicl;

namespace {

  template <typename T>
  void print( T const & t ) {
    std::cout << std::setw(15) << std::left << cet::demangle_symbol( typeid(t).name() ) << " with value \"" << t << "\"" << std::endl;
  }

}

int main()
{

  putenv(const_cast<char*>("FHICL_FILE_PATH=./test:."));
  cet::filepath_lookup policy("FHICL_FILE_PATH");
  std::string const in("to_tuple_test.fcl");

  fhicl::intermediate_table tbl1;
  fhicl::parse_document(in, policy, tbl1);
  fhicl::ParameterSet pset;
  fhicl::make_ParameterSet(tbl1, pset);

  // warmup
  auto const vs             = pset.get<std::vector<std::string>>( "vec" );
  auto const tup_as_strings = pset.get<std::vector<std::string>>( "tup" );

  // to tuple
  auto const tup = pset.get<std::tuple<int,std::string,double>>( "tup" );

  print( std::get<0>(tup) );
  print( std::get<1>(tup) );
  print( std::get<2>(tup) );

  // vector of tuples
  auto const composers = pset.get<std::vector<std::tuple<std::size_t,std::string,unsigned,double>>>("tup2");

  for ( const auto & composer : composers ) {
    std::cout << std::endl;
    print( std::get<0>( composer ) );
    print( std::get<1>( composer ) );
    print( std::get<2>( composer ) );
    print( std::get<3>( composer ) );
  }

  // test error
  try {
    auto const err1 = pset.get<std::vector<std::tuple<std::size_t,std::string,unsigned> > >("tup2");
  }
  catch(const fhicl::exception& e){
    std::cout << e.what() << std::endl;
  }

  // tuple of tuples
  auto const factoids = pset.get<
  std::tuple<std::tuple<std::string,std::string,std::string,std::string>,
    std::tuple<std::string,std::string,std::string,std::string>,
    std::tuple<std::string,std::string,std::string,std::string,unsigned,unsigned,unsigned> >>("tup3");

  const auto nine_symphonies = std::get<0>( factoids );
  std::cout << std::endl;
  print ( std::get<0>( nine_symphonies ) );
  print ( std::get<1>( nine_symphonies ) );
  print ( std::get<2>( nine_symphonies ) );
  print ( std::get<3>( nine_symphonies ) );

  const auto less_than_9_symphonies = std::get<1>( factoids );
  std::cout << std::endl;
  print ( std::get<0>( less_than_9_symphonies ) );
  print ( std::get<1>( less_than_9_symphonies ) );
  print ( std::get<2>( less_than_9_symphonies ) );

  const auto piano_sonatas = std::get<2>( factoids );
  std::cout << std::endl;
  print ( std::get<0>( piano_sonatas ) );
  print ( std::get<1>( piano_sonatas ) );
  print ( std::get<2>( piano_sonatas ) );
  print ( std::get<3>( piano_sonatas ) );
  print ( std::get<4>( piano_sonatas ) );
  print ( std::get<5>( piano_sonatas ) );
  print ( std::get<6>( piano_sonatas ) );


}
