#ifndef fhiclcpp_types_Table_h
#define fhiclcpp_types_Table_h

#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/types/Comment.h"
#include "fhiclcpp/types/Name.h"
#include "fhiclcpp/types/detail/NameStackRegistry.h"
#include "fhiclcpp/types/detail/ParameterArgumentTypes.h"
#include "fhiclcpp/types/detail/ParameterMetadata.h"
#include "fhiclcpp/types/detail/TableMemberRegistry.h"
#include "fhiclcpp/types/detail/TableBase.h"
#include "fhiclcpp/types/detail/type_traits_error_msgs.h"
#include "fhiclcpp/type_traits.h"

#include <memory>
#include <set>
#include <string>

namespace fhicl {

  //========================================================
  template<typename T>
  class Table :
    public  detail::TableBase,
    private detail::RegisterIfTableMember {
  public:

    static_assert(!tt::is_sequence_type<T>::value , NO_STD_CONTAINERS             );
    static_assert(!tt::is_fhicl_type<T>::value    , NO_NESTED_FHICL_TYPES_IN_TABLE);
    static_assert(!tt::is_table_fragment<T>::value, NO_NESTED_TABLE_FRAGMENTS     );

    //=====================================================
    // User-friendly
    // ... c'tors
    explicit Table(Name&& name);
    explicit Table(Name&& name, Comment&& comment );
    Table(ParameterSet const& pset, std::set<std::string> const & keysToIgnore );

    // ... Accessors
    auto const& operator()() const { return *value_; }

    ParameterSet const & get_PSet() const { return pset_; }

    void validate_ParameterSet(ParameterSet const& pset,
                               std::set<std::string> const & keysToIgnore = {} );

    void print_allowed_configuration(std::ostream& os,
                                     std::string const& tab = std::string(3, ' ') ) const;

    //=====================================================
    // Expert-only
    using dtype = T;
    using rtype = T;

  private:

    using members_t = std::vector<cet::exempt_ptr<ParameterBase>>;

    std::shared_ptr<T> value_ {std::make_shared<T>()};
    ParameterSet pset_ {};
    members_t members_ {detail::TableMemberRegistry::instance().release_members()};

    members_t const& get_members() const override { return members_; }
    void do_set_value(fhicl::ParameterSet const& pset, bool const trimParents) override;

    void maybe_implicitly_default();
  };

}

#include "fhiclcpp/types/detail/Table.icc"

#endif

// Local variables:
// mode: c++
// End:
