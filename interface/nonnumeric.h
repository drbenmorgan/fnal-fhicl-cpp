#include "fhiclcpp/type_traits.h"


template< class T >
  typename tt::disable_when<tt::is_numeric, T>::type
  get( std::string const & key ) const
{
  valuemap::const_iterator it = PSetMap.find(key);
  if( it == PSetMap.end() )
    throw NO_SUCH_ENTRY_EXCEPTION;
  try {
    return boost::any_cast<T>(it->second);
  }
  catch( boost::bad_any_cast const & ) {
    throw BAD_VALUE_EXCEPTION;
  }
}


template< class T >
  typename tt::disable_when<tt::is_numeric, T>::type
  get( std::string const & key, T const & default_value ) const
{
  valuemap::const_iterator it = PSetMap.find(key);
  if( it == PSetMap.end() )
    return default_value;
  try {
    return boost::any_cast<T>(it->second);
  }
  catch( boost::bad_any_cast const & ) {
    return default_value;
  }
}
