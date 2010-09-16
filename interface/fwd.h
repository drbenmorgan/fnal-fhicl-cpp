template< class T >
  T
  get( char const * key ) const
{ return get<T>( std::string(key) ); }


template< class T >
  T
  get( char const * key, T const & default_value ) const
{ return get<T>( std::string(key), default_value ); }
