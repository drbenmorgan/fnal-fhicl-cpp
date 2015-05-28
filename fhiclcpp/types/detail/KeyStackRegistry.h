#ifndef fhiclcpp_types_detail_KeyStackRegistry_h
#define fhiclcpp_types_detail_KeyStackRegistry_h

#include "fhiclcpp/exception.h"

#include <regex>
#include <string>
#include <vector>

namespace fhicl {

  class KeyStackRegistry {
  public:

    enum class index_type { NORMAL, PLACEHOLDER };

    std::string qualified_name(std::string const & key)
    {
      if ( keys_.empty() ||
           std::regex_match(key,std::regex( R"(\[(\d+)\])") ) || // Non-table sequence elements
           std::regex_match(key,std::regex( R"(<\d+>)")) )       // Placeholding table sequence elements
        keys_.push_back(key);
      else
        keys_.push_back(std::string(".")+key);
      return print_qualified_name();
    }

    std::string current() {
      return keys_.back();
    }

    void pop_back() {
      keys_.pop_back();
    }

    static KeyStackRegistry& instance(){
      static KeyStackRegistry registry;
      return registry;
    }

  private:

    KeyStackRegistry() : keys_() {}

    std::vector<std::string> keys_;

    std::string print_qualified_name() {
      std::string value;
      for ( const auto& key : keys_ ) value += key;
      return value;
    }
  };

}

#endif

// Local variables:
// mode: c++
// End:
