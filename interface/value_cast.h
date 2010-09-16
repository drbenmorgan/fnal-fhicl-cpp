inline
void
  adjust( std::string str )
{
  // TODO: if str holds a FHiCL string for infinity,
  // translate it into the C++ equivalent string
  //
  // TODO: do similarly for any other values needing special handling
}


template< class T
        , bool is_int     = tt::is_int<T>
        , bool is_uint    = tt::is_uint<T>
        , bool is_float   = tt::is_float<T>
        >
  struct via_type
{ };

template< class T > struct via_type<T,true,false,false>
{ typedef  long long  type; };

template< class T > struct via_type<T,false,true,false>
{ typedef  long long  type; };

template< class T > struct via_type<T,false,false,true>
{ typedef  long double  type; };


template< class result_type >
  result_type
  value_cast( valuemap::const_iterator it )
try
{
  // TODO: handle complex types, here and/or elsewhere
  typedef  typename via_type<result_type>::type  via_t;
  std::string str = boost::any_cast<std::string>(it->second);
  adjust(str);
  via_t v = boost::lexical_cast<via_t>(str);
  return boost::numeric_cast<result_type>(v);
}
catch( boost::bad_any_cast     const & ) { throw BAD_VALUE_EXCEPTION; }
catch( boost::bad_lexical_cast const & ) { throw BAD_VALUE_EXCEPTION; }
catch( boost::bad_numeric_cast const & ) { throw BAD_VALUE_EXCEPTION; }
