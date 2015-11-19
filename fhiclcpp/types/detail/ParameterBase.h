#ifndef fhiclcpp_types_detail_ParameterBase_h
#define fhiclcpp_types_detail_ParameterBase_h

/*
  ParameterBase is the most fundamental base class for all fhiclcpp
  types.  The taxonomy is:


          ParameterBase
         /      |      \
        /       |       \____________________
       /        |                            \
  AtomBase   TableBase                  SequenceBase
     |          |                      /     |      \
     |          |                     /      |       \
     |          |        SeqVectorBase       |        \
     |          |              |             |         \
     |          |              |             |          \
  Atom<T>    Table<T>     Sequence<T>   Sequence<T,SZ>   Tuple<T...>


  The design is meant to closely follow the classification of FHiCL
  values, as described in the FHiCL language quick start guide.
*/

#include "fhiclcpp/types/detail/ParameterArgumentTypes.h"
#include "fhiclcpp/types/detail/ParameterMetadata.h"
#include "fhiclcpp/types/detail/ParameterSchemaRegistry.h"

#include <string>

namespace fhicl {

  class ParameterSet;

  namespace detail {

    //========================================================
    class PSRSentry {

      std::string key_;

    public:

      PSRSentry(std::string const& key, ParameterBase* pb)
        : key_(key)
      {
        if (pb) {
          ParameterSchemaRegistry::instance().emplace(pb);
        }
      }

      ~PSRSentry()
      {
        ParameterSchemaRegistry::instance().erase(key_);
      }

    };

    //========================================================
    class ParameterBase {
    public:

      std::string key()            const { return mdata_.key(); }
      std::string name()           const { return mdata_.name(); }
      std::string comment()        const { return mdata_.comment(); }
      bool        has_default()    const { return mdata_.has_default(); }
      bool        is_optional()    const { return mdata_.is_optional(); }
      par_type    parameter_type() const { return mdata_.type(); }

      ParameterBase(Name const & name,
                    Comment const & comment,
                    value_type const vt,
                    par_type const type,
                    ParameterBase* pb)
        : mdata_{name, comment, vt, type}
        , sentry_{mdata_.key(), pb}
      {}

      virtual ~ParameterBase() = default;

      // Modifiers
      void set_value(fhicl::ParameterSet const& ps, bool trimParents)
      {
        do_set_value(ps, trimParents);
      }
      void set_value_type(value_type const vt) { mdata_.set_value_type(vt); }
      void set_key(std::string const& key) { mdata_.set_key(key); }


    protected:
      // ParameterBase& operator=(ParameterBase const&) = default;
      // ParameterBase& operator=(ParameterBase&&) = default;

    private:

      virtual void do_set_value(fhicl::ParameterSet const&, bool trimParents) = 0;

      ParameterMetadata mdata_;
      PSRSentry sentry_;

    };

    //===================================================================
    inline ParameterBase* ptr_to_base(ParameterBase& pb)
    {
      return &pb;
    }

    inline ParameterBase const* ptr_to_base(ParameterBase const& pb)
    {
      return &pb;
    }

  }
}

#endif
// Local variables:
// mode: c++
// End:
