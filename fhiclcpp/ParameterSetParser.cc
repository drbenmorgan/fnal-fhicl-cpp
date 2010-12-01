// ======================================================================
//
// ParameterSetParser
//
// ======================================================================


#include "fhiclcpp/ParameterSetParser.h"

#include "boost/spirit/include/phoenix_bind.hpp"
#include "boost/spirit/include/phoenix_container.hpp"
#include "boost/spirit/include/phoenix_core.hpp"
#include "boost/spirit/include/phoenix_operator.hpp"
#include "boost/version.hpp"
#include "fhiclcpp/exception.h"
#include "fhiclcpp/ParameterSetRegistry.h"
#include "fhiclcpp/Parser.h"
#include <fstream>
#include <iostream>
#include <utility>


namespace fhicl
{

namespace qi = ::boost::spirit::qi;
namespace ascii = ::boost::spirit::ascii;
namespace phoenix = ::boost::phoenix;


template<typename Iterator>
PSetParser<Iterator>::PSetParser()
  : PSetParser::base_type(doc)
  , PrimaryPSet(boost::any(PSet()))
  , RefPool(boost::any(PSet()))
  , errs ()
{
  using qi::_1;
  using qi::_a;
  using qi::_val;
  using qi::lit;
  using qi::char_;
  using qi::int_;
  using boost::spirit::eol;
  using boost::spirit::raw;

  doc = *( re_assign );

  re_assign = ( ref_literal [_a=_1]
                >> ':'
                >> expr  [phoenix::bind(&PSetParser::setObjFromName,this,_a,_1)]
              ) % ','
              ;

  expr   =  nil                          [_val = _1]
         |  double_literal               [_val = _1]
         |  int_literal                  [_val = _1]
         |  bool_literal                 [_val = _1]
         |  str                          [_val = _1]
         |  pset                         [_val = _1]
         |  array                        [_val = _1]
         |  reference                    [_val = _1]
        ;


  pset   =   lit('{')
       >> -( assign [phoenix::bind(&PSetParser::insertPSetEntry,this,_val,_1)]
                % ',' )
       >>    lit('}') ;

  assign =  valid_key >> ':' >> expr ;

  array %= lit('[') >> -( expr % ',') >> ']' ;

  reference      = ref_literal [_a=_1] >> lit("@local")
                    [_val=phoenix::bind(&PSetParser::getObjFromName,this,_a)];

  ref_literal    = raw[    valid_key
                        >> *( char_('.') >> valid_key )
                        >> *( char_('[') >> int_ >> char_(']') )
                      ];

  valid_key = !keywords >> key;

  key   = qi::lexeme[ascii::char_("a-zA-Z_") >> *ascii::char_("a-zA-Z_0-9")];

  keywords = ( lit("nil") | lit("null") | lit("true") | lit("false") | lit("inf") )
             >> !ascii::char_("a-zA-Z_0-9") ;

  str  %= qi::lexeme['"' >> +(ascii::char_ - '"') >> '"'];

  double_literal = raw[qi::double_];
  int_literal    = raw[qi::int_];
  bool_literal   = raw[ lit("true") | lit("false") ];
  nil            = raw[ lit("nil")  | lit("null")  ];

  space = lit(' ') | lit('\t') | lit('\n')
        | lit("//")>> *( char_ - eol ) >> eol
        | lit('#') >> *( char_ - eol ) >> eol
        ;
}

template<typename Iterator>
boost::any PSetParser<Iterator>::getObjFromName(std::string & name)
{
  boost::any * obj = parseRef(name, false);
  return *obj;
}

template<typename Iterator>
void PSetParser<Iterator>::setObjFromName(std::string & name, boost::any & obj)
{
  boost::any * pobj = parseRef(name, true);
  pobj->swap(obj);
}

template<typename Iterator>
void PSetParser<Iterator>::insertPSetEntry(
     PSet & pset, std::pair<std::string, boost::any> const & pair)
{
  pset.insertEntryObj(pair.first, pair.second);
}


template<typename Iterator>
boost::any * PSetParser<Iterator>::parseRef(
              std::string & str, bool bInsert)
{
  if(str.empty())
    throw std::runtime_error("Empty name not allowed!");

  std::string::iterator first = str.begin();
  std::string::iterator last  = str.end();

  using qi::int_;
  using qi::_val;
  using qi::_1;
  using qi::_a;
  using qi::_b;
  using qi::lit;

  boost::any * obj = &PrimaryPSet;

  typedef BOOST_TYPEOF(ascii::space
      | qi::lit('#') >>*(qi::char_ - boost::spirit::eol) >> boost::spirit::eol
      | qi::lit("//")>>*(qi::char_ - boost::spirit::eol) >> boost::spirit::eol
  ) skipper_type;

  qi::rule<std::string::iterator, void(), skipper_type> ref;

  ref =  key               [phoenix::ref(obj)=phoenix::bind(
                               &PSetParser::findPSetPtr, this,
                               phoenix::ref(obj), _1, phoenix::ref(bInsert))]
   >> *(  lit('.') >> key  [phoenix::ref(obj)=phoenix::bind(
                               &PSetParser::findPSetPtr, this,
                               phoenix::ref(obj), _1, phoenix::ref(bInsert))]
       )
   >> *(  lit('[') >> int_ [phoenix::ref(obj)=phoenix::bind(
                               &PSetParser::findArrayElementPtr, this,
                               phoenix::ref(obj), _1, phoenix::ref(bInsert))]
                   >> lit(']')
       ) ;


  bool r = qi::phrase_parse(
      first,
      last,
      ref,
      ascii::space
      | lit('#') >>*(qi::char_ - boost::spirit::eol) >> boost::spirit::eol
      | lit("//")>>*(qi::char_ - boost::spirit::eol) >> boost::spirit::eol
  );

  if(r && first==last)
  {
    return obj;
  }
  else
  {
    std::string err = "reference parse error when parsing \""
                      + str + "\"";
    throw fhicl::exception(parse_error, err);
  }
}

template<typename Iterator>
boost::any *
PSetParser<Iterator>::findPSetPtr(
    boost::any * object, std::string const & name, bool bInsert)
{
  try
  {
    PSet & pset = boost::any_cast<PSet &>(*object);
    boost::any * obj = pset.getPSetObjPtr(name, bInsert);
    return obj;
  }
  catch(const boost::bad_any_cast &)
  {
    std::string err = "The left to \"" + name
                      + "\" is not a ParameterSet entry";
    errs.push_back(err);
    throw fhicl::exception(parse_error, err);
  }
}

template<typename Iterator>
boost::any *
PSetParser<Iterator>::findArrayElementPtr(
    boost::any * object, int idx, bool bInsert)
{
  try
  {
    std::vector<boost::any> & array
        = boost::any_cast<std::vector<boost::any> & >(*object);

    // fill the missing elements with NIL object
    if(idx >= array.size())
    {
      if(bInsert)
      {
        array.resize(idx+1, boost::any(std::string("nil")));
        return &array[idx];
      }
      else
      {
        std::string err = "Invalid array index";
        errs.push_back(err);
        throw fhicl::exception(parse_error, err);
      }
    }

    return &array[idx];
  }
  catch(const boost::bad_any_cast &)
  {
    std::string err = "The left to [] is not an array object";
    errs.push_back(err);
    throw fhicl::exception(parse_error, err);
  }
}

// Helper functions for making a parameter set object from a PSet object
namespace {

