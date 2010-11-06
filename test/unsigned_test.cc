// ======================================================================
//
// test unsigned integral atoms
//
// ======================================================================


#include "fhiclcpp/ParameterSet.h"
#include <cstdlib>
#include <string>

using fhicl::ParameterSet;
using std::string;

typedef  unsigned int   uint;
typedef  unsigned long  ulong;


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
    pset.put<uint>( "u11", 0u );
    ensure( 11, pset.get<string>("u11") == "0" );
    ensure( 12, pset.to_string() == "u11:0" );
    ensure( 13, pset.get<uint>("u11") == 0u );
  }

  {
    ParameterSet pset;
    pset.put<string>( "u21", "000" );
    ensure( 21, pset.to_string() == "u21:000" );
    uint u21 = pset.get<uint>("u21");
    ensure( 22, u21 == 0u );
    pset.put<uint>( "u23", u21 );
    ensure( 23, pset.get<string>("u23") == "0" );
    ensure( 24, pset.to_string() == "u21:000 u23:0" );
  }

  {
    ParameterSet pset;
    pset.put<string>( "u31", "1.2e+1" );
    ensure( 31, pset.to_string() == "u31:1.2e+1" );
    ensure( 32, pset.get<uint>("u31") == 12u );
  }

  {
    ParameterSet pset;
    pset.put<ulong>( "u41", 123456u );
    ensure( 41, pset.get<string>("u41") == "123456" );
    ensure( 42, pset.to_string() == "u41:123456" );
    ensure( 43, pset.get<ulong>("u41") == 123456u );
    pset.put<ulong>( "u44", 1234567u );
    ensure( 44, pset.get<string>("u44") == "1.234567e+6" );
    ensure( 45, pset.to_string() == "u41:123456 u44:1.234567e+6" );
    ensure( 46, pset.get<ulong>("u44") == 1234567u );
  }

  return 0;

}  // main()
