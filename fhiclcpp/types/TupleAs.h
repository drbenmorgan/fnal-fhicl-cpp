#ifndef fhiclcpp_types_TupleAs_h
#define fhiclcpp_types_TupleAs_h

#include "fhiclcpp/types/OptionalTuple.h"
#include "fhiclcpp/types/Tuple.h"
#include "fhiclcpp/types/detail/NameStackRegistry.h"
#include "fhiclcpp/types/detail/ParameterBase.h"
#include "fhiclcpp/types/detail/type_traits_error_msgs.h"
#include "fhiclcpp/type_traits.h"

#include <memory>
#include <string>
#include <utility>

namespace fhicl {


  //==================================================================
  // e.g. TupleAs<T,int,double,bool> ====> T(int,double,bool)
  //
  template<typename T, typename ... ARGS>
  class TupleAs;


  template<typename T, typename ... ARGS>
  class TupleAs<T(ARGS...)> {
  public:

    using via_type = typename OptionalTuple<ARGS...>::rtype;
    using rtype    = T;
    using ftype    = typename OptionalTuple<ARGS...>::ftype;

    explicit TupleAs(Name&& name,
                     Comment&& cmt)
      : tupleObj_{std::move(name), conversion_comment(std::move(cmt))}
    {
      tupleObj_.set_value_type(value_type::REQUIRED);
    }

    explicit TupleAs(Name&& name,
                     Comment&& cmt,
                     T const& t)
      : tupleObj_{std::move(name), conversion_comment(std::move(cmt),t)}
      , t_{std::make_shared<T>(t)}
    {
      tupleObj_.set_value_type(value_type::DEFAULT);
    }

    explicit TupleAs(Name&& name) : TupleAs{std::move(name), Comment("")} {}
    explicit TupleAs(Name&& name, T const& t) : TupleAs{std::move(name), Comment(""), t} {}
    explicit TupleAs(Name&& name, T const& t, Comment&& cmt) : TupleAs{std::move(name), std::move(cmt), t} {}

    template<std::size_t ...I>
    T fill(via_type const& via, std::index_sequence<I...>) const
    {
      return T{std::get<I>(via)...};
    }

    T operator()() const {
      via_type via;
      return tupleObj_(via) ? fill(via, std::index_sequence_for<ARGS...>{}) : tupleObj_.has_default()
        ? *t_ : throw fhicl::exception(cant_find); // fix this exception category!
    }

    //=================================================================
    // expert only

    // The following functions are necessary for the 'set_elements'
    // call if a user ever does Sequence< TupleAs<> >
    TupleAs() : TupleAs{Name::anonymous()} {}

    //    TupleAs(T const& t) : tupleObj_{}, t_{std::make_shared<T>(t)} {}

    auto & get_ftype() { return tupleObj_.get_ftype(); }
    detail::ParameterBase * ptr_to_base() { return &tupleObj_; }

    operator detail::ParameterBase&() { return tupleObj_; } // Allows implicit conversion from TupleAs to ParameterBase

  private:
    OptionalTuple<ARGS...> tupleObj_;
    std::shared_ptr<T> t_ {}; // shared instead of unique to allowed Sequence<TupleAs<>> objects.

    Comment conversion_comment(Comment&& cmt) const;
    Comment conversion_comment(Comment&& cmt, T const& t) const;

  };

  //==================================================================
  // IMPLEMENTATION

  template <typename T, typename ... ARGS>
  Comment TupleAs<T(ARGS...)>::conversion_comment(Comment&& cmt) const
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


  //=================================================================
  // metaprogramming necessary for determining if provided type 'T'
  // has an 'std::ostream& operator<<(std::ostream&, T const&)' defined

  template <typename T, typename ... ARGS>
  detail::ParameterBase* ptr_to_base(TupleAs<T(ARGS...)> & ta)
  {
    return ta.ptr_to_base();
  }

  namespace has_insertion_operator_impl {
    typedef char no;
    typedef char yes[2];

    struct any_t {
      template<typename T> any_t( T const& );
    };

    no operator<<( std::ostream const&, any_t const& );

    yes& test( std::ostream& );
    no test( no );

    template<typename T>
    struct has_insertion_operator {
      static std::ostream &s;
      static T const &t;
      static bool const value = sizeof( test(s << t) ) == sizeof( yes );
    };
  }

  template<typename T>
  struct has_insertion_operator :
    has_insertion_operator_impl::has_insertion_operator<T> {
  };

  struct NoInsert {
    template <typename T>
    std::string operator()(T const&) {
      return
        "     A default value is present, but it cannot be\n"
        "     printed out since no 'operator<<' overload has\n"
        "     been provided for the above type."; }
  };

  struct YesInsert {
    template <typename T>
    std::string operator()(T const& t)
    {
      std::ostringstream os;
      os << "     with a default value of:\n"
         << "        " << t;
      return os.str();
    }
  };

  //===============================================================================

  template <typename T, typename ... ARGS>
  Comment TupleAs<T(ARGS...)>::conversion_comment(Comment&& cmt, T const& t) const
  {
    std::string const preface  = "N.B. The following sequence is converted to type:";
    std::string const name     = "        '"+cet::demangle(typeid(T).name()) +"'";

    std::conditional_t< has_insertion_operator<T>::value, YesInsert, NoInsert>
      stringified_default;

    std::string const user_comment = cmt.value.empty() ? "" : "\n\n"+cmt.value;

    std::ostringstream oss;
    oss << preface << '\n'
        << name << '\n'
        << stringified_default(t)
        << user_comment;
    return Comment{ oss.str().c_str() };
  }

}

#endif

// Local variables:
// mode: c++
// End:
