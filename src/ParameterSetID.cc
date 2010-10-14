#include "ParameterSetID.h"
#include "ParameterSet.h"

#include "boost/format.hpp"

#include <string>

void fhicl::ParameterSetID::reset(fhicl::ParameterSet const & ps)
{
  cet::sha1 sha;
  std::string hash = ps.hashString();
  sha << hash.c_str();

  id_ = sha.digest();
  valid_ = true;
}

std::string fhicl::ParameterSetID::to_string() const
{
  std::string str;
  for(int i=0; i<id_.size(); ++i) {
    str += boost::str(boost::format("%02x")%(unsigned int)id_[i]);
  }
  return str;
}

