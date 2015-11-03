#ifndef fhiclcpp_types_detail_NameStackRegistry_h
#define fhiclcpp_types_detail_NameStackRegistry_h

#include "fhiclcpp/exception.h"

#include <regex>
#include <string>
#include <vector>

namespace fhicl {

  class NameStackRegistry {
  public:

    enum class index_type { NORMAL, PLACEHOLDER };

    std::string full_key(std::string const & key)
    {
      if ( names_.empty() ||
           std::regex_match(key,std::regex( R"(\[(\d+)\])") ) || // Non-table sequence elements
           std::regex_match(key,std::regex( R"(<\d+>)")) )       // Placeholding table sequence elements
        names_.emplace_back(key);
      else
        names_.emplace_back("."+key);
      return full_key_();
    }

    std::string current() {
      return names_.back();
    }

    static void end_of_ctor()
    {
      instance().pop();
    }

    void pop() {
      names_.pop_back();
    }

    static NameStackRegistry& instance(){
      static NameStackRegistry registry;
      return registry;
    }

  private:

    NameStackRegistry() : names_() {}

    std::vector<std::string> names_;

    inline std::string full_key_() {
      return std::accumulate(names_.begin(), names_.end(), std::string{} );
    }
  };

}

#endif

// Local variables:
// mode: c++
// End:
