#ifndef FHICLCPP__PARSER_H
#define FHICLCPP__PARSER_H

///////////////////////////////////////////////////////////////////////////////
//
// Parser
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


#endif  // FHICLCPP__PARSER_H