  bool isAtom_(boost::any const & val)
  {
    return val.type() == typeid(std::string);
  }

  bool isPSet_(boost::any const & val)
  {
    return val.type() == typeid(PSet);
  }

  bool isVector_(boost::any const & val)
  {
    return val.type() == typeid(std::vector<boost::any>);
  }

  // declaration
  boost::any encode_(boost::any value);
  void MakePSet_(ParameterSet & pset, PSet const & ps);

  // definition
  boost::any encode_(boost::any value)
  {
    if (isAtom_(value))
      return value;

    if (isPSet_(value))
    {
      ParameterSet ps;
      MakePSet_(ps, boost::any_cast<PSet>(value));
      return boost::any(ParameterSetRegistry::put(ps));
    }

    if (isVector_(value))
    {
      std::vector<boost::any> val =
          boost::any_cast<std::vector<boost::any> >(value);
      std::vector<boost::any> result;
      std::transform( val.begin(), val.end()
                    , std::back_inserter(result)
                    , encode_
                    );
      return boost::any(result);
    }
  }

  void MakePSet_(ParameterSet & pset, PSet const & ps)
  {
    PSet::const_iterator it = ps.begin();

    while(it!=ps.end()) {
      pset.insert(it->first, encode_(it->second));
      ++it;
    }
  }

} // end of annonymous namespace

template<typename Iterator>
ParameterSet PSetParser<Iterator>::getPSet()
{
  ParameterSet pset;
  MakePSet_(pset, boost::any_cast<PSet>(PrimaryPSet));
  return pset;
}


//---------------------------------------------------------------------------
// Wrapper Parser methods

bool Parser::ParseFile(std::string const & fname, ParameterSet & pset)
{
    std::string storage;

    PreProcess(fname, storage);

#if 0
    std::ifstream in(fname.c_str(), std::ios_base::in);

    if (!in)
    {
        std::cerr << "Error: Could not open configuration file: "
            << fname << std::endl;
        return false;
    }

    std::string storage; // We will read the contents here.
    in.unsetf(std::ios::skipws); // No white space skipping!
    std::copy(
        std::istream_iterator<char>(in),
        std::istream_iterator<char>(),
        std::back_inserter(storage));
#endif

    // parse the configuration string
    return ParseString(storage, pset);
}

bool Parser::PreProcess(std::string const & fname, std::string & storage)
{
    std::ifstream in(fname.c_str(), std::ios_base::in);

    if (!in)
    {
        std::cerr << "Error: Could not open configuration file: "
            << fname << std::endl;
        return false;
    }

    std::string line;

    while ( !in.eof() )
    {
        std::getline(in, line);
        TrimSpace(line);

        size_t pos = line.find("#include");

        if ( pos!=0 )
        {
            storage += line;
            storage += "\n";
            continue;
        }

        size_t start = line.find_first_of("<\"");
        size_t end   = line.find_last_of (">\"");

        if ( (start==std::string::npos) || (end==std::string::npos)
             || (start==end) || (start+1==end) )
        {
            std::cerr << "Error: Syntax error of #include in file: "
                      << fname << std::endl;
            return false;
        }

        std::string file = line.substr(start+1, end-start-1);

        if ( PreProcess(file, storage) == false )
            return false;

    }

    in.close();
    return true;
}

void Parser::TrimSpace( std::string & str )
{
    size_t startpos = str.find_first_not_of(" \t");
    size_t endpos   = str.find_last_not_of(" \t");

    if( (std::string::npos == startpos ) || (std::string::npos == endpos) )
        str = "";
    else
        str = str.substr( startpos, endpos-startpos+1 );
}


bool Parser::ParseString(std::string const & pset_str, ParameterSet & pset)
{
    // make sure the string ends with a white space 
    std::string str = pset_str + ' '; 

    namespace qi = boost::spirit::qi;
    namespace ascii = boost::spirit::ascii;

    using boost::spirit::eol;

    PSetParser<std::string::iterator> p;

    std::string::iterator iter = str.begin();
    std::string::iterator end  = str.end();

    bool r = qi::phrase_parse(
                 iter,
                 end,
                 p,
                 ascii::space
                   | qi::lit('#') >>*(qi::char_ - eol) >> eol
                   | qi::lit("//")>>*(qi::char_ - eol) >> eol
             );

    if (r && (iter==end) )
    {
        //std::cout << "Parsing succeeded\n";
        pset = p.getPSet();
        //p.print();
        return true;
    }
    else
    {
        std::cout << "Parsing failed! "<< *iter << *(iter+1) << "\n";
        std::vector<std::string> errs = p.getErrorMsgs();
        for(int i=0;i<errs.size();++i)
          std::cout << errs[i] << "\n";
        return false;
    }
}

}  //namespace fhicl
