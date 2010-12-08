// ======================================================================
//
// test complex atoms
//
// ======================================================================


#include "fhiclcpp/ParameterSet.h"
#include <complex>
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
  {
    ParameterSet pset;
    pset.put<cldbl>( "f11", cldbl() );
    ensure( 11, pset.get<string>("f11") == "(0,0)" );
    ensure( 12, pset.to_string() == "f11:(0,0)" );
    ensure( 13, pset.get<cldbl>("f11") == cldbl(0.0L,0.0L) );
    pset.put<cdbl>( "f14", cdbl() );
    ensure( 14, pset.get<string>("f14") == "(0,0)" );
    ensure( 15, pset.to_string() == "f11:(0,0) f14:(0,0)" );
    ensure( 16, pset.get<cdbl>("f14") == cdbl(0.0,0.0) );
  }

  {
    ParameterSet pset;
    pset.put<cldbl>( "f21", cldbl(1,2) );
    ensure( 21, pset.to_string() == "f21:(1,2)" );
    ensure( 22, pset.get<cldbl>("f21") == cldbl(1.0L,2.0L) );
    ensure( 23, pset.get<cdbl>("f21") == cdbl(1.0,2.0) );
    pset.put<string>( "f24", "(3.25 , 4.75 )" );
    ensure( 24, pset.get<cldbl>("f24") == cldbl(3.25L,4.75L) );
    ensure( 25, pset.get<cdbl>("f24") == cdbl(3.25,4.75) );
  }

  return 0;

}  // main()
