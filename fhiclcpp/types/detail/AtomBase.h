#ifndef fhiclcpp_detail_AtomBase_h
#define fhiclcpp_detail_AtomBase_h

#include "fhiclcpp/types/detail/ParameterBase.h"

namespace fhicl {

  class ParameterSet;

  namespace detail {

    //========================================================
    class AtomBase : public ParameterBase {
    public:

      AtomBase(Key const & key,
               Comment const & comment,
               bool const hasDefault,
               par_type const type,
               ParameterBase* pb)
        : ParameterBase( key, comment, hasDefault, type, pb ) {}

      std::string stringified_value() const
      {
        return get_stringified_value();
      }

      void set_value(fhicl::ParameterSet const& pset, bool const trimParents)
      {
        do_set_value(pset,trimParents);
      }

    private:

      virtual std::string get_stringified_value() const = 0;
      virtual void do_set_value(fhicl::ParameterSet const&, bool const) = 0;

    };

  }
}

#endif
// Local variables:
// mode: c++
// End:
