// ======================================================================
//
// test string atoms
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
  ParameterSet pset;
  ensure( 1, pset.is_empty() );

  pset.put<string>( "n11", "" );
  ensure( 11, pset.get<string>("n11") == "" );
  ensure( 12, pset.to_string() == "n11:\"\"" );
  pset.put<string>( "n13", "a" );
  ensure( 13, pset.get<string>("n13") == "a" );
  ensure( 14, pset.to_string() == "n11:\"\" n13:\"a\"" );
  pset.put<string>( "n15", "\"a\"" );
  ensure( 15, pset.get<string>("n15") == "a" );
  ensure( 16, pset.to_string() == "n11:\"\" n13:\"a\" n15:\"a\"" );
  pset.put<string>( "n17", "'a b'" );
  ensure( 17, pset.get<string>("n17") == "a b" );
  ensure( 18, pset.to_string() == "n11:\"\" n13:\"a\" n15:\"a\" n17:\"a b\"" );

  return 0;

}  // main()
