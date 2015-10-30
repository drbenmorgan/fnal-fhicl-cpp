// ======================================================================
//
// test types keymap: verify that types 1-13 create the correct key
//                    maps for ParameterSet validation.
//
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

BOOST_AUTO_TEST_SUITE( types_keymap_test )

// [1] Atom<T>
BOOST_AUTO_TEST_CASE( one_atom_t )
{
  Atom<int> test { Name("atom") };
  auto map = key_map(&test);
  auto ref = {"atom"};
  BOOST_CHECK_EQUAL_COLLECTIONS( map.begin(), map.end(),
                                 ref.begin(), ref.end());
}

// [2] Sequence<T>
BOOST_AUTO_TEST_CASE( one_sequence_t )
{
  Sequence<int> test { Name("sequence") };
  auto map = key_map(&test);
  auto ref = {"sequence",
              "sequence[0]"}; // includes one member for comparison purposes
  BOOST_CHECK_EQUAL_COLLECTIONS( map.begin(), map.end(),
                                 ref.begin(), ref.end());
}

// [3] Sequence<T,SZ>
BOOST_AUTO_TEST_CASE( one_sequence_2_t )
{
  Sequence<int,2> test { Name("sequence") };
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
  Tuple<int,double,bool> test { Name("tuple") };
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
  Tuple< Sequence<int>,double,bool> test { Name("tuple") };
  auto map = key_map(&test);
  auto ref = {"tuple",
              "tuple[0]",
              "tuple[0][0]",
              "tuple[1]",
              "tuple[2]"};
  BOOST_CHECK_EQUAL_COLLECTIONS( map.begin(), map.end(),
                                 ref.begin(), ref.end());
}

// [6] Tuple< Sequence<T,SZ>, U...>
BOOST_AUTO_TEST_CASE( bounded_seq_in_tuple_t )
{
  Tuple< Sequence<int,2>,double,bool> test { Name("tuple") };
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
  Tuple< Tuple<int,float>,double,bool> test { Name("tuple") };
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
  Sequence< Tuple<int,float> > test { Name("seqtuple") };
  auto map = key_map(&test);
  auto ref = {"seqtuple",
              "seqtuple[0]",
              "seqtuple[0][0]",
              "seqtuple[0][1]"};
  BOOST_CHECK_EQUAL_COLLECTIONS( map.begin(), map.end(),
                                 ref.begin(), ref.end());
}

// [9] Sequence< Tuple<T...>, SZ >
BOOST_AUTO_TEST_CASE( tuple_in_seq_2_t )
{
  Sequence< Tuple<int,float>, 2 > test { Name("seqtuple") };
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
  Sequence< Sequence<int> > test { Name("seqseq") };
  auto map = key_map(&test);
  auto ref = {"seqseq",
              "seqseq[0]",
              "seqseq[0][0]"};
  BOOST_CHECK_EQUAL_COLLECTIONS( map.begin(), map.end(),
                                 ref.begin(), ref.end());
}

// [11] Sequence< Sequence<T,SZ> >
BOOST_AUTO_TEST_CASE( seq_2_in_seq_t )
{
  Sequence< Sequence<int,2> > test { Name("seqseq") };
  auto map = key_map(&test);
  auto ref = {"seqseq",
              "seqseq[0]",
              "seqseq[0][0]",
              "seqseq[0][1]"};
  BOOST_CHECK_EQUAL_COLLECTIONS( map.begin(), map.end(),
                                 ref.begin(), ref.end());
}

// [12] Sequence< Sequence<T,SZ> >
BOOST_AUTO_TEST_CASE( seq_in_seq_2_t )
{
  Sequence< Sequence<int>, 2> test { Name("seqseq") };
  auto map = key_map(&test);
  auto ref = {"seqseq",
              "seqseq[0]",
              "seqseq[0][0]",
              "seqseq[1]",
              "seqseq[1][0]"};
  BOOST_CHECK_EQUAL_COLLECTIONS( map.begin(), map.end(),
                                 ref.begin(), ref.end());
}

// [13] Sequence< Sequence<T,SZ>, SZ >
BOOST_AUTO_TEST_CASE( seq_2_in_seq_2_t )
{
  Sequence< Sequence<int, 2>, 2> test { Name("seqseq") };
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
