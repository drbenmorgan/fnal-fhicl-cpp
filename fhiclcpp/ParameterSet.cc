// ======================================================================
//
// ParameterSet
//
// ======================================================================


#include "fhiclcpp/ParameterSet.h"

#include "cetlib/canonical_number.h"
#include "cetlib/canonical_string.h"
#include "cpp0x/utility"
#include "fhiclcpp/ParameterSetRegistry.h"
#include "fhiclcpp/parse.h"
#include <cctype>
#include <cstddef>
#include <cstdlib>
#include <limits>

using namespace fhicl;
using namespace std;

using boost::any;
using boost::any_cast;
using boost::lexical_cast;

typedef  ParameterSet::ps_atom_t      ps_atom_t;
typedef  ParameterSet::ps_sequence_t  ps_sequence_t;
//typedef  ParameterSet::ps_table_t     ps_table_t;

typedef  long double  ldbl;

// ======================================================================

static inline  bool
  is_sequence( any const & val )
{ return val.type() == typeid(ps_sequence_t); }

static inline  bool
  is_table( any const & val )
{ return val.type() == typeid(ParameterSetID); }

// ----------------------------------------------------------------------

static inline  ps_atom_t
  literal_nil( )
{
  static  string  literal_nil("nil");
  return literal_nil;
}

static inline  ps_atom_t
  literal_true( )
{
  static  string  literal_true("true");
  return literal_true;
}

static inline  ps_atom_t
  literal_false( )
{
  static  string  literal_false("false");
  return literal_false;
}

static inline  ps_atom_t
  literal_infinity( )
{
  static  string  literal_infinity("infinity");
  return literal_infinity;
}

// ----------------------------------------------------------------------

string
  ParameterSet::stringify( any const & a ) const
{
  if( is_table(a) ) {
    ParameterSetID const & psid = any_cast<ParameterSetID>(a);
    return '{' + ParameterSetRegistry::get(psid).to_string() + '}';
  }

  else if( is_sequence(a) ) {
    ps_sequence_t const & seq = boost::any_cast<ps_sequence_t>(a);
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

// ----------------------------------------------------------------------

ps_atom_t  // string (with quotes)
  ParameterSet::encode( string const & value ) const
{
  bool is_quoted =  value.size() >= 2 && value[0] == value.end()[-1]
                 && (value[0] == '\"' || value[0] == '\'');

  string const & str = is_quoted  ?  value
                                  :  '\'' + value + '\'';

  extended_value xval;
  if( ! parse_value(str, xval) || ! xval.is_a(STRING) )
    throw fhicl::exception(type_mismatch, "invalid input string: ") << value;

  return extended_value::atom_t(xval);
}

ps_atom_t  // nil
  ParameterSet::encode( void * ) const
{
  return literal_nil();
}

ps_atom_t  // bool
  ParameterSet::encode( bool value ) const
{
  return value ? literal_true() : literal_false();
}

ParameterSetID  // table
  ParameterSet::encode( ParameterSet const & value ) const
{
  return ParameterSetRegistry::put(value);
}

ps_atom_t  // unsigned
  ParameterSet::encode( uintmax_t value ) const
{
  string result = lexical_cast<string>(value);
  if( result.size() > 6 ) {
    size_t sz = result.size() - 1;
    result.insert(1, ".");
    result += "e+" + lexical_cast<string>(sz);
  }
  return result;
}

ps_atom_t  // signed
  ParameterSet::encode( intmax_t value ) const
{
  string result = encode( uintmax_t(abs(value)) );
  if( value < 0 )
    result.insert(0, "-");
  return result;
}

ps_atom_t  // floating-point
  ParameterSet::encode( ldbl value ) const
{
  if( value == numeric_limits<ldbl>::infinity() )
    return '+' + literal_infinity();
  if( value == - numeric_limits<ldbl>::infinity() )
    return '-' + literal_infinity();

  intmax_t chopped = static_cast<intmax_t>( value );
  if( static_cast<ldbl>(chopped) == value )
    return encode(chopped);

  return lexical_cast<string>(value);
}

// ----------------------------------------------------------------------

void  // string without delimiting quotes
  ParameterSet::decode( boost::any const & a, string & result ) const
{
  if( is_table(a) )
    throw fhicl::exception(type_mismatch, "can't obtain string from table");
  if( is_sequence(a) )
    throw fhicl::exception(type_mismatch, "can't obtain string from sequence");

  result = any_cast<string>(a);
  if( result.size() >= 2 && result[0] == '\"' && result.end()[-1] == '\"' )
    result = cet::unescape( result.substr(1, result.size()-2) );
}

void  // nil
  ParameterSet::decode( boost::any const & a, void * & result ) const
{
  string str;
  decode(a, str);

  extended_value xval;
  if( ! parse_value(str, xval) || ! xval.is_a(NIL) )
    throw fhicl::exception(type_mismatch, "invalid nil string: ") << str;

  result = 0;
}

void  // bool
  ParameterSet::decode( boost::any const & a, bool & result ) const
{
  string str;
  decode(a, str);

  extended_value xval;
  if( ! parse_value(str, xval) || ! xval.is_a(BOOL) )
    throw fhicl::exception(type_mismatch, "invalid bool string: ") << str;

  typedef  extended_value::atom_t  atom_t;
  atom_t const & atom = atom_t(xval);
  result = atom == literal_true();
}

void  // table
  ParameterSet::decode( boost::any const & a, ParameterSet & result ) const
{
  ParameterSetID id = any_cast<ParameterSetID>(a);
  result = ParameterSetRegistry::get(id);
}

void  // unsigned
  ParameterSet::decode( boost::any const & a, uintmax_t & result ) const
{
  string str;
  decode(a, str);

  extended_value xval;
  if( ! parse_value(str, xval) || ! xval.is_a(NUMBER) )
    throw fhicl::exception(type_mismatch, "invalid unsigned string: ") << str;

  typedef  extended_value::atom_t  atom_t;
  atom_t const & atom = atom_t(xval);
  ldbl via = boost::lexical_cast<ldbl>(atom);
  result = boost::numeric_cast<uintmax_t>(via);
}

void  // signed
  ParameterSet::decode( boost::any const & a, intmax_t & result ) const
{
  string str;
  decode(a, str);

  extended_value xval;
  if( ! parse_value(str, xval) || ! xval.is_a(NUMBER) )
    throw fhicl::exception(type_mismatch, "invalid signed string: ") << str;

  typedef  extended_value::atom_t  atom_t;
  atom_t const & atom = atom_t(xval);
  ldbl via = boost::lexical_cast<ldbl>(atom);
  result = boost::numeric_cast<intmax_t>(via);
}

void  // floating-point
  ParameterSet::decode( boost::any const & a, ldbl & result ) const
{
  string str;
  decode(a, str);

  extended_value xval;
  if( ! parse_value(str, xval) || ! xval.is_a(NUMBER) )
    throw fhicl::exception(type_mismatch, "invalid float string: ") << str;

  typedef  extended_value::atom_t  atom_t;
  atom_t const & atom = atom_t(xval);
  if( atom.substr(1) == literal_infinity() )  {
    switch( atom[0] ) {
    case '+': result = + numeric_limits<ldbl>::infinity(); return;
    case '-': result = - numeric_limits<ldbl>::infinity(); return;
    }
  }
  else
    result = boost::lexical_cast<ldbl>(atom);
}

void  // complex
  ParameterSet::decode( boost::any const & a, complex<ldbl> & result ) const
{
  string str;
  decode(a, str);

  extended_value xval;
  if( ! parse_value(str, xval) || ! xval.is_a(COMPLEX) )
    throw fhicl::exception(type_mismatch, "invalid complex string: ") << str;

  typedef  extended_value::complex_t  complex_t;
  complex_t const & cmplx = complex_t(xval);
  ldbl real, imag;
  decode(cmplx.first , real);
  decode(cmplx.second, imag);
  result = complex<ldbl>(real,imag);
}

// ======================================================================
