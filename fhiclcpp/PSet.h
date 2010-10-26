#ifndef FHICLCPP__PSET_H
#define FHICLCPP__PSET_H

// ======================================================================
//
// PSet - the intermediate representation for ParameterSetParser
//
// ======================================================================


#include "boost/any.hpp"
#include "fhiclcpp/exception.h"
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

  void insertEntryObj(std::string const & key, boost::any const & value)
  { PSetMap.insert(std::make_pair(key, value)); }


  boost::any * getPSetObjPtr(std::string const & name, bool bInsert)
  {
    map_t::iterator it = PSetMap.find(name);

    if(it!=PSetMap.end())
      return &(it->second);

    if(bInsert)
    {
      insertEntryObj(name, boost::any(std::string("nil")));
      return getPSetObjPtr(name, false);
    }
    else
    {
      throw fhicl::exception(cant_find, name);
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

};  // PSet


// ======================================================================

#endif  // FHICLCPP__PSET_H
