#ifndef fhiclcpp_detail_AtomBase_h
#define fhiclcpp_detail_AtomBase_h

#include "fhiclcpp/types/detail/ParameterBase.h"

namespace fhicl {

  class ParameterSet;

  namespace detail {

    //========================================================
    class AtomBase : public ParameterBase {
    public:

      AtomBase(Name const & name,
               Comment const & comment,
               value_type const vt,
               ParameterBase* pb)
        : ParameterBase{name, comment, vt, par_type::ATOM, pb}
      {}

      std::string stringified_value() const
      {
        return get_stringified_value();
      }

    private:

      virtual std::string get_stringified_value() const = 0;

    };

  }
}

#endif
// Local variables:
// mode: c++
// End:
