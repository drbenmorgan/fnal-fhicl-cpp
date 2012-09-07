#ifndef  FHICLCPP_TOKENS_H
#define  FHICLCPP_TOKENS_H

// ======================================================================
//
// tokens
//
// ======================================================================

#include "boost/spirit/home/support/terminal.hpp"
#include "boost/spirit/include/qi.hpp"
#include "cetlib/canonical_number.h"
#include "cpp0x/cstddef"
#include "cpp0x/string"
#include <cctype>

namespace qi = boost::spirit::qi;

// ----------------------------------------------------------------------
// utilities:

namespace fhicl {

  inline  bool
    maximally_munched( char ch )
    {
      return ! std::isgraph(ch)
             || ch == '#' || ch == '/'
             || ch == ',' || ch == ']' || ch == '}'
             ;
    }

  inline  bool
    maximally_munched_number( char ch )
    {
      return maximally_munched(ch) || ch == ')';
    }

  inline  bool
    maximally_munched_ass( char ch )
    {
      return maximally_munched(ch) || ch == '.' || ch == '[' || ch == ':';
    }

  inline  bool
    maximally_munched_dss( char ch )
    {
      return maximally_munched(ch);
    }

}  // fhicl

// ----------------------------------------------------------------------
// identify custom terminal symbols:

namespace fhicl {

//  BOOST_SPIRIT_TERMINAL(Nil)
//  BOOST_SPIRIT_TERMINAL(Boolean)
  BOOST_SPIRIT_TERMINAL(real)
  BOOST_SPIRIT_TERMINAL(uint)
  BOOST_SPIRIT_TERMINAL(hex)
  BOOST_SPIRIT_TERMINAL(bin)
  BOOST_SPIRIT_TERMINAL(ass)
  BOOST_SPIRIT_TERMINAL(dss)
//  BOOST_SPIRIT_TERMINAL(Dquoted)
//  BOOST_SPIRIT_TERMINAL(Squoted)
//  BOOST_SPIRIT_TERMINAL(Target)
//  BOOST_SPIRIT_TERMINAL(Ref)

}

// ----------------------------------------------------------------------
// identify our tokens as terminals and in qi::domain (only for parsing):

namespace boost { namespace spirit {

//  template<>
//    struct use_terminal<qi::domain, fhicl::tag::Nil>
//  : mpl::true_
//  { };

//  template<>
//    struct use_terminal<qi::domain, fhicl::tag::Boolean>
//  : mpl::true_
//  { };

  template<>
    struct use_terminal<qi::domain, fhicl::tag::real>
  : mpl::true_
  { };

  template<>
    struct use_terminal<qi::domain, fhicl::tag::uint>
  : mpl::true_
  { };

  template<>
    struct use_terminal<qi::domain, fhicl::tag::hex>
  : mpl::true_
  { };

  template<>
    struct use_terminal<qi::domain, fhicl::tag::bin>
  : mpl::true_
  { };

  template<>
    struct use_terminal<qi::domain, fhicl::tag::ass>
  : mpl::true_
  { };

  template<>
    struct use_terminal<qi::domain, fhicl::tag::dss>
  : mpl::true_
  { };

//  template<>
//    struct use_terminal<qi::domain, fhicl::tag::Dquoted>
//  : mpl::true_
//  { };

//  template<>
//    struct use_terminal<qi::domain, fhicl::tag::Squoted>
//  : mpl::true_
//  { };

//  template<>
//    struct use_terminal<qi::domain, fhicl::tag::Target>
//  : mpl::true_
//  { };

//  template<>
//    struct use_terminal<qi::domain, fhicl::tag::Ref>
//  : mpl::true_
//  { };

} }  // boost::spirit

// ----------------------------------------------------------------------
// implement each token parser:

namespace fhicl {

//  struct Nil_parser
//    : qi::primitive_parser<Nil_parser>
//  {
//    // designate type resulting from successful parse:
//    template< typename Context
//            , typename Iterator
//            >
//      struct attribute
//    { typedef std::string type; };

//    // do the parse:
//    template< typename Iterator
//            , typename Context
//            , typename Skipper
//            , typename Attribute
//            >
//    bool
//      parse( Iterator & first, Iterator const & last
//           , Context & // unused
//           , Skipper const & skipper
//           , Attribute & attr
//           ) const
//    {
//      boost::spirit::qi::skip_over( first, last, skipper );
//      #if 0
//      std::cerr << "Nil about to parse <"
//                << std::string(first,last) << ">\n";
//      #endif

//      qi::rule<Iterator, Attribute()>
//        Nil_rule(  qi::raw[ qi::lit("@nil" ) ]
//                >> !( ascii::graph
//                    - ascii::char_(')')
//                    - ascii::char_(',')
//                    - ascii::char_(']')
//                    - ascii::char_('}')
//                    )
//                );
//      Attribute raw;
//      if( ! qi::parse(first, last, Nil_rule, raw) )
//        return false;

//      Attribute result = raw;

//      boost::spirit::traits::assign_to(result, attr);
//      return true;
//    }

//    // identify this token (in case of error):
//    template< typename Context >
//      boost::spirit::info what( Context & /*unused */ ) const
//    { return boost::spirit::info("fhicl::Nil"); }

//  };  // Nil_parser

//  struct Boolean_parser
//    : qi::primitive_parser<Boolean_parser>
//  {
//    // designate type resulting from successful parse:
//    template< typename Context
//            , typename Iterator
//            >
//      struct attribute
//    { typedef std::string type; };

//    // do the parse:
//    template< typename Iterator
//            , typename Context
//            , typename Skipper
//            , typename Attribute
//            >
//    bool
//      parse( Iterator & first, Iterator const & last
//           , Context & // unused
//           , Skipper const & skipper
//           , Attribute & attr
//           ) const
//    {
//      boost::spirit::qi::skip_over( first, last, skipper );
//      #if 0
//      std::cerr << "Boolean about to parse <"
//                << std::string(first,last) << ">\n";
//      #endif

//      qi::rule<Iterator, Attribute()>
//        Boolean_rule(  qi::raw[ qi::lit("true") | qi::lit("false") ]
//                    >> !( ascii::graph
//                        - ascii::char_(')')
//                        - ascii::char_(',')
//                        - ascii::char_(']')
//                        - ascii::char_('}')
//                        )
//                    );
//      Attribute raw;
//      if( ! qi::parse(first, last, Boolean_rule, raw) )
//        return false;

//      Attribute result = raw;

//      boost::spirit::traits::assign_to(result, attr);
//      return true;
//    }

//    // identify this token (in case of error):
//    template< typename Context >
//      boost::spirit::info what( Context & /*unused */ ) const
//    { return boost::spirit::info("fhicl::Boolean"); }

//  };  // Boolean_parser

  struct real_parser
    : qi::primitive_parser<real_parser>
  {
    // designate type resulting from successful parse:
    template< typename Context
            , typename Iterator
            >
      struct attribute
    { typedef std::string type; };

    // do the parse:
    template< typename Iterator
            , typename Context
            , typename Skipper
            , typename Attribute
            >
    bool
      parse( Iterator & first, Iterator const & last
           , Context & // unused
           , Skipper const & skipper
           , Attribute & attr
           ) const
    {
      boost::spirit::qi::skip_over( first, last, skipper );
      #if 0
      std::cerr << "real about to parse <"
                << std::string(first,last) << ">\n";
      #endif

      static  std::string const allowed("0123456789.-+eE");
      Iterator it = first;
      while( it != last  &&  allowed.find(*it) != std::string::npos )
        ++it;

      if( it != last  && ! maximally_munched_number(*it) )
        return false;

      Attribute raw(first, it);
      if( raw.empty() )
        return false;

      Attribute result;
      if( ! cet::canonical_number(raw, result) )
        return false;

      first = it;
      boost::spirit::traits::assign_to(result, attr);
      return true;
    }

    // identify this token (in case of error):
    template< typename Context >
      boost::spirit::info what( Context & /*unused */ ) const
    { return boost::spirit::info("fhicl::real"); }

  };  // real_parser

  struct uint_parser
    : boost::spirit::qi::primitive_parser<uint_parser>
  {
    // designate type resulting from successful parse:
    template< typename Context
            , typename Iterator
            >
      struct attribute
    { typedef std::string type; };

    // do the parse:
    template< typename Iterator
            , typename Context
            , typename Skipper
            , typename Attribute
            >
    bool
      parse( Iterator & first, Iterator const & last
           , Context & // unused
           , Skipper const & skipper
           , Attribute & attr
           ) const
    {
      boost::spirit::qi::skip_over( first, last, skipper );
      #if 0
      std::cerr << "uint about to parse <"
                << std::string(first,last) << ">\n";
      #endif

      Iterator it = first;
      while( it != last  &&  std::isdigit(*it) )
        ++it;
      Attribute result(first, it);

      if(  result.empty() )
        return false;

      if( it != last  && ! maximally_munched_number(*it) )
        return false;

      for( std::size_t ndig = result.size()
         ; ndig > 1  &&  result[0] == '0'; --ndig )
        result.erase( 0, 1 );

      first = it;
      boost::spirit::traits::assign_to(result, attr);
      return true;
    }

    // identify this token (in case of error):
    template< typename Context >
      boost::spirit::info what( Context & /*unused */ ) const
    { return boost::spirit::info("fhicl::uint"); }

  };  // uint_parser

  struct hex_parser
    : qi::primitive_parser<hex_parser>
  {
    // designate type resulting from successful parse:
    template< typename Context
            , typename Iterator
            >
      struct attribute
    { typedef std::string type; };

    // do the parse:
    template< typename Iterator
            , typename Context
            , typename Skipper
            , typename Attribute
            >
    bool
      parse( Iterator & first, Iterator const & last
           , Context & // unused
           , Skipper const & skipper
           , Attribute & attr
           ) const
    {
      boost::spirit::qi::skip_over( first, last, skipper );
      #if 0
      std::cerr << "hex about to parse <"
                << std::string(first,last) << ">\n";
      #endif

      static  std::string const allowed("0123456789abcdefABCDEF");
      Iterator it = first;

      if( it==last || *it!='0' )
        return false;

      ++it;

      if( it==last || toupper(*it)!='X' )
        return false;

      ++it;

      while( it != last  &&  allowed.find(*it) != std::string::npos )
        ++it;

      if( it != last  && ! maximally_munched_number(*it) )
        return false;

      Attribute raw(first, it);
      if( raw.empty() || raw.size()==2 )
        return false;

      Attribute result;
      if( ! cet::canonical_number(raw, result) )
        return false;

      first = it;
      boost::spirit::traits::assign_to(result, attr);
      return true;
    }

    // identify this token (in case of error):
    template< typename Context >
      boost::spirit::info what( Context & /*unused */ ) const
    { return boost::spirit::info("fhicl::hex"); }

  };  // hex_parser


  struct bin_parser
    : qi::primitive_parser<bin_parser>
  {
    // designate type resulting from successful parse:
    template< typename Context
            , typename Iterator
            >
      struct attribute
    { typedef std::string type; };

    // do the parse:
    template< typename Iterator
            , typename Context
            , typename Skipper
            , typename Attribute
            >
    bool
      parse( Iterator & first, Iterator const & last
           , Context & // unused
           , Skipper const & skipper
           , Attribute & attr
           ) const
    {
      boost::spirit::qi::skip_over( first, last, skipper );
      #if 0
      std::cerr << "bin about to parse <"
                << std::string(first,last) << ">\n";
      #endif

      static  std::string const allowed("01");
      Iterator it = first;

      if( it==last || *it!='0' )
        return false;

      ++it;

      if( it==last || toupper(*it)!='B' )
        return false;

      ++it;

      while( it != last  &&  allowed.find(*it) != std::string::npos )
        ++it;

      if( it != last  && ! maximally_munched_number(*it) )
        return false;

      Attribute raw(first, it);
      if( raw.empty() || raw.size()==2 )
        return false;

      Attribute result;
      if( ! cet::canonical_number(raw, result) )
        return false;

      first = it;
      boost::spirit::traits::assign_to(result, attr);
      return true;
    }

    // identify this token (in case of error):
    template< typename Context >
      boost::spirit::info what( Context & /*unused */ ) const
    { return boost::spirit::info("fhicl::bin"); }

  };  // bin_parser


  struct ass_parser
    : qi::primitive_parser<ass_parser>
  {
    // designate type resulting from successful parse:
    template< typename Context
            , typename Iterator
            >
      struct attribute
    { typedef std::string type; };

    // do the parse:
    template< typename Iterator
            , typename Context
            , typename Skipper
            , typename Attribute
            >
    bool
      parse( Iterator & first, Iterator const & last
           , Context & // unused
           , Skipper const & skipper
           , Attribute & attr
           ) const
    {
      boost::spirit::qi::skip_over( first, last, skipper );
      #if 0
      std::cerr << "ass about to parse <"
                << std::string(first,last) << ">\n";
      #endif

      Iterator it = first;
      while( it != last  &&  (std::isalnum(*it) || *it == '_') )
        ++it;

      if( it != last  && ! maximally_munched_ass(*it) )
        return false;

      Attribute result(first, it);
      if( result.empty() || std::isdigit(result[0]) )
        return false;

      first = it;
      boost::spirit::traits::assign_to(result, attr);
      return true;
    }

    // identify this token (in case of error):
    template< typename Context >
      boost::spirit::info what( Context & /*unused */ ) const
    { return boost::spirit::info("fhicl::ass"); }

  };  // ass_parser

  struct dss_parser
    : qi::primitive_parser<dss_parser>
  {
    // designate type resulting from successful parse:
    template< typename Context
            , typename Iterator
            >
      struct attribute
    { typedef std::string type; };

    // do the parse:
    template< typename Iterator
            , typename Context
            , typename Skipper
            , typename Attribute
            >
    bool
      parse( Iterator & first, Iterator const & last
           , Context & // unused
           , Skipper const & skipper
           , Attribute & attr
           ) const
    {
      boost::spirit::qi::skip_over( first, last, skipper );
      #if 0
      std::cerr << "dss about to parse <"
                << std::string(first,last) << ">\n";
      #endif

      bool all_digits = true;
      Iterator it = first;
      for( ; it != last  &&  (std::isalnum(*it) || *it == '_'); ++it )
        all_digits = all_digits && std::isdigit(*it);

      if( it != last  &&  ! maximally_munched_dss(*it) )
        return false;

      Attribute result(first, it);
      if( result.empty()  || all_digits  ||  ! std::isdigit(result[0]) )
        return false;

      first = it;
      boost::spirit::traits::assign_to(result, attr);
      return true;
    }

    // identify this token (in case of error):
    template< typename Context >
      boost::spirit::info what( Context & /*unused */ ) const
    { return boost::spirit::info("fhicl::dss"); }

  };  // dss_parser

//  struct Dquoted_parser
//    : qi::primitive_parser<Dquoted_parser>
//  {
//    // designate type resulting from successful parse:
//    template< typename Context
//            , typename Iterator
//            >
//      struct attribute
//    { typedef std::string type; };

//    // do the parse:
//    template< typename Iterator
//            , typename Context
//            , typename Skipper
//            , typename Attribute
//            >
//    bool
//      parse( Iterator & first, Iterator const & last
//           , Context & // unused
//           , Skipper const & skipper
//           , Attribute & attr
//           ) const
//    {
//      boost::spirit::qi::skip_over( first, last, skipper );
//      #if 0
//      std::cerr << "Dquoted about to parse <"
//                << std::string(first,last) << ">\n";
//      #endif

//      if( first == last )
//        return false;
//      if( *first != '\"' )
//        return false;
//      Iterator it = first;
//      while( ++it != last && *it != '\"' ) {
//        char ch = *it;
//        if( ch == '\\' && it != last-1 ) {
//          switch( *++it ) {
//            case '\"':
//            case '\'':
//            case '\\':
//            case 'n' :
//            case 't' :  break;
//            default  :  return false;
//          }
//        }
//      }
//      if( it == last )
//        return false;

//      Attribute raw(first, ++it);

//      Attribute result;
//      if( ! cet::canonical_string( raw, result ) )
//        return false;

//      boost::spirit::traits::assign_to(result, attr);
//      return true;
//    }

//    // identify this token (in case of error):
//    template< typename Context >
//      boost::spirit::info what( Context & /*unused */ ) const
//    { return boost::spirit::info("fhicl::Dquoted"); }

//  };  // Dquoted_parser

//  struct Squoted_parser
//    : qi::primitive_parser<Squoted_parser>
//  {
//    // designate type resulting from successful parse:
//    template< typename Context
//            , typename Iterator
//            >
//      struct attribute
//    { typedef std::string type; };

//    // do the parse:
//    template< typename Iterator
//            , typename Context
//            , typename Skipper
//            , typename Attribute
//            >
//    bool
//      parse( Iterator & first, Iterator const & last
//           , Context & // unused
//           , Skipper const & skipper
//           , Attribute & attr
//           ) const
//    {
//      boost::spirit::qi::skip_over( first, last, skipper );
//      #if 0
//      std::cerr << "Squoted about to parse <"
//                << std::string(first,last) << ">\n";
//      #endif

//      if( first == last )
//        return false;
//      if( *first != '\'' )
//        return false;
//      Iterator it = first;
//      while( ++it != last && *it != '\'' )
//        ;
//      if( it == last )
//        return false;

//      Attribute raw(first, ++it);

//      Attribute result;
//      if( ! cet::canonical_string( raw, result ) )
//        return false;

//      boost::spirit::traits::assign_to(result, attr);
//      return true;
//    }

//    // identify this token (in case of error):
//    template< typename Context >
//      boost::spirit::info what( Context & /*unused */ ) const
//    { return boost::spirit::info("fhicl::Squoted"); }

//  };  // Squoted_parser

//  struct Target_parser
//    : qi::primitive_parser<Target_parser>
//  {
//    // designate type resulting from successful parse:
//    template< typename Context
//            , typename Iterator
//            >
//      struct attribute
//    { typedef std::string type; };

//    // do the parse:
//    template< typename Iterator
//            , typename Context
//            , typename Skipper
//            , typename Attribute
//            >
//    bool
//      parse( Iterator & first, Iterator const & last
//           , Context & // unused
//           , Skipper const & skipper
//           , Attribute & attr
//           ) const
//    {
//      #if 0
//      std::cerr << "Target about to parse <"
//                << std::string(first,last) << ">\n";
//      #endif

//      Attribute result;
//      if( ! qi::phrase_parse(first, last, ass, skipper, result) )
//        return false;

//      bool at_least_one = false;
//      for( bool done = false; ! done;  )  {
//        if( first == last )
//          break;
//        Attribute raw;
//        if(  qi::phrase_parse(first, last, qi::lit('.'), skipper)
//          && qi::phrase_parse(first, last, ass, skipper, raw)
//          ) {
//          at_least_one = true;
//          result.append(1, '.')
//                .append(raw);
//        }
//        else if(  qi::phrase_parse(first, last, qi::lit('['), skipper)
//               && qi::phrase_parse(first, last, uint, skipper, raw)
//               && qi::phrase_parse(first, last, qi::lit(']'), skipper)
//               ) {
//          at_least_one = true;
//          result.append(1, '[')
//                .append(raw)
//                .append(1, ']');
//        }
//        else
//          done = true;
//      }  // for

//      if( at_least_one )
//        boost::spirit::traits::assign_to(result, attr);
//      return at_least_one;
//    }

//    // identify this token (in case of error):
//    template< typename Context >
//      boost::spirit::info what( Context & /*unused */ ) const
//    { return boost::spirit::info("fhicl::Target"); }

//  };  // Target_parser

//  struct Ref_parser
//    : qi::primitive_parser<Ref_parser>
//  {
//    // designate type resulting from successful parse:
//    template< typename Context
//            , typename Iterator
//            >
//      struct attribute
//    { typedef std::string type; };

//    // do the parse:
//    template< typename Iterator
//            , typename Context
//            , typename Skipper
//            , typename Attribute
//            >
//    bool
//      parse( Iterator & first, Iterator const & last
//           , Context & // unused
//           , Skipper const & skipper
//           , Attribute & attr
//           ) const
//    {
//      #if 0
//      std::cerr << "Ref about to parse <"
//                << std::string(first,last) << ">\n";
//      #endif

//      Attribute raw, result;
//      if( qi::phrase_parse( first, last, qi::lit("local") | qi::lit("db"), skipper, result )
//       && qi::phrase_parse(first, last, qi::lit("::"), skipper)
//       && qi::phrase_parse(first, last, Target, skipper, raw)
//        )
//        result.append("::")
//              .append(raw);

//      else if( ! qi::phrase_parse(first, last, ass, skipper, result) )
//        return false;

//      boost::spirit::traits::assign_to(result, attr);
//      return true;
//    }

//    // identify this token (in case of error):
//    template< typename Context >
//      boost::spirit::info what( Context & /*unused */ ) const
//    { return boost::spirit::info("fhicl::Ref"); }

//  };  // Ref_parser

}  // fhicl

// ----------------------------------------------------------------------
// provide factory functions to create instances of our parser:

namespace boost { namespace spirit { namespace qi {

//  template< typename Modifiers >
//    struct make_primitive<fhicl::tag::Nil, Modifiers>
//  {
//    typedef  fhicl::Nil_parser  result_type;

//    result_type
//      operator () ( unused_type, unused_type ) const
//    { return result_type(); }

//  };  // make_primitive<...Nil...>

//  template< typename Modifiers >
//    struct make_primitive<fhicl::tag::Boolean, Modifiers>
//  {
//    typedef  fhicl::Boolean_parser  result_type;

//    result_type
//      operator () ( unused_type, unused_type ) const
//    { return result_type(); }

//  };  // make_primitive<...Boolean...>

  template< typename Modifiers >
    struct make_primitive<fhicl::tag::real, Modifiers>
  {
    typedef  fhicl::real_parser  result_type;

    result_type
      operator () ( unused_type, unused_type ) const
    { return result_type(); }

  };  // make_primitive<...real...>

  template< typename Modifiers >
    struct make_primitive<fhicl::tag::uint, Modifiers>
  {
    typedef  fhicl::uint_parser  result_type;

    result_type
      operator () ( unused_type, unused_type ) const
    { return result_type(); }

  };  // make_primitive<...uint...>

  template< typename Modifiers >
    struct make_primitive<fhicl::tag::hex, Modifiers>
  {
    typedef  fhicl::hex_parser  result_type;

    result_type
      operator () ( unused_type, unused_type ) const
    { return result_type(); }

  };  // make_primitive<...hex...>

  template< typename Modifiers >
    struct make_primitive<fhicl::tag::bin, Modifiers>
  {
    typedef  fhicl::bin_parser  result_type;

    result_type
      operator () ( unused_type, unused_type ) const
    { return result_type(); }

  };  // make_primitive<...bin...>

  template< typename Modifiers >
    struct make_primitive<fhicl::tag::ass, Modifiers>
  {
    typedef  fhicl::ass_parser  result_type;

    result_type
      operator () ( unused_type, unused_type ) const
    { return result_type(); }

  };  // make_primitive<...ass...>

  template< typename Modifiers >
    struct make_primitive<fhicl::tag::dss, Modifiers>
  {
    typedef  fhicl::dss_parser  result_type;

    result_type
      operator () ( unused_type, unused_type ) const
    { return result_type(); }

  };  // make_primitive<...dss...>

//  template< typename Modifiers >
//    struct make_primitive<fhicl::tag::Dquoted, Modifiers>
//  {
//    typedef  fhicl::Dquoted_parser  result_type;

//    result_type
//      operator () ( unused_type, unused_type ) const
//    { return result_type(); }

//  };  // make_primitive<...Dquoted...>

//  template< typename Modifiers >
//    struct make_primitive<fhicl::tag::Squoted, Modifiers>
//  {
//    typedef  fhicl::Squoted_parser  result_type;

//    result_type
//      operator () ( unused_type, unused_type ) const
//    { return result_type(); }

//  };  // make_primitive<...Squoted...>

//  template< typename Modifiers >
//    struct make_primitive<fhicl::tag::Target, Modifiers>
//  {
//    typedef  fhicl::Target_parser  result_type;

//    result_type
//      operator () ( unused_type, unused_type ) const
//    { return result_type(); }

//  };  // make_primitive<...Target...>

//  template< typename Modifiers >
//    struct make_primitive<fhicl::tag::Ref, Modifiers>
//  {
//    typedef  fhicl::Ref_parser  result_type;

//    result_type
//      operator () ( unused_type, unused_type ) const
//    { return result_type(); }

//  };  // make_primitive<...Ref...>

} } }  // boost::spirit::qi

// ======================================================================

#endif
