#ifndef fhiclcpp_Atom_h
#define fhiclcpp_Atom_h

#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/types/Comment.h"
#include "fhiclcpp/types/Name.h"
#include "fhiclcpp/types/detail/AtomBase.h"
#include "fhiclcpp/types/detail/NameStackRegistry.h"
#include "fhiclcpp/types/detail/ParameterMetadata.h"
#include "fhiclcpp/types/detail/ParameterReferenceRegistry.h"
#include "fhiclcpp/types/detail/ostream_helpers.h"
#include "fhiclcpp/types/detail/type_traits_error_msgs.h"
#include "fhiclcpp/type_traits.h"

#include <sstream>
#include <string>

namespace fhicl {

  //========================================================
  template<typename T>
  class Atom final : public detail::AtomBase {
  public:

    //=====================================================
    // User-friendly
    // ... c'tors
    explicit Atom(Name && name);

    explicit Atom(Name && name, Comment && cmt );
    explicit Atom(Name && name, T const& dflt_value );

    explicit Atom(Name && name, Comment && cmt, T const& dflt_value );
    explicit Atom(Name && name, T const& dflt_value, Comment && cmt );

    // ... Accessors
    auto const & operator()() const { return value_; }

    // Expert-only
    using rtype = T;

    Atom();

    auto const & get_ftype() const { return value_; }
    auto       & get_ftype()       { return value_; }

  private:
    T value_;

    std::string get_stringified_value() const override;
    void do_set_value( fhicl::ParameterSet const &, bool const ) override;

  };

}

#include "fhiclcpp/types/detail/Atom.icc"

#endif

// Local variables:
// mode: c++
// End:
