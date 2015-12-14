#ifndef fhiclcpp_types_Name_h
#define fhiclcpp_types_Name_h

#include <regex>
#include <string>

namespace fhicl {

  struct Name {

    explicit Name(std::string const& name = "") : value(name) {}
    std::string value;

    static inline Name sequence_element(std::size_t i)
    {
      return Name{ index(i) };
    }

    static inline Name sequence_element(std::string const& key_stem, std::size_t i)
    {
      return Name{ key_stem + index(i) };
    }

    /*
      Sometimes the key has '[' and ']' literals in it.  Those must be
      padded with an escape character so the regex library doesn't
      think we're trying to specify a character set.
    */
    static std::string regex_safe(std::string const& key)
    {
      return std::regex_replace( key, std::regex("\\[|\\]"), "\\$&" );
    }

  private:

    static inline std::string index(std::size_t i)
    {
      return "[" + std::to_string(i) + "]";
    }


  };

}

#endif

// Local variables:
// mode: c++
// End:
