// ======================================================================
//
// test atoms "true" and "false"
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
    pset.put<string>( "b11", "true" );
    ensure( 11, pset.to_string() == "b11:true" );
    ensure( 12, pset.get<bool>("b11") );
    pset.put<string>( "b13", "false" );
    ensure( 13, pset.to_string() == "b11:true b13:false" );
    ensure( 14, ! pset.get<bool>("b13") );
  }

  {
    ParameterSet pset;
    pset.put<bool>( "b21", true );
    ensure( 21, pset.get<string>("b21") == "true" );
    ensure( 22, pset.to_string() == "b21:true" );
    ensure( 23, pset.get<bool>("b21") );
    pset.put<bool>( "b24", false );
    ensure( 24, pset.get<string>("b24") == "false" );
    ensure( 25, pset.to_string() == "b21:true b24:false" );
    ensure( 26, ! pset.get<bool>("b24") );
  }

  {
    ParameterSet pset;
    pset.put<string>( "b31", "tru" );
    ensure( 31, pset.to_string() == "b31:tru" );
    try {
      pset.get<bool>("b31");
      ensure( 32, false );
    }
    catch( fhicl::exception const & e ) {
      ensure( 33, true );
    }
  }

  return 0;

}  // main()
