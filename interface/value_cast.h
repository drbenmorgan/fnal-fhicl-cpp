

// ----------------------------------------------------------------------
// adjust string for any special representations

inline
void
  fhicl_to_cpp( std::string & str )
{
  // TODO: if str holds a FHiCL string for infinity,
  // translate it into the C++ equivalent string
  //
  // TODO: do similarly for any other values needing special handling
}


// ----------------------------------------------------------------------
// specify largest available corresponding type

template< class T
        , bool is_int     = tt::is_int<T>
        , bool is_uint    = tt::is_uint<T>
        , bool is_float   = tt::is_float<T>
        >
  struct largest_type
{ };

template< class T > struct largest_type<T,true,false,false>
{ typedef  long long  type; };

template< class T > struct largest_type<T,false,true,false>
{ typedef  long long  type; };

template< class T > struct largest_type<T,false,false,true>
{ typedef  long double  type; };


// ----------------------------------------------------------------------
// obtain a numeric value

template< class result_type >
  typename tt::enable_when<tt::is_numeric, result_type>::type
  value_cast( boost::any const & any_val )
try
{
  // TODO: handle complex types, here and/or elsewhere
  typedef  typename largest_type<result_type>::type  via_t;
  std::string str = boost::any_cast<std::string>(any_val);
  fhicl_to_cpp(str);
  via_t v = boost::lexical_cast<via_t>(str);
  return boost::numeric_cast<result_type>(v);
}
catch( boost::bad_any_cast     const & ) { throw BAD_VALUE_EXCEPTION; }
catch( boost::bad_lexical_cast const & ) { throw BAD_VALUE_EXCEPTION; }
catch( boost::bad_numeric_cast const & ) { throw BAD_VALUE_EXCEPTION; }


// ----------------------------------------------------------------------
// obtain a nonnumeric value

template< class result_type >
  typename tt::disable_when<tt::is_numeric, result_type>::type
  value_cast( boost::any const & any_val )
try
{
  // TODO: handle complex types, here and/or elsewhere
  return = boost::any_cast<result_type>(any_val);
}
catch( boost::bad_any_cast     const & ) { throw BAD_VALUE_EXCEPTION; }
