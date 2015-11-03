#ifndef fhiclcpp_define_get_names_h
#define fhiclcpp_define_get_names_h

#include "boost/algorithm/string.hpp"
#include "fhiclcpp/exception.h"

#include <algorithm>
#include <string>
#include <vector>

namespace fhicl {
  namespace detail {

    class Keys {
    public:

      Keys(std::vector<std::string> const& keys,
           std::string const& last)
        : tables_{keys}
        , last_{last}
      {}

      auto const& tables() const { return tables_; }
      auto const& last()   const { return last_; }

    private:
      std::vector<std::string> tables_;
      std::string last_;
    };

    inline Keys get_names(std::string const& key)
    {
      std::vector<std::string> keys;
      boost::algorithm::split(keys, key, boost::algorithm::is_any_of(".") );

      // Remove empty keys
      keys.erase( std::remove( keys.begin(), keys.end(), "" ), keys.end() );

      if (keys.empty())
        throw fhicl::exception(cant_find, "vacuous key");

      std::string const last = keys.back();
      keys.pop_back();

      return Keys{keys, last};
    }

  }
}

#endif

// Local variables:
// mode: c++
// End:
