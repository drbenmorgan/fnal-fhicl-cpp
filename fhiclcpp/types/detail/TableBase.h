#ifndef fhiclcpp_detail_TableBase_h
#define fhiclcpp_detail_TableBase_h

#include "fhiclcpp/types/detail/ParameterBase.h"

namespace fhicl {

  class ParameterSet;

  namespace detail {

    //========================================================
    class TableBase : public ParameterBase {
    public:

      TableBase(Name const & name,
                Comment const & comment,
                ParameterBase* pb)
        : ParameterBase( name, comment, false, par_type::TABLE, pb ) {}

      void set_value(fhicl::ParameterSet const& pset, bool const trimParents)
      {
        do_set_value(pset,trimParents);
      }

    private:

      virtual void do_set_value(fhicl::ParameterSet const&, bool const trimParents) = 0;

    };

  }
}

#endif
// Local variables:
// mode: c++
// End:
