// ======================================================================
//
// test floating-point atoms
//
// ======================================================================


#include "fhiclcpp/ParameterSet.h"
#include <cstdlib>
#include <limits>
#include <string>

using fhicl::ParameterSet;
using std::string;
using std::numeric_limits;

typedef  double       dbl;
typedef  long double  ldbl;


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
    pset.put<dbl>( "f11", 0.0 );
    ensure( 11, pset.get<string>("f11") == "0" );
    ensure( 12, pset.to_string() == "f11:0" );
    ensure( 13, pset.get<dbl>("f11") == 0.0 );
    pset.put<dbl>( "f14", 12.0 );
    ensure( 14, pset.get<string>("f14") == "12" );
    ensure( 15, pset.to_string() == "f11:0 f14:12" );
    ensure( 16, pset.get<dbl>("f14") == 12.0 );
    pset.put<dbl>( "f17", 12.3e1 );
    ensure( 17, pset.get<string>("f17") == "123" );
    ensure( 18, pset.to_string() == "f11:0 f14:12 f17:123" );
    ensure( 19, pset.get<dbl>("f17") == 123.0 );
  }

  {
    ParameterSet pset;
    pset.put<dbl>( "f21", 12.3456e4 );
    ensure( 21, pset.get<string>("f21") == "123456" );
    ensure( 22, pset.to_string() == "f21:123456" );
    ensure( 23, pset.get<dbl>("f21") == 123456.0 );
    pset.put<dbl>( "f24", 12.34567e5 );
    ensure( 24, pset.get<string>("f24") == "1.234567e+6" );
    ensure( 25, pset.to_string() == "f21:123456 f24:1.234567e+6" );
    ensure( 26, pset.get<dbl>("f24") == 1234567.0 );
  }

  {
    ParameterSet pset;
    pset.put<dbl>( "f31", -12.3456e4 );
    ensure( 31, pset.get<string>("f31") == "-123456" );
    ensure( 32, pset.to_string() == "f31:-123456" );
    ensure( 33, pset.get<dbl>("f31") == -123456.0 );
    pset.put<dbl>( "f34", -12.34567e5 );
    ensure( 34, pset.get<string>("f34") == "-1.234567e+6" );
    ensure( 35, pset.to_string() == "f31:-123456 f34:-1.234567e+6" );
    ensure( 36, pset.get<dbl>("f34") == -1234567.0 );
  }

  {
    ParameterSet pset;
    pset.put<dbl>( "f41", numeric_limits<dbl>::infinity() );
    ensure( 41, pset.get<string>("f41") == "+infinity" );
    ensure( 42, pset.to_string() == "f41:+infinity" );
    ensure( 43, pset.get<dbl>("f41") == numeric_limits<dbl>::infinity() );
    pset.put<dbl>( "f44", - numeric_limits<dbl>::infinity() );
    ensure( 44, pset.get<string>("f44") == "-infinity" );
    ensure( 45, pset.to_string() == "f41:+infinity f44:-infinity" );
    ensure( 46, pset.get<dbl>("f44") == - numeric_limits<dbl>::infinity() );
  }

  return 0;

}  // main()
