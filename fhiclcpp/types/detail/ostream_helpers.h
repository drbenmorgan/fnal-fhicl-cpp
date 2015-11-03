#ifndef fhiclcpp_types_detail_ostream_helpers_h
#define fhiclcpp_types_detail_ostream_helpers_h

#include "cetlib/demangle.h"

#include <ostream>
#include <regex>
#include <string>

namespace fhicl {
  namespace detail {
    namespace yes_defaults {

      //=================================================================
      // std::string
      //
      // When printing out strings, it is best to surround them with
      // \"...\" so that there will be no ambiguity for the user.
      template <typename T>
      struct maybe_quotes{
        maybe_quotes(T const & t) : value(t) {}
        T const & value;
      };

      template<>
      struct maybe_quotes<std::string>{
        maybe_quotes(std::string const & t) : value(std::string("\"")+t+std::string("\"") ) {}
        std::string value;
      };

      template <typename T>
      std::ostream& operator<<(std::ostream& os, maybe_quotes<T>&& mq)
      {
        return os << std::boolalpha << mq.value;
      }

    } } } // fhicl::detail::yes_defaults

//====================================================================================
//                                                                                   =
//====================================================================================

namespace fhicl {
  namespace detail {
    namespace no_defaults {

      inline std::string stripped_typename( std::string const& fullName )
      {
        // Remove 'std::' from fullName
        std::string const strippedName = std::regex_replace(fullName, std::regex("std::"), "" );
        return strippedName;
      }

      inline std::string padded_string( std::string const& tnToPad )
      {
        return std::string("<")+tnToPad+std::string(">");
      }

      inline std::string presented_string( std::string const& fullName ) {
        return padded_string( stripped_typename( fullName ) );
      }

      template <typename T>
      struct expected_types {
        expected_types() : value("") {
          std::string const strippedTypename = stripped_typename( cet::demangle_symbol(typeid(T).name() ) );
          value = padded_string( strippedTypename );
        }
        std::string value;
      };


      template <typename T>
      std::ostream& operator<<(std::ostream& os, expected_types<T>&& et)
      {
        return os << et.value;
      }

    } } } // fhicl::detail::no_defaults

#endif

// Local variables:
// mode : c++
// End: