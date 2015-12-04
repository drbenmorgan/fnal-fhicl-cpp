#ifndef fhiclcpp_types_detail_PrintAllowedConfiguration_h
#define fhiclcpp_types_detail_PrintAllowedConfiguration_h

#include "cetlib/exempt_ptr.h"
#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/detail/Indentation.h"
#include "fhiclcpp/types/detail/MaybeDisplayParent.h"
#include "fhiclcpp/types/detail/ParameterBase.h"
#include "fhiclcpp/types/detail/ParameterWalker.h"

#include <stack>
#include <string>
#include <unordered_set>

namespace fhicl {
  namespace detail {

    class PrintAllowedConfiguration : public ParameterWalker<tt::const_flavor::require_const> {
    public:

      PrintAllowedConfiguration(std::ostream& os,
                                bool const showParents = false,
                                std::string const& prefix = std::string(3,' '))
        : buffer_{os}
        , indent_{prefix}
        , mps_{}
        , keysWithCommas_{}
        , keysWithEllipses_{}
        , showParentsForFirstParam_{showParents}
      {}

    private:

      std::ostream& buffer_;
      Indentation indent_;
      std::stack<MaybeDisplayParent> mps_;
      std::unordered_set<std::string> keysWithCommas_;
      std::unordered_set<std::string> keysWithEllipses_;
      bool showParentsForFirstParam_;

      bool before_action(ParameterBase const&) override;
      void after_action(ParameterBase const&) override;

      void enter_table(TableBase const&) override;
      void exit_table (TableBase const&) override;

      void enter_sequence(SequenceBase const&) override;
      void exit_sequence(SequenceBase const&) override;

      void atom(AtomBase const&) override;

    };

  }
}

#endif

// Local variables:
// mode: c++
// End:
