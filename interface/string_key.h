#include <string>


// ----------------------------------------------------------------------
// obtain value from given key

template< class T >
  T
  get( std::string const & key ) const
{
  valuemap::const_iterator it = PSetMap.find(key);
  if( it == PSetMap.end() )  throw NO_SUCH_ENTRY_EXCEPTION;
  else                       return value_cast<T>(it->second);
}


template< class T >
  T
  get( std::string const & key, T const default_value ) const
try { return get<T>(key); }
catch( NO_SUCH_ENTRY_EXCEPTION const & ) { return default_value; }
catch( BAD_VALUE_EXCEPTION     const & ) { return default_value; }
