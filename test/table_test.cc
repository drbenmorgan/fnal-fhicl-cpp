// ======================================================================
//
// test tables
//
// ======================================================================


#include "fhiclcpp/ParameterSet.h"
#include <cstdlib>
#include <string>

using fhicl::ParameterSet;
using std::complex;
using std::string;

typedef  double       dbl;
typedef  long double  ldbl;

typedef  complex<dbl>   cdbl;
typedef  complex<ldbl>  cldbl;


void
  ensure( int which, bool claim )
{ if( not claim )
    std::exit(which);
}


int
  main( )
{
  ParameterSet p0;
  ensure( 1, p0.to_string() == "" );
  try {
    p0.get<string>("a");
    ensure( 2, false );
  }
  catch( ... ) {
    ensure( 3, true );
  }

  ParameterSet p1;
  p1.put<string>( "b", "B" );
  p1.put<string>( "a", "A" );
  ensure( 11, p1.get<string>("a") == "A" );
  ensure( 12, p1.get<string>("b") == "B" );
  ensure( 13, p1.to_string() == "a:\"A\" b:\"B\"" );

  ParameterSet p2;
  p2.put<string>( "y", "Y" );
  p2.put<string>( "z", "Z" );
  p2.put<string>( "x", "X" );
  ensure( 21, p2.get<string>("x") == "X" );
  ensure( 22, p2.get<string>("y") == "Y" );
  ensure( 23, p2.get<string>("z") == "Z" );
  ensure( 24, p2.to_string() == "x:\"X\" y:\"Y\" z:\"Z\"" );

  ParameterSet p3;
  p3.put<ParameterSet>( "p2", p2 );
  p3.put<ParameterSet>( "p1", p1 );
  ensure( 31, p3.get<ParameterSet>("p1").get<string>("a") == "A" );
  ensure( 32, p3.get<ParameterSet>("p1").get<string>("b") == "B" );
  ensure( 33, p3.get<ParameterSet>("p2").get<string>("x") == "X" );
  ensure( 34, p3.get<ParameterSet>("p2").get<string>("y") == "Y" );
  ensure( 35, p3.get<ParameterSet>("p2").get<string>("z") == "Z" );
  ensure( 36, p3.to_string()
              == "p1:{a:\"A\" b:\"B\"} p2:{x:\"X\" y:\"Y\" z:\"Z\"}" );

  return 0;

}  // main()
