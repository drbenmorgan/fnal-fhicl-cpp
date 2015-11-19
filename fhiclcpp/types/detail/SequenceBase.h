#ifndef fhiclcpp_types_detail_SequenceBase_h
#define fhiclcpp_types_detail_SequenceBase_h

#include "cetlib/exempt_ptr.h"
#include "fhiclcpp/types/detail/ParameterBase.h"

#include <array>
#include <tuple>
#include <vector>

namespace fhicl {
  namespace detail {

    //========================================================
    class SequenceBase : public ParameterBase {

      using elements_t = std::vector<cet::exempt_ptr<ParameterBase>>;

    public:

      SequenceBase(Name && name,
                   Comment && comment,
                   value_type const vt,
                   par_type const type,
                   ParameterBase* pb)
        : ParameterBase{name, comment, vt, type, pb}
      {}

      elements_t const& elements() const { return elements_; }

    protected:

      void append_to_elements(cet::exempt_ptr<ParameterBase> pb)
      {
        elements_.push_back(pb);
      }

      void clear_elements()
      {
        elements_.clear();
      }

      template <typename T>
      static void set_elements(T&,
                               ParameterBase&,
                               std::string const& key_prefix,
                               std::size_t const i);

      template <typename T,std::size_t SZ>
      static void set_elements(std::array<T,SZ> & arr,
                               ParameterBase & pb,
                               std::string const& key_prefix,
                               std::size_t const i);

      template <typename T>
      static void set_elements(std::vector<T> & vec,
                               ParameterBase & pb,
                               std::string const& key_prefix,
                               std::size_t const i);

      template <size_t I, typename ... TYPES>
      static
      std::enable_if_t<(I >= std::tuple_size<std::tuple<TYPES...> >::value)>
      set_tuple_element(std::tuple<TYPES...> &,
                        SequenceBase&,
                        std::string const&){}

      template <std::size_t I, typename ... TYPES>
      static
      std::enable_if_t<(I < std::tuple_size<std::tuple<TYPES...> >::value)>
      set_tuple_element(std::tuple<TYPES...> & tuple,
                        SequenceBase & sb,
                        std::string const& key_prefix)
      {
        auto & elem = std::get<I>(tuple);
        set_elements( elem.get_ftype(), elem, key_prefix, I );
        sb.append_to_elements( ptr_to_base(elem) );
        set_tuple_element<I+1>(tuple, sb, key_prefix);
      }

      template <typename ... TYPES>
      static void set_elements(std::tuple<TYPES...> & tuple,
                               ParameterBase & pb,
                               std::string const& key_prefix,
                               std::size_t const i);

    private:

      elements_t elements_;

    };

  }
}

#include "fhiclcpp/types/detail/SequenceBase.icc"

#endif
// Local variables:
// mode: c++
// End:
