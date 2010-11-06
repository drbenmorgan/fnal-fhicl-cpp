// ======================================================================
//
// test atom "nil"
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
  ensure( 2, pset.to_string() == "" );

  pset.put<void*>( "n11", 0 );
  ensure( 11, pset.get<string>("n11") == "nil" );
  ensure( 12, pset.get<void*>("n11") == 0 );
  ensure( 13, pset.to_string() == "n11:nil" );

  pset.put<string>( "n21", "nil" );
  ensure( 21, pset.get<void*>("n21") == 0 );
  ensure( 22, pset.to_string() == "n11:nil n21:nil" );

  pset.put<string>( "n31", "NIL" );
  ensure( 31, pset.to_string() == "n11:nil n21:nil n31:NIL" );
  try {
    pset.get<void*>("n31");
    ensure( 32, false );
  }
  catch( fhicl::exception const & e ) {
    ensure( 33, true );
  }

  return 0;

}  // main()
