#ifndef fhiclcpp_detail_printing_helpers_h
#define fhiclcpp_detail_printing_helpers_h

// ===================================================
//
// Various printing helpers
//
// These helpers are used by the 'Prettifier' classes.
//
// ===================================================

#include "boost/any.hpp"
#include "fhiclcpp/detail/print_mode.h"

#include <string>

namespace fhicl {
  namespace detail {

    inline std::string nl() { return "\n"; }

    std::string printed_suffix(std::string const& key, std::size_t const sz);

    std::string print_parsable_info(std::string const& curr_info);

    std::string print_annotated_info(std::string const& curr_info,
                                     std::string const& cached_info);

    namespace table {
      inline std::string opening_brace() { return "{"; }
      inline std::string closing_brace() { return "}"; }
      std::string printed_prefix(std::string const& key);
    }

    namespace sequence {
      inline std::string opening_brace() { return "["; }
      inline std::string closing_brace() { return "]"; }
      std::string printed_prefix(std::string const& key);
    }

    namespace atom {
      std::string printed_prefix(std::string const& key);
      std::string value(boost::any const&);
    }

  }
}

#endif

// Local variables:
// mode: c++
// End:
