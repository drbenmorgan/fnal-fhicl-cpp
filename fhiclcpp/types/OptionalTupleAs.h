#ifndef fhiclcpp_types_OptionalTupleAs_h
#define fhiclcpp_types_OptionalTupleAs_h

#include "fhiclcpp/types/OptionalTuple.h"
#include "fhiclcpp/types/detail/NameStackRegistry.h"
#include "fhiclcpp/types/detail/ParameterBase.h"
#include "fhiclcpp/types/detail/type_traits_error_msgs.h"
#include "fhiclcpp/type_traits.h"

#include <memory>
#include <string>
#include <utility>

namespace fhicl {

  //==================================================================
  // e.g. OptionalTupleAs<T,int,double,bool> ====> T(int,double,bool)
  //
  template<typename T, typename ... ARGS>
  class OptionalTupleAs;


  template<typename T, typename ... ARGS>
  class OptionalTupleAs<T(ARGS...)> {
  public:

    explicit OptionalTupleAs(Name&& name,
                             Comment&& cmt)
      : tupleObj_{std::move(name), conversion_comment(std::move(cmt))}
    {}

    explicit OptionalTupleAs(Name&& name) : OptionalTupleAs(std::move(name), Comment("") ) {}

    template<std::size_t ...I>
    T fill(typename OptionalTuple<ARGS...>::rtype const& via,
           std::index_sequence<I...>) const
    {
      return T{std::get<I>(via)...};
    }

    bool operator()(T& result) const {
      typename OptionalTuple<ARGS...>::rtype const& via;
      if ( tupleObj_(via) ) {
        auto tmp = fill(via, std::index_sequence_for<ARGS...>{});
        std::swap(result, tmp);
        return true;
      }
      return false;
    }

  private:

    OptionalTuple<ARGS...> tupleObj_;

    Comment conversion_comment(Comment&& cmt) const
    {
      std::string const preface  = "N.B. The following sequence is converted to type:";
      std::string const name     = "        '"+cet::demangle(typeid(T).name()) +"'";
      std::string const user_comment = cmt.value.empty() ? "" : "\n\n"+cmt.value;

      std::ostringstream oss;
      oss << preface << '\n'
          << name
          << user_comment;

      return Comment{ oss.str().c_str() };
    }

  };
}

#endif

// Local variables:
// mode: c++
// End:
