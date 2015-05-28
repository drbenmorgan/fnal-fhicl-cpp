// ======================================================================
//
// test static_types keymap

/* The purpose of this test is to verify that types 1-13 below return
   the correct types.

   In what follows, ’T’ represents a type supported by an Atom<>
   and ’S’ represents an explicitly constructed struct that may
   contain Atom<>, Sequence<>, Tuple<>, or Table<> objects.

   [ 1] Atom<T>;
   [ 2] Sequence<T>
   [ 3] Sequence<T,SZ>
   [ 4] Tuple<T...>
   [ 5] Tuple< Sequence<T>, U...>
   [ 6] Tuple< Sequence<T,SZ>, U...>
   [ 7] Tuple< Tuple<T...>,U...>
   [ 8] Sequence< Tuple<T...> >
   [ 9] Sequence< Tuple<T...>, SZ >
   [10] Sequence< Sequence<T> >
   [11] Sequence< Sequence<T,SZ> >
   [12] Sequence< Sequence<T>, SZ >
   [13] Sequence< Sequence<T,SZ>, SZ >

   14-21 cannot support default arguments (since Table<> cannot have a default)

   [14] Table<S>
   [15] Sequence< Table<S> >
   [16] Sequence< Table<S>, SZ >
   [17] Tuple< Table<S>, U... >
   [18] Tuple< Sequence< Table<S> >, U... >
   [19] Tuple< Sequence< Table<S>, SZ>, U... >
   [20] Sequence< Tuple< Table<S>, U... > >
   [21] Sequence< Tuple< Table<S>, U... >, SZ>

 */

// ======================================================================

#define BOOST_TEST_MODULE ( return types test )

#include "boost/test/auto_unit_test.hpp"

#include "fhiclcpp/types/Atom.h"
#include "fhiclcpp/types/detail/ParameterReferenceRegistry.h"
#include "fhiclcpp/types/Sequence.h"
#include "fhiclcpp/types/Tuple.h"

#include <array>
#include <string>
#include <vector>
#include <tuple>
#include <type_traits>

using namespace fhicl;
using namespace fhicl::detail;
using namespace std;

namespace {

  template<typename R, typename T>
  void require_type_as(T& t)
  {
    auto rt = t();
    ParameterReferenceRegistry::instance().clear();
    BOOST_CHECK( (std::is_same<R,decltype(rt)>::value) );
  }

}

BOOST_AUTO_TEST_SUITE( static_types_return_types_test )

// [1] Atom<T>
BOOST_AUTO_TEST_CASE( one_atom_t )
{
  Atom<int> test { Key("atom") };
  require_type_as<int>( test );
}

// [2] Sequence<T>
BOOST_AUTO_TEST_CASE( one_sequence_t )
{
  Sequence<int> test { Key("sequence") };
  require_type_as<std::vector<int>>( test );
}

// [3] Sequence<T,SZ>
BOOST_AUTO_TEST_CASE( one_sequence_2_t )
{
  Sequence<int,2> test { Key("sequence") };
  require_type_as<std::array<int,2>>( test );
}

// [4] Tuple<T...>
BOOST_AUTO_TEST_CASE( one_tuple_t )
{
  Tuple<int,double,bool> test { Key("tuple") };
  require_type_as<std::tuple<int,double,bool>>( test );
}

// [5] Tuple< Sequence<T>, U...>
BOOST_AUTO_TEST_CASE( seq_in_tuple_t )
{
  Tuple< Sequence<int>,double,bool> test { Key("tuple") };
  require_type_as<std::tuple<std::vector<int>,double,bool>>( test );
}

// [6] Tuple< Sequence<T,SZ>, U...>
BOOST_AUTO_TEST_CASE( bounded_seq_in_tuple_t )
{
  Tuple< Sequence<int,2>,double,bool> test { Key("tuple") };
  require_type_as<std::tuple<std::array<int,2>,double,bool>>( test );
}

// [7] Tuple< Tuple<T...>, U...>
BOOST_AUTO_TEST_CASE( tuple_in_tuple_t )
{
  Tuple< Tuple<int,float>,double,bool> test { Key("tuple") };
  require_type_as<std::tuple<std::tuple<int,float>,double,bool>>( test );
}

// [8] Sequence< Tuple<T...> >
BOOST_AUTO_TEST_CASE( tuple_in_seq_t )
{
  Sequence< Tuple<int,float> > test { Key("seqtuple") };
  require_type_as<std::vector<std::tuple<int,float>>>( test );
}

// [9] Sequence< Tuple<T...>, SZ >
BOOST_AUTO_TEST_CASE( tuple_in_seq_2_t )
{
  Sequence< Tuple<int,float>, 2 > test { Key("seqtuple") };
  require_type_as<std::array<std::tuple<int,float>,2>>( test );
}

// [10] Sequence< Sequence<T> >
BOOST_AUTO_TEST_CASE( seq_in_seq_t )
{
  Sequence< Sequence<int> > test { Key("seqseq") };
  require_type_as<std::vector<std::vector<int>>>( test );
}

// [11] Sequence< Sequence<T,SZ> >
BOOST_AUTO_TEST_CASE( seq_2_in_seq_t )
{
  Sequence< Sequence<int,2> > test { Key("seqseq") };
  require_type_as<std::vector<std::array<int,2>>>( test );
}

// [12] Sequence< Sequence<T,SZ> >
BOOST_AUTO_TEST_CASE( seq_in_seq_2_t )
{
  Sequence< Sequence<int>, 2> test { Key("seqseq") };
  require_type_as<std::array<std::vector<int>,2>>( test );
}

// [13] Sequence< Sequence<T,SZ>, SZ >
BOOST_AUTO_TEST_CASE( seq_2_in_seq_2_t )
{
  Sequence< Sequence<int, 2>, 2> test { Key("seqseq") };
  require_type_as<std::array<std::array<int,2>,2>>( test );
}
BOOST_AUTO_TEST_SUITE_END()
