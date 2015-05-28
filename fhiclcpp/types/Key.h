#ifndef fhiclcpp_Key_h
#define fhiclcpp_Key_h

#include <regex>
#include <string>

namespace fhicl {

  struct Key {

    explicit Key(std::string const& key = "") : value(key) {}
    std::string value;

    enum class bracket  { SQUARE, ANGLE, NTYPES };

    static inline std::string array_index(std::size_t i, bracket const bkt = bracket::SQUARE)
    {
      std::string const prefix = bkt == bracket::SQUARE ? std::string("[") : std::string("<");
      std::string const suffix = bkt == bracket::SQUARE ? std::string("]") : std::string(">");
      std::string const index  = prefix + std::to_string(i) + suffix;
      return index;
    }

    static Key anonymous()
    {
      static std::size_t i{};
      return Key( array_index(i++, bracket::ANGLE) );
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

    static inline Key append_to (std::string const & prefix,
                                 std::string const & suffix )
    {
      return Key( prefix + std::string(".") + suffix );
    }

  };

}

#endif

// Local variables:
// mode: c++
// End:
