#ifndef CETLIB_REGISTRY_H
#define CETLIB_REGISTRY_H

// ======================================================================
//
// registry<> - a singleton map
//
// ======================================================================


#include <map>
#include <stdexcept>

namespace cet {
  template< class K, class V >  class registry;
}

// ======================================================================


template< class K, class V >
  class cet::registry
{
  typedef  std::map<K const, V>            reg_t;
  typedef  typename reg_t::const_iterator  iter_t;

public:
  static
  bool
    empty( )
  {return the_registry_().empty(); }

  static
  typename reg_t::size_type
    size( )
  {return the_registry_().size(); }

  static
  void
    put( K const & key, V const & value )
  { the_registry_().insert( std::make_pair(key, value) ); }

  static
  V const &
    get( K const & key )
  {
    iter_t it = the_registry_().find(key);
    if( it == the_registry_().end() ) throw std::out_of_range("");
    else                              return it->second;
  }

  static
  bool
    get( K const & key, V & value ) throw()
  {
    iter_t it = the_registry_().find(key);
    return it == the_registry_().end() ?  (                   false)
                                       :  (value = it->second, true);
  }

private:
  // non-instantiable and non-copyable:
  registry() { }
  ~registry() { }
  registry( registry const & );
  void  operator = ( registry const & );

  // encapsulated singleton:
  static  reg_t &  the_registry_() {
    static  reg_t  the_registry;
    return the_registry;
  }

};  // registry<>

// ======================================================================


#endif  // CETLIB_REGISTRY_H
