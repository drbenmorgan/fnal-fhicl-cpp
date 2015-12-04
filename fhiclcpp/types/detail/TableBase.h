#ifndef fhiclcpp_detail_TableBase_h
#define fhiclcpp_detail_TableBase_h

#include "cetlib/exempt_ptr.h"
#include "fhiclcpp/types/detail/ParameterBase.h"

namespace fhicl {

  class ParameterSet;

  namespace detail {

    //========================================================
    class TableBase : public ParameterBase {
    public:

      TableBase(Name const & name,
                Comment const & comment,
                value_type const vt)
        : ParameterBase{name, comment, vt, par_type::TABLE}
      {}

      std::vector<cet::exempt_ptr<ParameterBase>> const& members() const
      {
        return get_members();
      }

    private:

      virtual std::vector<cet::exempt_ptr<ParameterBase>> const& get_members() const = 0;

    };

  }
}

#endif
// Local variables:
// mode: c++
// End:
