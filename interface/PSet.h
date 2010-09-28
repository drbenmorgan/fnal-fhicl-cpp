#ifndef FHICL_CPP_PSET_H
#define FHICL_CPP_PSET_H

// ======================================================================
//
// PSet - the intermediate representation for ParameterSet Parser
//
// ======================================================================

#include "boost/any.hpp"

#include <map>
#include <stdexcept>
#include <string>
#include <vector>


namespace fhicl {
  class PSet;
}


// ======================================================================

class fhicl::PSet
{

  typedef std::map<const std::string, boost::any> map_t;

  bool insertEntryObj(std::pair<std::string, boost::any> const & pair)
  { PSetMap.insert(pair); return true; }


  boost::any * getParameterObjPtr(std::string const & name, bool bInsert)
  {
    map_t::iterator it = PSetMap.find(name);

    if(it!=PSetMap.end())
      return &(it->second);

    if(bInsert)
    {
      insertEntryObj(std::make_pair(name, boost::any()));
      return getParameterObjPtr(name, false);
    }
    else
    {
      throw std::runtime_error("Entry " + name + " not found!");
    }
  }

public:

  // iterator
  typedef map_t::const_iterator const_iterator;

  const_iterator begin() const { return PSetMap.begin(); }
  const_iterator end()   const { return PSetMap.end();   }

  // Check if empty
  bool empty() const { return PSetMap.empty(); }

private:
  map_t PSetMap;

  // Make the PSetParser class friend to allow the access of private members
  template<typename Iterator> friend class PSetParser;

};  // ParameterSet


// ======================================================================

#endif  // FHICL_CPP_PARAMETERSET_H
