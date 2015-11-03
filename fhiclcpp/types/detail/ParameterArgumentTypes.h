#ifndef fhiclcpp_types_detail_ParameterArgumentTypes_h
#define fhiclcpp_types_detail_ParameterArgumentTypes_h

#include <string>

namespace fhicl {

  enum class par_type { ATOM, TABLE, SEQ_VECTOR, SEQ_ARRAY, TUPLE, NTYPES };

  inline bool is_table( par_type pt ) { return pt == par_type::TABLE; }
  inline bool is_sequence( par_type pt ){
    return
      pt == par_type::SEQ_VECTOR ||
      pt == par_type::SEQ_ARRAY ||
      pt == par_type::TUPLE;
  }


}

#endif

// Local variables:
// mode: c++
// End: