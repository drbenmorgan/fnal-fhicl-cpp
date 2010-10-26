#ifndef FHICLCPP__PARAMETERSETPARSER_H
#define FHICLCPP__PARAMETERSETPARSER_H

///////////////////////////////////////////////////////////////////////////////
//
// ParameterSetParser
//
///////////////////////////////////////////////////////////////////////////////


#include "boost/any.hpp"
#include "boost/config/warning_disable.hpp"
#include "boost/fusion/include/std_pair.hpp"
#include "boost/spirit/include/qi.hpp"
#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/PSet.h"
#include <string>
#include <utility>  // pair
#include <vector>


namespace fhicl
{

namespace qi = ::boost::spirit::qi;
namespace ascii = ::boost::spirit::ascii;
namespace phoenix = ::boost::phoenix;

typedef BOOST_TYPEOF(ascii::space
    | qi::lit('#') >>*(qi::char_ - boost::spirit::eol) >> boost::spirit::eol
    | qi::lit("//")>>*(qi::char_ - boost::spirit::eol) >> boost::spirit::eol
) skipper_type;


template <typename Iterator>
struct PSetParser
: qi::grammar<Iterator, skipper_type>
{
  PSetParser();

  ParameterSet             getPSet();
  std::vector<std::string> getErrorMsgs() { return errs; };

private:

  boost::any * findPSetPtr(boost::any *, std::string const &, bool);
  boost::any * findArrayElementPtr(boost::any *, int, bool);

  boost::any   getObjFromName(std::string & name);
  void         setObjFromName(std::string & name, boost::any & obj);
  boost::any * parseRef(std::string &, bool);

  void insertPSetEntry(PSet &, std::pair<std::string, boost::any> const&);

  // Qi parser rules
  qi::rule<Iterator, skipper_type> doc;

  qi::rule<Iterator, void(), qi::locals<std::string>, skipper_type> re_assign;
  qi::rule<Iterator, boost::any(), skipper_type> expr;

  qi::rule<Iterator, std::string(), skipper_type> nil;
  qi::rule<Iterator, std::string(), skipper_type> double_literal;
  qi::rule<Iterator, std::string(), skipper_type> int_literal;
  qi::rule<Iterator, std::string(), skipper_type> bool_literal;
  qi::rule<Iterator, std::vector<boost::any>(), skipper_type> array;
  qi::rule<Iterator, PSet(), skipper_type> pset;
  qi::rule<Iterator, boost::any(), qi::locals<std::string>, skipper_type> reference;
  qi::rule<Iterator, std::string(), skipper_type> ref_literal;

  qi::rule<Iterator, std::string() , skipper_type> key;
  qi::rule<Iterator, std::string() , skipper_type> valid_key;
  qi::rule<Iterator, void()        , skipper_type> keywords;

  qi::rule<Iterator, std::string() , skipper_type> str;

  qi::rule<Iterator, std::pair<std::string, boost::any>(), skipper_type> assign;

  qi::rule<Iterator> space;

  // data structure for primary entries
  boost::any PrimaryPSet;
  boost::any RefPool;

  // vector of string to hold parsing errors
  std::vector<std::string> errs;
};

}  // namespace fhicl

#endif  // FHICLCPP__PARAMETERSETPARSER_H
