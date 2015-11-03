#ifndef fhiclcpp_types_detail_ValidateThenSet_h
#define fhiclcpp_types_detail_ValidateThenSet_h

#include "cetlib/exempt_ptr.h"
#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/types/detail/ParameterBase.h"
#include "fhiclcpp/types/detail/ParameterWalker.h"

#include <set>
#include <string>
#include <vector>

namespace fhicl {
  namespace detail {

    class ValidateThenSet : public ParameterWalker<tt::const_flavor::require_non_const> {
    public:

      ValidateThenSet(fhicl::ParameterSet const& pset,
                            std::set<std::string> const& keysToIgnore)
        : pset_{pset}
        , ignorableKeys_{keysToIgnore}
        , userKeys_{to_set(pset.get_all_keys())}
        , missingParameters_{}
      {}

      void check_keys();

    private:

      bool before_action(ParameterBase* p) override;
      void after_action (ParameterBase* p) override;
      void enter_sequence(SequenceBase* p) override;

      void enter_table(TableBase*) override {}
      void atom(AtomBase*) override {}

      ParameterSet const& pset_;
      std::set<std::string> ignorableKeys_;
      std::set<std::string> userKeys_;
      std::vector<cet::exempt_ptr<ParameterBase>> missingParameters_;

      std::set<std::string> to_set(std::vector<std::string> keys)
      {
        return std::set<std::string>(keys.begin(), keys.end());
      }

    };

  }
}

#endif

// Local variables:
// mode: c++
// End:
