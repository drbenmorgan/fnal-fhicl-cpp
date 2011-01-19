// ======================================================================
//
// coding
//
// ======================================================================

#include "fhiclcpp/coding.h"

#include "cetlib/canonical_string.h"
#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/ParameterSetRegistry.h"
#include <cstddef>
#include <cstdlib>
#include <limits>

using namespace fhicl;
using namespace fhicl::detail;

using boost::any;
using boost::any_cast;
using boost::lexical_cast;
using boost::numeric_cast;

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
  static  std::string  literal_nil("nil");
  return literal_nil;
}

static inline  ps_atom_t
  literal_true( )
{
  static  std::string  literal_true("true");
  return literal_true;
}

static inline  ps_atom_t
  literal_false( )
{
  static  std::string  literal_false("false");
  return literal_false;
}

static inline  ps_atom_t
  literal_infinity( )
{
  static  std::string  literal_infinity("infinity");
  return literal_infinity;
}

// ----------------------------------------------------------------------

ps_atom_t  // string (with quotes)
  fhicl::detail::encode( std::string const & value )
{
  bool is_quoted =  value.size() >= 2 && value[0] == value.end()[-1]
                 && (value[0] == '\"' || value[0] == '\'');

  std::string const & str = is_quoted  ?  value
                                  :  '\'' + value + '\'';

  extended_value xval;
  if( ! parse_value(str, xval) || ! xval.is_a(STRING) )
    throw fhicl::exception(type_mismatch, "invalid input string: ") << value;

  return extended_value::atom_t(xval);
}

ps_atom_t  // nil
  fhicl::detail::encode( void * )
{
  return literal_nil();
}

ps_atom_t  // bool
  fhicl::detail::encode( bool value )
{
  return value ? literal_true() : literal_false();
}

ParameterSetID  // table
  fhicl::detail::encode( ParameterSet const & value )
{
  return ParameterSetRegistry::put(value);
}

ps_atom_t  // unsigned
  fhicl::detail::encode( std::uintmax_t value )
{
  std::string result = lexical_cast<std::string>(value);
  if( result.size() > 6 ) {
    std::size_t sz = result.size() - 1;
    result.insert(1, ".");
    result += "e+" + lexical_cast<std::string>(sz);
  }
  return result;
}

ps_atom_t  // signed
  fhicl::detail::encode( std::intmax_t value )
{
  std::string result = encode( std::uintmax_t(std::abs(value)) );
  if( value < 0 )
    result.insert(0, "-");
  return result;
}

ps_atom_t  // floating-point
  fhicl::detail::encode( ldbl value )
{
  if( value == std::numeric_limits<ldbl>::infinity() )
    return '+' + literal_infinity();
  if( value == - std::numeric_limits<ldbl>::infinity() )
    return '-' + literal_infinity();

  std::intmax_t chopped = static_cast<std::intmax_t>( value );
  if( static_cast<ldbl>(chopped) == value )
    return encode(chopped);

  return lexical_cast<std::string>(value);
}

// ----------------------------------------------------------------------

void  // string without delimiting quotes
  fhicl::detail::decode( any const & a, std::string & result )
{
  if( is_table(a) )
    throw fhicl::exception(type_mismatch, "can't obtain string from table");
  if( is_sequence(a) )
    throw fhicl::exception(type_mismatch, "can't obtain string from sequence");

  result = any_cast<std::string>(a);
  if( result.size() >= 2 && result[0] == '\"' && result.end()[-1] == '\"' )
    result = cet::unescape( result.substr(1, result.size()-2) );
}

void  // nil
  fhicl::detail::decode( any const & a, void * & result )
{
  std::string str;
  decode(a, str);

  extended_value xval;
  if( ! parse_value(str, xval) || ! xval.is_a(NIL) )
    throw fhicl::exception(type_mismatch, "invalid nil string: ") << str;

  result = 0;
}

void  // bool
  fhicl::detail::decode( any const & a, bool & result )
{
  std::string str;
  decode(a, str);

  extended_value xval;
  if( ! parse_value(str, xval) || ! xval.is_a(BOOL) )
    throw fhicl::exception(type_mismatch, "invalid bool string: ") << str;

  typedef  extended_value::atom_t  atom_t;
  atom_t const & atom = atom_t(xval);
  result = atom == literal_true();
}

void  // table
  fhicl::detail::decode( any const & a, ParameterSet & result )
{
  ParameterSetID id = any_cast<ParameterSetID>(a);
  result = ParameterSetRegistry::get(id);
}

void  // unsigned
  fhicl::detail::decode( any const & a, std::uintmax_t & result )
{
  std::string str;
  decode(a, str);

  extended_value xval;
  if( ! parse_value(str, xval) || ! xval.is_a(NUMBER) )
    throw fhicl::exception(type_mismatch, "invalid unsigned string: ") << str;

  typedef  extended_value::atom_t  atom_t;
  atom_t const & atom = atom_t(xval);
  ldbl via = lexical_cast<ldbl>(atom);
  result = numeric_cast<std::uintmax_t>(via);
}

void  // signed
  fhicl::detail::decode( any const & a, std::intmax_t & result )
{
  std::string str;
  decode(a, str);

  extended_value xval;
  if( ! parse_value(str, xval) || ! xval.is_a(NUMBER) )
    throw fhicl::exception(type_mismatch, "invalid signed string: ") << str;

  typedef  extended_value::atom_t  atom_t;
  atom_t const & atom = atom_t(xval);
  ldbl via = lexical_cast<ldbl>(atom);
  result = numeric_cast<std::intmax_t>(via);
}

void  // floating-point
  fhicl::detail::decode( any const & a, ldbl & result )
{
  std::string str;
  decode(a, str);

  extended_value xval;
  if( ! parse_value(str, xval) || ! xval.is_a(NUMBER) )
    throw fhicl::exception(type_mismatch, "invalid float string: ") << str;

  typedef  extended_value::atom_t  atom_t;
  atom_t const & atom = atom_t(xval);
  if( atom.substr(1) == literal_infinity() )  {
    switch( atom[0] ) {
    case '+': result = + std::numeric_limits<ldbl>::infinity(); return;
    case '-': result = - std::numeric_limits<ldbl>::infinity(); return;
    }
  }
  else
    result = lexical_cast<ldbl>(atom);
}

void  // complex
  fhicl::detail::decode( any const & a, std::complex<ldbl> & result )
{
  std::string str;
  decode(a, str);

  extended_value xval;
  if( ! parse_value(str, xval) || ! xval.is_a(COMPLEX) )
    throw fhicl::exception(type_mismatch, "invalid complex string: ") << str;

  typedef  extended_value::complex_t  complex_t;
  complex_t const & cmplx = complex_t(xval);
  ldbl real, imag;
  decode(cmplx.first , real);
  decode(cmplx.second, imag);
  result = std::complex<ldbl>(real,imag);
}

// ======================================================================
