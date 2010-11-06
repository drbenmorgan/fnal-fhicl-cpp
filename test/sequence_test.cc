// ======================================================================
//
// test sequences
//
// ======================================================================


#include "fhiclcpp/ParameterSet.h"
#include <cstdlib>
#include <string>
#include <vector>

using fhicl::ParameterSet;
using std::string;
using std::vector;

typedef  vector<string>  svec;


void
  ensure( int which, bool claim )
{ if( not claim )
    std::exit(which);
}


int
  main( )
{
  ParameterSet pset;
  svec  sv;

  pset.put<string>( "f11", "[]" );
  ensure( 11, pset.get<string>("f11") == "[]" );
  ensure( 13, pset.to_string() == "f11:[]" );
  ensure( 14, pset.get<svec>("f11").empty() );
  ensure( 15, pset.get<svec>("f11") == svec() );
  ensure( 16, pset.get<svec>("f11") == sv );
  pset.put<svec>( "f17", sv );
  ensure( 17, pset.get<string>("f17") == "[]" );
  ensure( 18, pset.to_string() == "f11:[] f17:[]" );
  ensure( 19, pset.get<svec>("f17") == sv );

  sv.push_back("a");
  pset.put<string>( "f21", "[a]" );
  ensure( 21, pset.get<string>("f21") == "[a]" );
  ensure( 22, pset.to_string() == "f11:[] f17:[] f21:[a]" );
  ensure( 23, pset.get<svec>("f21").size() == 1 );
  ensure( 24, pset.get<svec>("f21") == sv );
  pset.put<svec>( "f25", sv );
  ensure( 25, pset.get<string>("f25") == "[a]" );
  ensure( 26, pset.to_string() == "f11:[] f17:[] f21:[a] f25:[a]" );
  ensure( 27, pset.get<svec>("f25").size() == 1 );
  ensure( 28, pset.get<svec>("f25") == sv );

  sv.push_back("b");
  pset.put<string>( "f31", "[a,b]" );
  ensure( 31, pset.get<string>("f31") == "[a,b]" );
  ensure( 32, pset.get<svec>("f31").size() == 2 );
  ensure( 33, pset.get<svec>("f31") == sv );
  sv.push_back("c");
  pset.put<svec>( "f34", sv );
  ensure( 34, pset.get<string>("f34") == "[a,b,c]" );
  ensure( 32, pset.get<svec>("f34").size() == 3 );
  ensure( 33, pset.get<svec>("f34") == sv );

  sv.push_back("d");
  pset.put<string>( "f41", "[a,b,c,d]" );
  ensure( 41, pset.get<string>("f41") == "[a,b,c,d]" );
  ensure( 42, pset.get<svec>("f41").size() == 4 );
  ensure( 43, pset.get<svec>("f41") == sv );
  sv.push_back("e");
  pset.put<svec>( "f43", sv );
  ensure( 43, pset.get<string>("f43") == "[a,b,c,d,e]" );
  ensure( 44, pset.get<svec>("f43").size() == 5 );
  ensure( 45, pset.get<svec>("f43") == sv );

  return 0;

}  // main()
