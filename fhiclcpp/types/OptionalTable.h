#ifndef fhiclcpp_types_OptionalTable_h
#define fhiclcpp_types_OptionalTable_h

#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/types/Comment.h"
#include "fhiclcpp/types/Name.h"
#include "fhiclcpp/types/detail/NameStackRegistry.h"
#include "fhiclcpp/types/detail/ParameterArgumentTypes.h"
#include "fhiclcpp/types/detail/ParameterMetadata.h"
#include "fhiclcpp/types/detail/ParameterSchemaRegistry.h"
#include "fhiclcpp/types/detail/TableBase.h"
#include "fhiclcpp/types/detail/type_traits_error_msgs.h"
#include "fhiclcpp/type_traits.h"

#include <string>

namespace fhicl {

  //========================================================
  template<typename T>
  class OptionalTable : public detail::TableBase {
  public:

    static_assert(!tt::is_sequence_type<T>::value , NO_STD_CONTAINERS             );
    static_assert(!tt::is_fhicl_type<T>::value    , NO_NESTED_FHICL_TYPES_IN_TABLE);
    static_assert(!tt::is_table_fragment<T>::value, NO_NESTED_TABLE_FRAGMENTS     );

    //=====================================================
    // User-friendly
    // ... c'tors
    explicit OptionalTable(Name && name);
    explicit OptionalTable(Name && name, Comment && comment );
    OptionalTable(ParameterSet const& pset, std::set<std::string> const & keysToIgnore );

    // ... Accessors
    bool operator()(T& value) const {
      if(has_value_) {
        value = value_;
        return true;
      }
      return false;
    }

    ParameterSet const & get_PSet() const { return pset_; }

    void validate_ParameterSet(ParameterSet const& pset,
                               std::set<std::string> const & keysToIgnore = {} );

    void print_allowed_configuration(std::ostream& os,
                                     std::string const& tab = std::string(3, ' ') ) const;

    //=====================================================
    // Expert-only
    using rtype = T;

    OptionalTable();

    auto const & get_ftype() const { return value_; }
    auto       & get_ftype()       { return value_; }

  private:

    using members_t = std::vector<cet::exempt_ptr<ParameterBase>>;

    T value_ {};
    bool has_value_ {false};
    ParameterSet pset_ {};
    members_t members_ {};

    members_t const& get_members() const override { return members_; }
    void do_set_value(fhicl::ParameterSet const& pset, bool const trimParents) override;

    void fill_members();

  };

}

#include "fhiclcpp/types/detail/OptionalTable.icc"

#endif

// Local variables:
// mode: c++
// End:
