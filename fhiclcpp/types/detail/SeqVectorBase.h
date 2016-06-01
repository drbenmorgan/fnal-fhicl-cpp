#ifndef fhiclcpp_types_detail_SeqVectorBase_h
#define fhiclcpp_types_detail_SeqVectorBase_h

#include "fhiclcpp/types/detail/SequenceBase.h"

namespace fhicl {
  namespace detail {

    //========================================================
    class SeqVectorBase : public SequenceBase {
    public:

      SeqVectorBase(Name&& name,
                    Comment&& comment,
                    value_type const vt,
                    std::function<bool()> maybeUse)
        : SequenceBase{std::move(name), std::move(comment), vt, par_type::SEQ_VECTOR, maybeUse}
      {}

      void resize_sequence(std::size_t const sz)
      {
        do_resize_sequence(sz);
      }

    private:

      virtual void do_resize_sequence(std::size_t const) = 0;

    };

  }
}

#endif /* fhiclcpp_types_detail_SeqVectorBase_h */

// Local variables:
// mode: c++
// End:
