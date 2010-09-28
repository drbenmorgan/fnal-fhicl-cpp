#include "ParameterSetID.h"
#include "ParameterSet.h"
#include "sha1.h"

#include <string>

void fhicl::ParameterSetID::reset(fhicl::ParameterSet const & ps)
{
  SHA1 sha;
  sha.Reset();
  std::string hash = ps.hashString();
  sha << hash.c_str();
  sha.Result((unsigned int*)(&id_));
  valid_ = true;
}
