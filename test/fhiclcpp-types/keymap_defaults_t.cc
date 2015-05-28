// ======================================================================
//
// test static_types keymap with defaults

/* The purpose of this test is to verify that types 1-13 below create
   the correct key maps for ParameterSet validation whenever default
   arguments are included during construction.

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

#define BOOST_TEST_MODULE ( keymap test )

#include "boost/test/auto_unit_test.hpp"
#include "boost/test/test_tools.hpp"

#include "fhiclcpp/types/Atom.h"
#include "fhiclcpp/types/detail/ParameterReferenceRegistry.h"
#include "fhiclcpp/types/Sequence.h"
#include "fhiclcpp/types/Tuple.h"

#include <iostream>
#include <string>
#include <vector>

using namespace fhicl;
using namespace fhicl::detail;
using namespace std;

namespace {

  std::vector<std::string>
  key_map(ParameterBase const * pb)
  {
    auto keys = ParameterReferenceRegistry::get_parameter_keys(pb);
    ParameterReferenceRegistry::instance().clear();
    return keys;
  }

}

BOOST_AUTO_TEST_SUITE( static_types_keymap_defaults_test )

// [1] Atom<T>
BOOST_AUTO_TEST_CASE( one_atom_t )
{
  Atom<int> test { Key("atom"), 4 };
  auto map = key_map(&test);
  auto ref = {"atom"};
  BOOST_CHECK_EQUAL_COLLECTIONS( map.begin(), map.end(),
                                 ref.begin(), ref.end());
}

// [2] Sequence<T>
BOOST_AUTO_TEST_CASE( one_sequence_t )
{
  Sequence<int> test { Key("sequence"), {1,2,4} };
  auto map = key_map(&test);
  auto ref = {"sequence",
              "sequence[0]",
              "sequence[1]",
              "sequence[2]"};
  BOOST_CHECK_EQUAL_COLLECTIONS( map.begin(), map.end(),
                                 ref.begin(), ref.end());
}

// [3] Sequence<T,SZ>
BOOST_AUTO_TEST_CASE( one_sequence_2_t )
{
  Sequence<int,2> test { Key("sequence"), {5,7} };
  auto map = key_map(&test);
  auto ref = {"sequence",
              "sequence[0]",
              "sequence[1]"};
  BOOST_CHECK_EQUAL_COLLECTIONS( map.begin(), map.end(),
                                 ref.begin(), ref.end());
}

// [4] Tuple<T...>
BOOST_AUTO_TEST_CASE( one_tuple_t )
{
  Tuple<int,double,bool> test { Key("tuple"), {4,1.5,false} };
  auto map = key_map(&test);
  auto ref = {"tuple",
              "tuple[0]",
              "tuple[1]",
              "tuple[2]"};
  BOOST_CHECK_EQUAL_COLLECTIONS( map.begin(), map.end(),
                                 ref.begin(), ref.end());
}

// [5] Tuple< Sequence<T>, U...>
BOOST_AUTO_TEST_CASE( seq_in_tuple_t )
{
  Tuple< Sequence<int>,double,bool> test { Key("tuple"), { {1,3,5},4.6,true } };
  auto map = key_map(&test);
  auto ref = {"tuple",
              "tuple[0]",
              "tuple[0][0]",
              "tuple[0][1]",
              "tuple[0][2]",
              "tuple[1]",
              "tuple[2]"};
  BOOST_CHECK_EQUAL_COLLECTIONS( map.begin(), map.end(),
                                 ref.begin(), ref.end());
}

// [6] Tuple< Sequence<T,SZ>, U...>
BOOST_AUTO_TEST_CASE( bounded_seq_in_tuple_t )
{
  Tuple< Sequence<int,2>,double,bool> test { Key("tuple"), { {9,15}, 0.2, false} };
  auto map = key_map(&test);
  auto ref = {"tuple",
              "tuple[0]",
              "tuple[0][0]",
              "tuple[0][1]",
              "tuple[1]",
              "tuple[2]"};
  BOOST_CHECK_EQUAL_COLLECTIONS( map.begin(), map.end(),
                                 ref.begin(), ref.end());
}

// [7] Tuple< Tuple<T...>, U...>
BOOST_AUTO_TEST_CASE( tuple_in_tuple_t )
{
  Tuple< Tuple<int,float>,double,bool> test { Key("tuple"), { {4,3.7f}, 8.1, true } };
  auto map = key_map(&test);
  auto ref = {"tuple",
              "tuple[0]",
              "tuple[0][0]",
              "tuple[0][1]",
              "tuple[1]",
              "tuple[2]"};
  BOOST_CHECK_EQUAL_COLLECTIONS( map.begin(), map.end(),
                                 ref.begin(), ref.end());
}

// [8] Sequence< Tuple<T...> >
BOOST_AUTO_TEST_CASE( tuple_in_seq_t )
{
  Sequence< Tuple<int,float> > test { Key("seqtuple"), { {2,5.4f}, {4,104.5f}, {8,15.3f} } };
  auto map = key_map(&test);
  auto ref = {"seqtuple",
              "seqtuple[0]",
              "seqtuple[0][0]",
              "seqtuple[0][1]",
              "seqtuple[1]",
              "seqtuple[1][0]",
              "seqtuple[1][1]",
              "seqtuple[2]",
              "seqtuple[2][0]",
              "seqtuple[2][1]"};
  BOOST_CHECK_EQUAL_COLLECTIONS( map.begin(), map.end(),
                                 ref.begin(), ref.end());
}

// [9] Sequence< Tuple<T...>, SZ >
BOOST_AUTO_TEST_CASE( tuple_in_seq_2_t )
{
  Sequence< Tuple<int,float>, 2 > test { Key("seqtuple"), { {1,2.3f},{9,3.2f} } };
  auto map = key_map(&test);
  auto ref = {"seqtuple",
              "seqtuple[0]",
              "seqtuple[0][0]",
              "seqtuple[0][1]",
              "seqtuple[1]",
              "seqtuple[1][0]",
              "seqtuple[1][1]"};
  BOOST_CHECK_EQUAL_COLLECTIONS( map.begin(), map.end(),
                                 ref.begin(), ref.end());
}

// [10] Sequence< Sequence<T> >
BOOST_AUTO_TEST_CASE( seq_in_seq_t )
{
  Sequence< Sequence<int> > test { Key("seqseq"), { { 1,5,7}, {2} }};
  auto map = key_map(&test);
  auto ref = {"seqseq",
              "seqseq[0]",
              "seqseq[0][0]",
              "seqseq[0][1]",
              "seqseq[0][2]",
              "seqseq[1]",
              "seqseq[1][0]"};
  BOOST_CHECK_EQUAL_COLLECTIONS( map.begin(), map.end(),
                                 ref.begin(), ref.end());
}

// [11] Sequence< Sequence<T,SZ> >
BOOST_AUTO_TEST_CASE( seq_2_in_seq_t )
{
  Sequence< Sequence<int,2> > test { Key("seqseq"), { {1,2} } };
  auto map = key_map(&test);
  auto ref = {"seqseq",
              "seqseq[0]",
              "seqseq[0][0]",
              "seqseq[0][1]"};
  BOOST_CHECK_EQUAL_COLLECTIONS( map.begin(), map.end(),
                                 ref.begin(), ref.end());
}

// [12] Sequence< Sequence<T>, SZ >
BOOST_AUTO_TEST_CASE( seq_in_seq_2_t )
{
  Sequence< Sequence<int>, 2> test { Key("seqseq"), { {4}, {1,4,9,1} } };
  auto map = key_map(&test);
  auto ref = {"seqseq",
              "seqseq[0]",
              "seqseq[0][0]",
              "seqseq[1]",
              "seqseq[1][0]",
              "seqseq[1][1]",
              "seqseq[1][2]",
              "seqseq[1][3]"};
  BOOST_CHECK_EQUAL_COLLECTIONS( map.begin(), map.end(),
                                 ref.begin(), ref.end());
}

// [13] Sequence< Sequence<T,SZ>, SZ >
BOOST_AUTO_TEST_CASE( seq_2_in_seq_2_t )
{
  Sequence< Sequence<int, 2>, 2> test { Key("seqseq"), { {6,7},{2,1} } };
  auto map = key_map(&test);
  auto ref = {"seqseq",
              "seqseq[0]",
              "seqseq[0][0]",
              "seqseq[0][1]",
              "seqseq[1]",
              "seqseq[1][0]",
              "seqseq[1][1]"};
  BOOST_CHECK_EQUAL_COLLECTIONS( map.begin(), map.end(),
                                 ref.begin(), ref.end());
}
BOOST_AUTO_TEST_SUITE_END()
