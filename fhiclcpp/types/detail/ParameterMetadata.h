#ifndef fhiclcpp_types_detail_ParameterMetadata_h
#define fhiclcpp_types_detail_ParameterMetadata_h

#include "fhiclcpp/types/Comment.h"
#include "fhiclcpp/types/Key.h"
#include "fhiclcpp/types/detail/ParameterArgumentTypes.h"
#include "fhiclcpp/types/detail/KeyStackRegistry.h"

#include <string>

namespace fhicl {
  namespace detail {

    class ParameterBase;

    class ParameterMetadata {
    public:

      ParameterMetadata()
        : key_()
        , comment_()
        , hasDefault_(false)
        , type_(par_type::NTYPES)
      {
      }

      ParameterMetadata(Key const& key = Key(),
                        Comment const& cmt = Comment(""),
                        bool const hasDefault = false,
                        par_type const parType = par_type::NTYPES)
        : key_( KeyStackRegistry::instance().qualified_name(key.value) )
        , comment_(cmt.value)
        , hasDefault_(hasDefault)
        , type_(parType)
      {}

      std::string key()         const { return key_; }
      std::string comment()     const { return comment_;}
      bool        has_default() const { return hasDefault_; }
      par_type    type()        const { return type_; }

      void set_key( std::string const& key ) { key_ = key; }
      void set_default_flag( bool const flag ) { hasDefault_ = flag; }

    private:
      std::string key_;
      std::string comment_;
      bool hasDefault_;
      par_type type_;

    };

  }
}

#endif

  // Local variables:
  // mode: c++
  // End:
