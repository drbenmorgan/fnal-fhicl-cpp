#ifndef fhiclcpp_types_detail_print_allowed_configuration_h
#define fhiclcpp_types_detail_print_allowed_configuration_h

#include "cetlib/exempt_ptr.h"

#include <ostream>
#include <string>

namespace fhicl {

  namespace detail {

    class ParameterBase;

    using base_ptr = cet::exempt_ptr<ParameterBase>;

    void print_allowed_configuration( std::string const& key, std::ostream & os,
                                      bool const showParents = false,
                                      std::string const & prefix = std::string(3,' ') );

    void stringify(base_ptr parameter, std::ostream & os,
                   bool const showParents = false,
                   std::string const & prefix = "",
                   std::string const & suffix = "" );

    void stringify_array (base_ptr parameter, std::ostream & os, bool const showParents, std::string const& prefix);
    void stringify_atom  (base_ptr parameter, std::ostream & os, bool const showParents, std::string const& prefix);
    void stringify_tuple (base_ptr parameter, std::ostream & os, bool const showParents, std::string const& prefix);
    void stringify_table (base_ptr parameter, std::ostream & os, bool const showParents, std::string const& prefix);
    void stringify_vector(base_ptr parameter, std::ostream & os, bool const showParents, std::string const& prefix);

  }

}

#endif

// Local variables:
// mode: c++
// End:
