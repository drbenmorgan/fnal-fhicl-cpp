#ifndef FHICLCPP_PARAMETERSET_H
#define FHICLCPP_PARAMETERSET_H

// ======================================================================
//
// ParameterSet
//
// ======================================================================


#include "ParameterSetID.h"             // fhicl/
#include "ParameterSetRegistry.h"       // fhicl/
#include "type_traits.h"                // fhicl/
#include "boost/any.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/numeric/conversion/cast.hpp"

#include <algorithm>
#include <iterator>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

namespace fhicl {
  class ParameterSet;
}

// ======================================================================


class fhicl::ParameterSet
{
  typedef  ParameterSet               ps_t;
  typedef  ParameterSetID             psid_t;
  typedef  ParameterSetRegistry       psreg_t;

  typedef  std::string                string_t;
  typedef  boost::any                 any_t;
  typedef  std::map<string_t, any_t>  entries_t;

  entries_t  psetmap_;
  psid_t     id_;

  friend class ParameterSetID;

  template< typename Iterator >
    friend class PSetParser;

  static  void  cpp_to_atom_( string_t & str );
  static  void  atom_to_cpp_( string_t & str );

  template< class >  struct encode_;
  template< class T, bool=tt::is_numeric<T>::value >  struct decode_;

public:
  // compiler generates default c'tor, d'tor, copy c'tor, copy assignment

  bool     empty() const  { return psetmap_.empty(); }
  string_t toString() const { return string_t(""); }
  psid_t   id();

  //----------------------------------------------------------------------
  // insert user's key-value pair

  void insert( string_t const & key, boost::any const & value)
  { psetmap_.insert( std::make_pair(key, value) ); }

  template< class T >
    void
    put( string_t const & key, T const & value )
  try
  {
    psetmap_.insert( std::make_pair(key,encode_<T>()(value)) );
    id_.invalidate();
  }
  catch( boost::bad_lexical_cast const & ) {
    throw std::domain_error("");
  }

  template< class T >
    void
    put( char const * key, T const & value )  // literal key -> string key
  { put( string_t(key), value ); }

  //----------------------------------------------------------------------
  // obtain value corresponding to user's key

  template< class T >
    T
    get( std::string const & key ) const
  {
    entries_t::const_iterator it = psetmap_.find(key);
    if( it == psetmap_.end() )  throw std::domain_error("");
    else                        return decode_<T>()(it->second);
  }

  template< class T >
    T
    get( std::string const & key, T const default_value ) const
  {
    try { return get<T>(key); }
    catch( std::domain_error const & ) { return default_value; }
    catch( std::range_error  const & ) { return default_value; }
  }

  template< class T >
    T
    get( char const * key ) const  // literal -> string
  { return get<T>( std::string(key) ); }

  template< class T >
    T
    get( char const * key, T const & default_value ) const // literal -> string
  { return get<T>( std::string(key), default_value ); }

};  // ParameterSet

// ======================================================================

namespace fhicl {  // most atoms

  template< class Atom >
    struct ParameterSet::encode_
  {
    boost::any
      operator () ( Atom const & value ) const
    {
      std::string str = boost::lexical_cast<std::string>(value);
      ParameterSet::cpp_to_atom_(str);
      return str;
    }
  };  // encode_<>

  template< class Atom >
    struct ParameterSet::decode_<Atom,true>  // numeric
  {
    Atom
      operator () ( boost::any const & any_val ) const
    try
    {
      typedef  typename tt::largest_type<Atom>::type  via_t;
      std::string str = boost::any_cast<std::string>(any_val);
      atom_to_cpp_(str);
      via_t v = boost::lexical_cast<via_t>(str);
      return boost::numeric_cast<Atom>(v);
    }
    catch( boost::bad_any_cast     const & ) { throw std::range_error(""); }
    catch( boost::bad_lexical_cast const & ) { throw std::range_error(""); }
    catch( boost::bad_numeric_cast const & ) { throw std::range_error(""); }
  };  // decode_<>

  template< class Atom >
    struct ParameterSet::decode_<Atom,false>  // nonnumeric
  {
    Atom
      operator () ( boost::any const & any_val ) const
    try
    {
      return boost::any_cast<Atom>(any_val);
    }
    catch( boost::bad_any_cast const & ) { throw std::range_error(""); }
  };  // decode_<>

}  // namespace fhicl

// ======================================================================

namespace fhicl {  // nil

  template<>
    struct ParameterSet::encode_<void*>
  {
    boost::any
      operator () ( void * value ) const
    {
      std::string str("nil");
      ParameterSet::cpp_to_atom_(str);
      return str;
    }
  };

  template<>
    struct ParameterSet::decode_<void*,false>
  {
    void *
      operator () ( boost::any const & any_val ) const
    {
      return 0;
    }
  };  // decode_<>

}  // namespace fhicl

// ======================================================================

namespace fhicl {  // bool

  template<>
    struct ParameterSet::encode_<bool>
  {
    boost::any
      operator () ( bool value ) const
    {
      std::string str = boost::lexical_cast<std::string>(value);
      ParameterSet::cpp_to_atom_(str);
      return str;
    }
  };

  template<>
    struct ParameterSet::decode_<bool,false>
  {
    bool
      operator () ( boost::any const & any_val ) const
    {
      std::string str = boost::any_cast<std::string>(any_val);
      atom_to_cpp_(str);
      return boost::lexical_cast<bool>(str);
    }
  };  // decode_<>

}  // namespace fhicl

// ======================================================================

namespace fhicl {  // ParameterSet

  template<>
    struct ParameterSet::encode_<ParameterSet>
  {
    boost::any
      operator () ( ParameterSet const & value ) const
    { return ParameterSetRegistry::put(value); }
  };

  template<>
    struct ParameterSet::decode_<ParameterSet,false>
  {
    ParameterSet
      operator () ( boost::any const & any_val ) const
    {
      ParameterSetID id = boost::any_cast<ParameterSetID>(any_val);
      return ParameterSetRegistry::get(id);
    }
  };  // decode_<>

}  // namespace fhicl

// ======================================================================

namespace fhicl {  // sequence

  template< class T >
    struct ParameterSet::encode_< std::vector<T> >
  {
    boost::any
      operator () ( std::vector<T> const & value ) const
    {
      std::vector<boost::any>  result;
      std::transform( value.begin(), value.end()
                    , std::back_inserter(result)
                    , encode_<T>()
                    );
      return result;
    }
  };

  template< class T >
    struct ParameterSet::decode_<std::vector<T>,false>
  {
    std::vector<T>
      operator () ( boost::any const & any_val ) const
    {
      typedef  std::vector<boost::any>  vec_t;
      vec_t va = boost::any_cast<vec_t>(any_val);
      std::vector<T>  result;
      std::transform( va.begin(), va.end()
                    , std::back_inserter(result)
                    , decode_<T>()
                    );
      return result;
    }
  };  // decode_<>

}  // namespace fhicl

// ======================================================================

namespace fhicl {  // complex

  template< class T >
    struct ParameterSet::encode_< std::complex<T> >
  {
    boost::any
      operator () ( std::complex<T> const & value ) const
    {
      std::vector<T>  pair;
      pair.push_back(value.real()), pair.push_back(value.imag());
      return encode_<std::vector<T> >()(pair);
    }
  };

  template< class T >
    struct ParameterSet::decode_<std::complex<T>,false>
  {
    std::complex<T>
      operator () ( boost::any const & any_val ) const
    {
      std::vector<T>  result = decode_<std::vector<T> >()(any_val);
      if( result.size() != 2 )
        throw std::domain_error("");
      return std::complex<T>(result[0], result[1]);
    }
  };  // decode_<>

}  // namespace fhicl

// ======================================================================


#endif  // FHICLCPP_PARAMETERSET_H
