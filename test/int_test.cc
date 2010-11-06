// ======================================================================
//
// test signed integral atoms
//
// ======================================================================


#include "fhiclcpp/ParameterSet.h"
#include <cstdlib>
#include <string>

using fhicl::ParameterSet;
using std::string;


void
  ensure( int which, bool claim )
{ if( not claim )
    std::exit(which);
}


int
  main( )
{
  {
    ParameterSet pset;
    pset.put<int>( "i1", 0 );
    ensure( 11, pset.get<string>("i1") == "0" );
    ensure( 12, pset.to_string() == "i1:0" );
    ensure( 13, pset.get<int>("i1") == 0 );
  }

  {
    ParameterSet pset;
    pset.put<string>( "i21", "000" );
    ensure( 21, pset.to_string() == "i21:000" );
    int i21 = pset.get<int>("i21");
    ensure( 22, i21 == 0 );
    pset.put<int>( "i23", i21 );
    ensure( 23, pset.get<string>("i23") == "0" );
    ensure( 24, pset.to_string() == "i21:000 i23:0" );
  }

  {
    ParameterSet pset;
    pset.put<string>( "i31", "-1.2e+1" );
    ensure( 31, pset.to_string() == "i31:-1.2e+1" );
    ensure( 32, pset.get<int>("i31") == -12 );
  }

  {
    ParameterSet pset;
    pset.put<long>( "i41", 123456 );
    ensure( 41, pset.get<string>("i41") == "123456" );
    ensure( 42, pset.to_string() == "i41:123456" );
    ensure( 43, pset.get<long>("i41") == 123456 );
    pset.put<long>( "i44", 1234567 );
    ensure( 44, pset.get<string>("i44") == "1.234567e+6" );
    ensure( 45, pset.to_string() == "i41:123456 i44:1.234567e+6" );
    ensure( 46, pset.get<long>("i44") == 1234567 );
  }

  {
    ParameterSet pset;
    pset.put<long>( "i51", -123456 );
    ensure( 51, pset.get<string>("i51") == "-123456" );
    ensure( 52, pset.to_string() == "i51:-123456" );
    ensure( 53, pset.get<long>("i51") == -123456 );
    pset.put<long>( "i54", -1234567 );
    ensure( 54, pset.get<string>("i54") == "-1.234567e+6" );
    ensure( 55, pset.to_string() == "i51:-123456 i54:-1.234567e+6" );
    ensure( 56, pset.get<long>("i54") == -1234567 );
  }

  return 0;

}  // main()
