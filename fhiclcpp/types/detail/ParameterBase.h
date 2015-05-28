#ifndef fhiclcpp_types_detail_ParameterBase_h
#define fhiclcpp_types_detail_ParameterBase_h

#include "fhiclcpp/types/detail/ParameterArgumentTypes.h"
#include "fhiclcpp/types/detail/ParameterMetadata.h"

#include <string>

namespace fhicl {

  class ParameterSet;

  namespace detail {

    //========================================================
    class ParameterBase{
    public:

      virtual ~ParameterBase();

      std::string key()            const { return mdata_.key(); }
      std::string comment()        const { return mdata_.comment(); }
      bool        has_default()    const { return mdata_.has_default(); }
      par_type    parameter_type() const { return mdata_.type(); }

      ParameterBase(Key const & key,
                    Comment const & comment,
                    bool const hasDefault,
                    par_type const type,
                    ParameterBase* pb);

      // Modifiers
      void set_default_flag(bool const flag) { mdata_.set_default_flag(flag); }
      void set_key(std::string const& key) { mdata_.set_key(key); }

    protected:
      // ParameterBase& operator=(ParameterBase const&) = default;
      // ParameterBase& operator=(ParameterBase&&) = default;

    private:

      detail::ParameterMetadata mdata_;

    };

  }
}

#endif
// Local variables:
// mode: c++
// End:
