#ifndef FHICLCPP_INTERMEDIATE_TABLE_H
#define FHICLCPP_INTERMEDIATE_TABLE_H

////////////////////////////////////////////////////////////////////////
// intermediate table: generic representation of parameters.
//
// This class should only be used in the following two circumstances:
//
// 1. Configuration post-processing, prior to ParameterSet creation
//    (use the "simple interface" where possible).
//
// 2. Internally by FHiCL.
//
// In all other circumstances, ParameterSet is almost certainly more
// appropriate.
//
// 2012/08/29 CG.
////////////////////////////////////////////////////////////////////////

#include "boost/any.hpp"
#include "cpp0x/string"
#include "fhiclcpp/coding.h"
#include "fhiclcpp/extended_value.h"
#include "fhiclcpp/type_traits.h"
#include "fhiclcpp/fwd.h"
#include <complex>
#include <map>
#include <vector>

// ----------------------------------------------------------------------

class fhicl::intermediate_table {
public:
  // Constructor
  intermediate_table();

  ////////////////////
  // Simple interface:
  bool empty() const;

  bool exists(std::string const & name) const;

  void erase(std::string const & name);

  template <typename T>
  T get(std::string const & name);

  void put(std::string const & name,
           std::string const & value, // String.
           bool in_prolog = false);
  void put(std::string const & name,
           char const * value, // String.
           bool in_prolog = false);
  void put(std::string const & name,
           bool value, // Boolean.
           bool in_prolog = false);
  template <typename T>
  void put(std::string const & name,
           std::complex<T> const & value, // Complex.
           bool in_prolog = false);
  template <typename T>
  void put(std::string const & name,
           std::vector<T> const & value, // Sequence.
           bool in_prolog = false);
  template <typename T>
  typename std::enable_if<tt::is_numeric<T>::value, void>::type
  put(std::string const & name,
      T value, // Number
      bool in_prolog = false);

  void putEmptySequence(std::string const & name,
                        bool in_prolog = false); // Empty Sequence.
  void putEmptyTable(std::string const & name,
                     bool in_prolog = false); // Empty Table.
  void putNil(std::string const & name,
              bool in_prolog = false); // Nil.

  ////////////////////
  // Expert interface.
  // Typedefs.
  typedef extended_value::atom_t      atom_t;
  typedef extended_value::complex_t   complex_t;
  typedef extended_value::sequence_t  sequence_t;
  typedef extended_value::table_t     table_t;
  typedef table_t::iterator iterator;
  typedef table_t::const_iterator const_iterator;

  const_iterator  begin() const;
  const_iterator  end() const;

  // Flexible insert interface.
  void  insert(std::string const & name
               , bool                in_prolog
               , value_tag           tag
               , boost::any const  & value
              );
  void  insert(std::string    const & name
               , extended_value const & value
              );

  extended_value  & operator [](std::string const & name);
  extended_value const  & find(std::string const & name) const;

private:
  extended_value  ex_val;

  std::vector<std::string>  split(std::string const & name) const;

};  // intermediate_table

namespace fhicl {
  namespace detail {
    template <typename T>
    typename tt::disable_if<std::is_reference<T>::value || std::is_pointer<T>::value, T>::type
    it_value_get(intermediate_table & table,
                 std::string const & name)
    {
      T result;
      detail::decode(table[name].value, result);
      return result;
    }

    template <>
    typename tt::disable_if<std::is_reference<intermediate_table::sequence_t>::value ||
                            std::is_pointer<intermediate_table::sequence_t>::value,
                            intermediate_table::sequence_t>::type
    it_value_get<intermediate_table::sequence_t>(intermediate_table & table,
                                                 std::string const & name) = delete;

    template <>
    typename tt::disable_if<std::is_reference<intermediate_table::table_t>::value ||
                            std::is_pointer<intermediate_table::table_t>::value,
                            intermediate_table::table_t>::type
    it_value_get<intermediate_table::table_t>(intermediate_table & table,
                                              std::string const & name) = delete;

    template <typename T>
    typename tt::enable_if<std::is_reference<T>::value ||
                           std::is_pointer<T>::value, T>::type
    it_value_get(intermediate_table & table,
                 std::string const & name) = delete;

    template <>
    inline
    typename tt::enable_if<std::is_reference<intermediate_table::sequence_t &>::value,
                           intermediate_table::sequence_t &>::type
    it_value_get<intermediate_table::sequence_t &> (intermediate_table & table,
                                                    std::string const & name)
    {
      return boost::any_cast<intermediate_table::sequence_t &>(table[name].value);
    }

    template <>
    inline
    typename tt::enable_if<std::is_reference<intermediate_table::table_t &>::value,
                           intermediate_table::table_t &>::type
    it_value_get<intermediate_table::table_t &> (intermediate_table & table,
                                                 std::string const & name)
    {
      return boost::any_cast<intermediate_table::table_t &>(table[name].value);
    }

    template <>
    inline
    typename tt::enable_if<std::is_reference<intermediate_table::sequence_t const &>::value,
                           intermediate_table::sequence_t const &>::type
    it_value_get<intermediate_table::sequence_t const &> (intermediate_table & table,
                                                    std::string const & name)
    {
      return boost::any_cast<intermediate_table::sequence_t const &>(table[name].value);
    }

    template <>
    inline
    typename tt::enable_if<std::is_reference<intermediate_table::table_t const &>::value,
                           intermediate_table::table_t const &>::type
    it_value_get<intermediate_table::table_t const &> (intermediate_table & table,
                                                 std::string const & name)
    {
      return boost::any_cast<intermediate_table::table_t const &>(table[name].value);
    }
  }
}

template <typename T>
inline
T
fhicl::intermediate_table::
get(std::string const & name)
{
  return detail::it_value_get<T>(* this, name);
}

inline
void
fhicl::intermediate_table::
put(std::string const & name,
       std::string const & value, // String.
       bool in_prolog)
{
  insert(name, in_prolog, STRING, detail::encode(value));
}

inline
void
fhicl::intermediate_table::
put(std::string const & name,
       char const * value, // String.
       bool in_prolog)
{
  insert(name, in_prolog, STRING, detail::encode(value));
}

inline
void
fhicl::intermediate_table::
put(std::string const & name,
       bool value, // Boolean.
       bool in_prolog)
{
  insert(name, in_prolog, BOOL, detail::encode(value));
}

template <typename T>
void
fhicl::intermediate_table::
put(std::string const & name,
       std::complex<T> const & value, // Complex.
       bool in_prolog)
{
  insert(name, in_prolog, COMPLEX, detail::encode(value));
}

template <typename T>
inline
void
fhicl::intermediate_table::
put(std::string const & name,
       std::vector<T> const & value, // Sequence.
       bool in_prolog)
{
  putEmptySequence(name, in_prolog);
  size_t count(0);
  for (auto const & item : value) {
    put(name + "[" + std::to_string(count++) + "]", item, in_prolog);
  }
}

template <typename T>
inline
typename std::enable_if<tt::is_numeric<T>::value, void>::type
fhicl::intermediate_table::
put(std::string const & name,
       T value, // Number
       bool in_prolog)
{
  insert(name, in_prolog, NUMBER, detail::encode(value));
}

inline
void
fhicl::intermediate_table::
putEmptySequence(std::string const & name,
                    bool in_prolog) // Sequence.
{
  insert(name, in_prolog, SEQUENCE, sequence_t());
}

inline
void
fhicl::intermediate_table::
putEmptyTable(std::string const & name,
                    bool in_prolog) // Table.
{
  insert(name, in_prolog, TABLE, table_t());
}

inline
void
fhicl::intermediate_table::
putNil(std::string const & name,
          bool in_prolog) // Nil.
{
  insert(name, in_prolog, NIL, detail::encode((void *)0));
}
#endif

// Local Variables:
// mode: c++
// End:
