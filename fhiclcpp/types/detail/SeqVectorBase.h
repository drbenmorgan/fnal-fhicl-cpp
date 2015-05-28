#ifndef fhiclcpp_types_detail_SeqVectorBase_h
#define fhiclcpp_types_detail_SeqVectorBase_h

#include "fhiclcpp/types/detail/ParameterBase.h"

namespace fhicl {
  namespace detail {

    //========================================================
    class SeqVectorBase : public ParameterBase {
    public:

      SeqVectorBase(Key && key,
                    Comment && comment,
                    bool const hasDefault,
                    par_type const type,
                    ParameterBase* pb)
        : ParameterBase( key, comment, hasDefault, type, pb ) {}

      void resize_sequence(std::size_t const sz)
      {
        do_resize_sequence(sz);
      }

    private:

      virtual void do_resize_sequence(std::size_t const) = 0;

    };

  }
}

#endif
// Local variables:
// mode: c++
// End:
