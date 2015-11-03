#ifndef fhiclcpp_types_OptionalAtom_h
#define fhiclcpp_types_OptionalAtom_h

#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/types/Comment.h"
#include "fhiclcpp/types/Name.h"
#include "fhiclcpp/types/detail/AtomBase.h"
#include "fhiclcpp/types/detail/NameStackRegistry.h"
#include "fhiclcpp/types/detail/ParameterMetadata.h"
#include "fhiclcpp/types/detail/ParameterSchemaRegistry.h"
#include "fhiclcpp/types/detail/ostream_helpers.h"
#include "fhiclcpp/types/detail/type_traits_error_msgs.h"
#include "fhiclcpp/type_traits.h"

#include <sstream>
#include <string>

namespace fhicl {

  //========================================================
  template<typename T>
  class OptionalAtom final : public detail::AtomBase {
  public:

    static_assert(!tt::is_sequence_type<T>::value , NO_STD_CONTAINERS            );
    static_assert(!tt::is_fhicl_type<T>::value    , NO_NESTED_FHICL_TYPES_IN_ATOM);
    static_assert(!tt::is_table_fragment<T>::value, NO_NESTED_TABLE_FRAGMENTS    );

    //=====================================================
    // User-friendly
    // ... c'tors
    explicit OptionalAtom(Name && name);
    explicit OptionalAtom(Name && name, Comment && cmt );

    // ... Accessors
    bool operator()(T& value) const {
      if (has_value_) {
        value = value_;
        return true;
      }
      return false;
    }

    // Expert-only
    using rtype = T;

    OptionalAtom();

    auto const & get_ftype() const { return value_; }
    auto       & get_ftype()       { return value_; }

  private:
    T value_ {};
    bool has_value_ {false};

    std::string get_stringified_value() const override;
    void do_set_value( fhicl::ParameterSet const &, bool const ) override;
  };

}

#include "fhiclcpp/types/detail/OptionalAtom.icc"

#endif

// Local variables:
// mode: c++
// End: