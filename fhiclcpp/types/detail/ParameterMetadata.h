#ifndef fhiclcpp_types_detail_ParameterMetadata_h
#define fhiclcpp_types_detail_ParameterMetadata_h

#include "fhiclcpp/types/Comment.h"
#include "fhiclcpp/types/Name.h"
#include "fhiclcpp/types/detail/ParameterArgumentTypes.h"
#include "fhiclcpp/types/detail/NameStackRegistry.h"

#include <string>

namespace fhicl {
  namespace detail {

    class ParameterBase;

    class ParameterMetadata {
    public:

      ParameterMetadata() = default;

      ParameterMetadata(Name const& name = Name(),
                        Comment const& comment = Comment(""),
                        value_type const valType = value_type::NTYPES,
                        par_type const parType = par_type::NTYPES)
        : key_{NameStackRegistry::instance().full_key(name.value)}
        , name_{name.value}
        , comment_{comment.value}
        , valType_{valType}
        , parType_{parType}
      {}

      std::string key()         const { return key_; }
      std::string name()        const { return name_; }
      std::string comment()     const { return comment_;}
      bool        has_default() const { return valType_ == value_type::DEFAULT; }
      bool        is_optional() const { return valType_ == value_type::OPTIONAL; }
      par_type    type()        const { return parType_; }

      void set_key( std::string const& key ) {
        key_  = key;
        name_ = key.substr( key.find_last_of(".")+1 );
      }
      void set_value_type( value_type const vt ) { valType_ = vt; }

    private:
      std::string key_ {};
      std::string name_ {};
      std::string comment_ {};
      value_type  valType_ { value_type::NTYPES };
      par_type    parType_ { par_type::NTYPES };

    };

  }
}

#endif

  // Local variables:
  // mode: c++
  // End:
