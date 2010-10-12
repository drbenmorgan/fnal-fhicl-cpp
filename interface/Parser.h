#ifndef FHICL_CPP_PARSER_H
#define FHICL_CPP_PARSER_H

///////////////////////////////////////////////////////////////////////////////
//
// ParameterSetParser
//
///////////////////////////////////////////////////////////////////////////////


#include <string>

namespace fhicl {
  class ParameterSet;
  class Parser;
}

class fhicl::Parser
{
private:
  Parser() {}

public:
  static bool Parse(std::string const &, ParameterSet &);
  static bool ParseString(std::string &, ParameterSet &);

private:
  static bool PreProcess(std::string const &, std::string &);
  static void TrimSpace(std::string &);
};

#endif  // FHICL_CPP_PARSER_H
