// ======================================================================
//
// ParameterSet
//
// ======================================================================


#include "ParameterSet.h"


// ======================================================================


typedef  fhicl::ParameterSet          ps_t;
typedef  fhicl::ParameterSetID        psid_t;
typedef  fhicl::ParameterSetRegistry  psreg_t;

typedef  std::string                  string_t;
typedef  boost::any                   any_t;


// ======================================================================


void
  ps_t::cpp_to_atom_( string_t & str )
{
  // TODO: transform C++ string to equivalent FHiCL atom
}

void
  ps_t::atom_to_cpp_( std::string & str )
{
  // TODO: if str holds a FHiCL string for infinity,
  // translate it into the C++ equivalent string;
  // do similarly for any other values needing special handling
}


// ======================================================================


psid_t
  ps_t::id()
{
  if( ! id_.isValid() )
    id_.reset(*this);
  return id_;
}




// ======================================================================


