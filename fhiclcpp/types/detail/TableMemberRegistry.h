#ifndef fhiclcpp_types_detail_TableMemberRegistry_h
#define fhiclcpp_types_detail_TableMemberRegistry_h

#include "cetlib/exempt_ptr.h"
#include "fhiclcpp/detail/printing_helpers.h"
#include "fhiclcpp/types/detail/ParameterBase.h"

#include <stack>
#include <vector>

namespace fhicl {

  class ParameterSet;

  namespace detail {

    // All-private class that only
    // fhicl::detail::RegisterIfTableMember, fhicl::Table, and
    // fhicl::OptionalTable can access via friendship.

    class TableMemberRegistry {
    public:

      // Disable copy/move operations
      TableMemberRegistry(TableMemberRegistry const&) = delete;
      TableMemberRegistry(TableMemberRegistry&&) = delete;
      TableMemberRegistry& operator=(TableMemberRegistry const&) = delete;
      TableMemberRegistry& operator=(TableMemberRegistry&&) = delete;

    private:

      TableMemberRegistry() = default;

      using base_ptr        = cet::exempt_ptr<ParameterBase>;
      using table_members_t = std::vector<base_ptr>;
      std::stack< table_members_t > tables_;

      static TableMemberRegistry& instance()
      {
        static TableMemberRegistry registry;
        return registry;
      }

      // Retrieval facilities for fhicl::(Optional)Table

      template <typename T> friend class fhicl::Table;
      template <typename T> friend class fhicl::OptionalTable;

      std::vector<base_ptr> release_members()
      {
        std::vector<base_ptr> result;
        std::swap(tables_.top(), result);
        tables_.pop();
        return result;
      }

      // Registration facilities

      friend class RegisterIfTableMember;
      void emplace_table_member(ParameterBase* pb)
      {
        tables_.top().emplace_back(pb);
      }

      void new_table()
      {
        tables_.emplace();
      }

    };

    //========================================================
    class RegisterIfTableMember {
    public:

      RegisterIfTableMember(ParameterBase* pb)
      {
        if (is_table_member(pb->key())) {
          TableMemberRegistry::instance().emplace_table_member(pb);
        }
        if (is_table(pb->parameter_type())) {
          TableMemberRegistry::instance().new_table();
        }
      }

    };

  }
}

#endif

  // Local variables:
  // mode: c++
  // End:
