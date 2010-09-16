template< class T >
  typename tt::enable_when<tt::is_numeric, T>::type
  get( std::string const & key ) const
{
  valuemap::const_iterator it = PSetMap.find(key);
  if( it == PSetMap.end() )
    throw NO_SUCH_ENTRY_EXCEPTION;
  return value_cast<T>(it);
}


template< class T >
  typename tt::enable_when<tt::is_numeric, T>::type
  get( std::string const & key, T const default_value ) const
try
{
  valuemap::const_iterator it = PSetMap.find(key);
  return it == PSetMap.end() ?  default_value
                             :  value_cast<T>(it);
}
catch( ... ) { return default_value; }
