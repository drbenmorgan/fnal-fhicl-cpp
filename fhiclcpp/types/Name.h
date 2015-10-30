#ifndef fhiclcpp_types_Name_h
#define fhiclcpp_types_Name_h

#include <regex>
#include <string>

namespace fhicl {

  struct Name {

    explicit Name(std::string const& name = "") : value(name) {}
    std::string value;

    enum class bracket { SQUARE, ANGLE, NTYPES };

    static Name anonymous()
    {
      static std::size_t i{};
      return Name( array_index(i++, bracket::ANGLE) );
    }

    static inline std::string array_index(std::size_t i, bracket const bkt = bracket::SQUARE)
    {
      std::string const prefix = bkt == bracket::SQUARE ? std::string("[") : std::string("<");
      std::string const suffix = bkt == bracket::SQUARE ? std::string("]") : std::string(">");
      std::string const index  = prefix + std::to_string(i) + suffix;
      return index;
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

  };

}

#endif

// Local variables:
// mode: c++
// End:
