// ======================================================================
//
// ParameterSet
//
// ======================================================================


#include "fhiclcpp/ParameterSet.h"

#include "fhiclcpp/ParameterSetRegistry.h"

using namespace boost;
using namespace fhicl;
using namespace std;

// ======================================================================

static inline  bool
  is_atom( any const & val )
{ return val.type() == typeid(string); }

static inline  bool
  is_parameterset( any const & val )
{ return val.type() == typeid(ParameterSetID); }

static inline  bool
  is_vector( any const & val )
{ return val.type() == typeid(vector<any>); }

static  string
  do_hash( any const & val )
{
  if( is_atom(val) )
    return '"' + any_cast<string>(val) + '"';

  else if( is_parameterset(val) )
    return '{' + any_cast<ParameterSetID>(val).to_string() + '}';

  else if( is_vector(val) ) {
    vector<any> v = any_cast< vector<any> >(val);
    if( v.empty() )
      return "[]";
    string result = do_hash(v[0]);
    for( int k = 1; k != v.size(); ++k )
      result.append( "," )
            .append( do_hash(v[k]) );
    return '[' + result + ']';
  }

  else
    throw fhicl::exception(cant_happen, "fhicl::do_hash failure");
}

// ----------------------------------------------------------------------

bool
  ParameterSet::is_empty( ) const
{ return mapping_.empty(); }

ParameterSetID
  ParameterSet::id( ) const
{
  if( ! id_.is_valid() )
    id_.reset(*this);
  return id_;
}

string
  ParameterSet::to_string( ) const
{
  return hash_string(); //TODO: implement pretty print
}

string
  ParameterSet::hash_string( ) const
{
  string result;

  for( map_iter_t it = mapping_.begin(); it != mapping_.end(); ++it )
    result.append( it->first )
          .append( ":" )
          .append( do_hash(it->second) )
          ;

  return result;
}

vector<string>
  ParameterSet::get_keys( ) const
{
  vector<string> keys;  keys.reserve( mapping_.size() );
  for( map_iter_t it = mapping_.begin()
                , e  = mapping_.end(); it != e; ++it )
    keys.push_back( it->first );
  return keys;
}

vector<string>
  ParameterSet::get_pset_keys( ) const
{
  vector<string> keys;
  for( map_iter_t it = mapping_.begin()
                , e  = mapping_.end(); it != e; ++it )
    if( is_parameterset(it->second) )
      keys.push_back( it->first );
  return keys;
}

// ----------------------------------------------------------------------

void
  ParameterSet::cpp_to_atom_( string & str )
{
  // TODO: transform C++ string to equivalent FHiCL atom
}

void
  ParameterSet::atom_to_cpp_( string & str )
{
  // TODO: if str holds a FHiCL string for infinity,
  // translate it into the C++ equivalent string;
  // do similarly for any other values needing special handling
}

// ----------------------------------------------------------------------

void
  ParameterSet::insert( string const & key, any const & value)
{
  mapping_.insert( make_pair(key, value) );
  id_.invalidate();
}

// ----------------------------------------------------------------------

any
  ParameterSet::encode_( void * value ) const
{
  string str("nil");
  cpp_to_atom_(str);
  return str;
}

any
  ParameterSet::encode_( bool value ) const
{
  string str = lexical_cast<string>(value);
  cpp_to_atom_(str);
  return str;
}

any
  ParameterSet::encode_( ParameterSet const & value ) const
{ return ParameterSetRegistry::put(value); }

// ----------------------------------------------------------------------

void
  ParameterSet::decode_( any const & any_val
                       , void *    & result
                       ) const
{
  result = 0;
}

void
  ParameterSet::decode_( any const & any_val
                       , bool      & result
                       ) const
{
  string str = any_cast<string>(any_val);
  atom_to_cpp_(str);
  result = lexical_cast<bool>(str);
}

void
  ParameterSet::decode_( any const    & any_val
                       , ParameterSet & result
                       ) const
{
  ParameterSetID id = any_cast<ParameterSetID>(any_val);
  result = ParameterSetRegistry::get(id);
}

// ======================================================================
