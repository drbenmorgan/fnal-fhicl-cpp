#include <algorithm>
#include <iterator>
#include <string>
#include <vector>


// ----------------------------------------------------------------------
// obtain a sequence of values

template< class T >
  std::vector<T>
  get<std::vector<T> >( std::string const & key ) const
{
  valuemap::const_iterator it = PSetMap.find(key);
  if( it == PSetMap.end() )
    throw NO_SUCH_ENTRY_EXCEPTION;
  try {
    typedef  std::vector<boost::any>  vec_any_t;
    vec_any_t va = boost::any_cast<vec_any_t> >(it->second);
    std::vector<T>  result;
    std::transform( va.begin(), va.end()
                  , std::back_inserter(result)
                  , value_cast<T>
                  );
    return result;
  }
  catch( ... ) {
    throw BAD_VALUE_EXCEPTION;
  }
}

template< class T >
  std::vector<T>
  get( std::string    const & key
     , std::vector<T> const & default_value ) const
{
  try {
    return get<std::vector<T> >(key);
  }
  catch( NO_SUCH_ENTRY_EXCEPTION const & ) { return default_value; }
  catch( BAD_VALUE_EXCEPTION     const & ) { return default_value; }
}
