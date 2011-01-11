// ======================================================================
//
// test sequences
//
// ======================================================================


#include "fhiclcpp/ParameterSet.h"
#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>

using fhicl::ParameterSet;
using std::string;
using std::vector;

typedef  vector<uint>  uvec;


void
  ensure( int which, bool claim )
{ if( not claim )
    std::exit(which);
}


int
  main( )
{
  ParameterSet pset;
  uvec         uv;

  pset.put<string>( "f11", "[]" );
  ensure( 11, pset.get<string>("f11") == "[]" );
  ensure( 12, pset.to_string() == "f11:\"[]\"" );
  ensure( 13, pset.get<uvec>("f11").empty() );
  ensure( 14, pset.get<uvec>("f11") == uvec() );
  ensure( 15, pset.get<uvec>("f11") == uv );
  pset.put<uvec>( "f16", uv );
  //  ensure( 16, pset.get<string>("f16", "NO") == "NO" );
  ensure( 17, pset.to_string() == "f11:\"[]\" f16:[]" );
  ensure( 18, pset.get<uvec>("f16") == uv );

  uv.push_back(1);
  pset.put<string>( "f21", "[1]" );
  ensure( 21, pset.get<string>("f21") == "[1]" );
  ensure( 22, pset.to_string() == "f11:\"[]\" f16:[] f21:\"[1]\"" );
  ensure( 23, pset.get<uvec>("f21").size() == 1 );
  ensure( 24, pset.get<uvec>("f21") == uv );
  pset.put<uvec>( "f25", uv );
  //  ensure( 25, pset.get<string>("f25", "NO") == "NO" );
  ensure( 26, pset.to_string() == "f11:\"[]\" f16:[] f21:\"[1]\" f25:[1]" );
  ensure( 27, pset.get<uvec>("f25").size() == 1 );
  ensure( 28, pset.get<uvec>("f25") == uv );

  uv.push_back(2);
  pset.put<string>( "f31", "[1,2]" );
  ensure( 31, pset.get<string>("f31") == "[1,2]" );
  ensure( 32, pset.get<uvec>("f31").size() == 2 );
  ensure( 33, pset.get<uvec>("f31") == uv );
  uv.push_back(3);
  pset.put<uvec>( "f34", uv );
  //  ensure( 34, pset.get<string>("f34", "NO") == "NO" );
  ensure( 35, pset.get<uvec>("f34").size() == 3 );
  ensure( 36, pset.get<uvec>("f34") == uv );

  uv.push_back(4);
  pset.put<string>( "f41", "[1,2,3,4]" );
  ensure( 41, pset.get<string>("f41") == "[1,2,3,4]" );
  ensure( 42, pset.get<uvec>("f41").size() == 4 );
  ensure( 43, pset.get<uvec>("f41") == uv );
  uv.push_back(5);
  pset.put<uvec>( "f43", uv );
  //  ensure( 43, pset.get<string>("f43", "NO") == "NO" );
  ensure( 44, pset.get<uvec>("f43").size() == 5 );
  ensure( 45, pset.get<uvec>("f43") == uv );

  return 0;

}  // main()
