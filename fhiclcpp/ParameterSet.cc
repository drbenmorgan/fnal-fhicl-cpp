// ======================================================================
//
// ParameterSet
//
// ======================================================================


#include "fhiclcpp/ParameterSet.h"

#include "fhiclcpp/ParameterSetRegistry.h"

using namespace fhicl;
using namespace fhicl::detail;
using namespace std;

using boost::any;
using boost::any_cast;

typedef  ParameterSet::ps_atom_t      ps_atom_t;
typedef  ParameterSet::ps_sequence_t  ps_sequence_t;

typedef  long double  ldbl;

// ======================================================================

string
  ParameterSet::stringify( any const & a ) const
{
  if( is_table(a) ) {
    ParameterSetID const & psid = any_cast<ParameterSetID>(a);
    return '{' + ParameterSetRegistry::get(psid).to_string() + '}';
  }

  else if( is_sequence(a) ) {
    ps_sequence_t const & seq = any_cast<ps_sequence_t>(a);
    string str;
    if( ! seq.empty() ) {
      str = stringify(*seq.begin());
      for( ps_sequence_t::const_iterator it = seq.begin()
                                       , e  = seq.end(); ++it != e;  ) {
        str.append(1, ',')
           .append(stringify(*it));
      }
    }
    return '[' + str + ']';
  }

  else  // is_atom(a)
    return any_cast<ps_atom_t>(a);

}  // stringify()

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
  string result;
  if( mapping_.empty() )
    return result;

  map_iter_t it = mapping_.begin();
  result.append( it->first )
        .append( 1, ':' )
        .append( stringify(it->second) )
        ;
  for( map_iter_t const e = mapping_.end(); ++it != e; )
    result.append( 1, ' ' )
          .append( it->first )
          .append( 1, ':' )
          .append( stringify(it->second) )
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
    if( is_table(it->second) )
      keys.push_back( it->first );
  return keys;
}

// ----------------------------------------------------------------------

void
  ParameterSet::insert( string const & key, any const & value)
{
  mapping_.insert( pair<string const,any>(key, value) );
  id_.invalidate();
}

// ======================================================================
