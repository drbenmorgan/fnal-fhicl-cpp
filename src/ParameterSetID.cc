#include "ParameterSetID.h"
#include "ParameterSet.h"
#include "sha1.h"

#include "cetlib/sha1.h"

#include "boost/format.hpp"

#include <string>

void fhicl::ParameterSetID::reset(fhicl::ParameterSet const & ps)
{
  SHA1 sha;
  std::string hash = ps.hashString();
  sha << hash.c_str();

  unsigned int buf[5];
  sha.Result(buf);

  for(int i=0; i<5; ++i) id_[i]=buf[i];
  valid_ = true;
}

std::string fhicl::ParameterSetID::to_string() const
{
  std::string str;
  str = boost::str(boost::format("%08X%08X%08X%08X%08X")
             %id_[0]%id_[1]%id_[2]%id_[3]%id_[4]);
  return str;
}

