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
typedef  std::map<string_t, any_t>    entries_t;


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

string_t ps_t::toString() const
{
  return hashString(); //TODO: implement pretty print
}

namespace {

  bool isAtom_(boost::any const & val) 
  { 
    return val.type() == typeid(std::string);
  }

  bool isParameterSet_(boost::any const & val)
  {
    return val.type() == typeid(fhicl::ParameterSetID);
  }

  bool isVector_(boost::any const & val)
  {
    return val.type() == typeid(std::vector<boost::any>);
  }

  void hashStringEntry( string_t & dest
                      , boost::any const & val
                      , string_t const & mark)
  {
    if (isAtom_(val)) {
      dest += "\"";
      dest += boost::any_cast<string_t>(val);
      dest += "\"";
      dest += mark;
      return;
    }

    if (isParameterSet_(val)) {
      dest += boost::any_cast<fhicl::ParameterSetID>(val).to_string();
      dest += mark;
      return;
    }

    if (isVector_(val)) {
      dest += "[";

      std::vector<boost::any> v
          = boost::any_cast<std::vector<boost::any> >(val);

      if (v.empty())
        return;

      for(int i=0; i<v.size()-1; ++i)
        hashStringEntry(dest, v[i], ",");

      hashStringEntry(dest, v[v.size()-1], "");

      dest += "]";
      dest += mark;

      return;
    }
  }

} // end of annonymous namespace

string_t ps_t::hashString() const
{
  string_t hash("{");

  entries_t::const_iterator it = psetmap_.begin();
  while(it!=psetmap_.end()) {
    hash += it->first;
    hash += ":";
    hashStringEntry(hash, it->second, "");
    ++it;
  }

  hash += "}";

  return hash;
}

