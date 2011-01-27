// ======================================================================
//
// fhicl-expand: read/write specified text, replacing each #include
//               directive with the corresponding file's contents
//
// ======================================================================

#include "cetlib/exception.h"
#include "cetlib/include.h"
#include <fstream>
#include <iostream>
#include <string>

// ----------------------------------------------------------------------

static  std::string const &
  fhicl_env_var( )
{
  static  std::string const  fhicl_env_var("FHICL_FILE_PATH");
  return fhicl_env_var;
}

// ----------------------------------------------------------------------

bool
  is_single_dash( char const * s ) { return s[0] == '-' && s[1] == '\0'; }

// ----------------------------------------------------------------------

int
  do_including( std::istream & from, std::ostream & to )
try
{
  std::string result;
  cet::include(from, fhicl_env_var(), result);
  to << result;
  return 0;
}
catch( cet::exception const & e )
{
  return 1;
}  // do_including()

// ----------------------------------------------------------------------

int
  main( int argc, char* argv[] )
{
  int nfailures = 0;

  if( argc == 1 )
    nfailures += do_including(std::cin, std::cout);
  else {
    for( int k = 1; k != argc; ++k ) {
      if( is_single_dash(argv[k]) )
        nfailures += do_including(std::cin, std::cout);
      else {
        std::ifstream from(argv[k], std::ios_base::in);
        nfailures += do_including(from, std::cout);
      }
    }
  }

  return nfailures;

}  // main()
