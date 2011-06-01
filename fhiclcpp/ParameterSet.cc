// ======================================================================
//
// ParameterSet
//
// ======================================================================


#include "fhiclcpp/ParameterSet.h"

#include "fhiclcpp/ParameterSetRegistry.h"
#include <cassert>
#include <cstddef>

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

class ParameterSet::Prettifier
{
public:
  Prettifier( map_t const & mapping )
    : mapping_( mapping )
    , result_ ( )
    , start_  ( )
  { }

  string
    operator () ( )
  {
    for( map_iter_t it = mapping_.begin()
                  , e  = mapping_.end();  it != e;  ++it ) {
      assert( start_.empty() );
      assert( next_col() == 1u );
      result_.append( it->first )
             .append( ": " )
             ;
      stringify( it->second );
      result_.append( "\n" );
    }

    assert( start_.empty() );
    return result_;
  }

private:
  map_t const &  mapping_;
  string         result_;
  vector<size_t> start_;

  size_t  next_col( ) const
  {
    size_t  ans     = result_.size() + 1u;
    size_t  last_nl = result_.find_last_of('\n') + 1u;
    return ans - last_nl;
  }

  void
    goto_col( size_t dest )
  {
    size_t now_at = next_col();
    if( now_at > dest ) {
      result_.append( "\n" );
      now_at = 1;
    }
    result_.append( dest-now_at, ' ' );
  }

  void
    stringify( any const & a )
  {
    size_t col = next_col();

    if( is_table(a) ) {
      ParameterSetID const & psid    = any_cast<ParameterSetID>(a);
      ParameterSet   const & ps      = ParameterSetRegistry::get(psid);
      map_t          const & mapping = ps.mapping_;

      result_.append( "{" );
      if( ! mapping.empty() ) {
        // Emit 1st pair:
        map_iter_t it = mapping.begin();
        result_.append( " " )
               .append( it->first )
               .append( ": " )
               ;
        stringify( it->second );
        // Emit remaining pairs:
        for( map_iter_t const e  = mapping.end();  ++it != e;  ) {
          goto_col( col+2 );
          result_.append( it->first )
                 .append( ": " )
                 ;
          stringify( it->second );
        }
        goto_col( col );
      }
      result_.append( "}" );
    }

    else if( is_sequence(a) ) {
      ps_sequence_t const & seq = any_cast<ps_sequence_t>(a);

      result_.append( "[" );
      if( ! seq.empty() ) {
        result_.append( " " );
        stringify(*seq.begin());
        for( ps_sequence_t::const_iterator it = seq.begin()
                                         , e  = seq.end(); ++it != e;  ) {
          goto_col( col );
          result_.append( ", " );
          stringify(*it);
        }
        if( seq.size() == 1u )  result_.append( " " );
        else                    goto_col( col );
      }
      result_.append( "]" );
    }

    else  // is_atom(a)
      result_.append( any_cast<ps_atom_t>(a) );

  }  // stringify()

};  // Prettifier

// ----------------------------------------------------------------------

string
  ParameterSet::to_indented_string( ) const
{
  Prettifier p(mapping_);
  string const & result = p();
  return result;
}

// ======================================================================
