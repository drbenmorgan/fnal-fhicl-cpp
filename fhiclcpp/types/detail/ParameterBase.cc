#include "fhiclcpp/types/detail/ParameterBase.h"
#include "fhiclcpp/types/detail/ParameterReferenceRegistry.h"

namespace fhicl {
  namespace detail {

    ParameterBase::ParameterBase(Name const & name,
                                 Comment const & comment,
                                 bool const hasDefault,
                                 par_type const type,
                                 ParameterBase* pb)
      : mdata_(name,comment,hasDefault,type)
    {
      if ( pb ) ParameterReferenceRegistry::instance().emplace( mdata_.key(), pb );
    }

    ParameterBase::~ParameterBase()
    {
      ParameterReferenceRegistry::instance().erase( mdata_.key() );
    }

  }
}

// Local variables:
// mode: c++
// End:
