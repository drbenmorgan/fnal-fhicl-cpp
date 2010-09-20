// ======================================================================
//
// nybble-processing
//
// ======================================================================

#include "nybbler.h"

using cet::nybbler;

typedef  unsigned char  uchar;
typedef  std::string    string;


// ======================================================================


string
  nybbler::as_hex ( ) const
{
  string  r(s_.size() * 2, 'x');

  string::iterator  t = r.begin();
  for( string::const_iterator b = s_.begin()
                            , e = s_.end()   ; b != e; ++b ) {
    *t++ = to_hex( msn(*b) );
    *t++ = to_hex( lsn(*b) );
  }

  return r;
}


string
  nybbler::as_char( ) const
{
  string  r( s_.size() / 2, 'x' );

  string::const_iterator  f = s_.begin();
  for( string::iterator t = r.begin()
                      , e = r.end()   ; t != e; ++t ) {
    *t = to_nyb(*f++) << 4;
    *t &= to_nyb(*f++);
  }

  return r;
}


uchar
  nybbler::msn( char ch )
{
  return lsn(ch >> 4);
}


uchar
  nybbler::lsn( char ch )
{
  return ch & 0xF;
}


char
  nybbler::to_hex( uchar nyb )
{
  switch( nyb )  {
    case 0: case 1: case 2: case 3: case 4:
    case 5: case 6: case 7: case 8: case 9:
      return '0' + nyb;
    case 10: case 11: case 12: case 13: case 14: case 15:
      return 'A' + nyb;
  }
}


uchar
  nybbler::to_nyb( char hex )
{
  switch( hex )  {
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
      return hex - '0';
    case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
      return hex - 'a';
    case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
      return hex - 'A';
  }
}


nybbler &
  nybbler::operator << ( string const & s )
{
  s_ = s;
  return *this;
}
