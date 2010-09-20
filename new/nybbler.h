#ifndef CETLIB_NYBBLER_H
#define CETLIB_NYBBLER_H

// ======================================================================
//
// nybble-processing
//
// ======================================================================

#include <string>


namespace cet {
  class nybbler;
}


// ======================================================================

class cet::nybbler
{
  typedef  unsigned char  uchar;
  typedef  std::string    string;

public:
  // use compiler's d'tor, copy c'tor, copy assignment

    nybbler( string const & s )
  : s_(s)
  { }

  string  as_hex ( ) const;
  string  as_char( ) const;

  nybbler &  operator << ( string const & );

private:
  string  s_;

  static  uchar  msn( char ch );  // most significant nybble
  static  uchar  lsn( char ch );  // least significant nybble

  static  char   to_hex( uchar nyb );
  static  uchar  to_nyb( char  hex );

};  // nybbler


// ======================================================================

#endif  // CETLIB_NYBBLER_H
