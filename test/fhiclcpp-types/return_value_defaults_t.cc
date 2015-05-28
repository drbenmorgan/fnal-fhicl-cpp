// ======================================================================
//
// test static_types return values with defaults

/* The purpose of this test is to verify that types 1-13 below return
   the correct values

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

#define BOOST_TEST_MODULE ( return value defaults )

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

BOOST_AUTO_TEST_SUITE( static_types_return_value_defaults_test )

// [1] Atom<T>
BOOST_AUTO_TEST_CASE( one_atom_t )
{
  Atom<int> test { Key("atom"), 4 };
  BOOST_CHECK_EQUAL( test(), 4 );
}

// [2] Sequence<T>
BOOST_AUTO_TEST_CASE( one_sequence_t )
{
  auto ref = {1,2,4};
  Sequence<int> test { Key("sequence"), ref };
  auto rval = test();
  BOOST_CHECK_EQUAL_COLLECTIONS( rval.begin(), rval.end(),
                                 ref.begin(), ref.end());
  std::size_t i{};
  for ( auto const& elem : ref )
    BOOST_CHECK_EQUAL( elem, test(i++) );

}

// [3] Sequence<T,SZ>
BOOST_AUTO_TEST_CASE( one_sequence_2_t )
{
  auto ref = {5,7};
  Sequence<int,2> test { Key("sequence"), ref };
  auto rval = test();
  BOOST_CHECK_EQUAL_COLLECTIONS( rval.begin(), rval.end(),
                                 ref.begin(), ref.end());

  std::size_t i{};
  for ( auto const& elem : ref )
    BOOST_CHECK_EQUAL( elem, test(i++) );

}

// [4] Tuple<T...>
BOOST_AUTO_TEST_CASE( one_tuple_t )
{
  Tuple<int,double,bool> test { Key("tuple"), {4,1.5,false} };
  BOOST_CHECK_EQUAL( test.get<0>(), 4     );
  BOOST_CHECK_EQUAL( test.get<1>(), 1.5   );
  BOOST_CHECK_EQUAL( test.get<2>(), false );
}

// [5] Tuple< Sequence<T>, U...>
BOOST_AUTO_TEST_CASE( seq_in_tuple_t )
{
  auto ref = {1,3,5};
  Tuple< Sequence<int>,double,bool> test { Key("tuple"), { ref, 4.6, true } };
  auto rval = test.get<0>();
  BOOST_CHECK_EQUAL_COLLECTIONS( rval.begin(), rval.end(),
                                 ref.begin(), ref.end() );
  BOOST_CHECK_EQUAL( test.get<1>(), 4.6  );
  BOOST_CHECK_EQUAL( test.get<2>(), true );
}

// [6] Tuple< Sequence<T,SZ>, U...>
BOOST_AUTO_TEST_CASE( bounded_seq_in_tuple_t )
{
  auto ref = {9,15};
  Tuple< Sequence<int,2>,double,bool> test { Key("tuple"), { ref, 0.2, false} };
  auto rval = test.get<0>();
  BOOST_CHECK_EQUAL_COLLECTIONS( rval.begin(), rval.end(),
                                 ref.begin(), ref.end() );
  BOOST_CHECK_EQUAL( test.get<1>(), 0.2   );
  BOOST_CHECK_EQUAL( test.get<2>(), false );
}

// [7] Tuple< Tuple<T...>, U...>
BOOST_AUTO_TEST_CASE( tuple_in_tuple_t )
{
  Tuple< Tuple<int,float>,double,bool> test { Key("tuple"), { {4,3.7f}, 8.1, true } };
  auto tuple0 [[gnu::unused ]] = test.get<0>();
  BOOST_CHECK_EQUAL( std::get<0>( tuple0 ), 4 );
  BOOST_CHECK_EQUAL( std::get<1>( tuple0 ), 3.7f );
  BOOST_CHECK_EQUAL( test.get<1>(), 8.1 );
  BOOST_CHECK_EQUAL( test.get<2>(), true );
}

// [8] Sequence< Tuple<T...> >
BOOST_AUTO_TEST_CASE( tuple_in_seq_t )
{
  std::vector<std::tuple<int,float>> ref_vec {
      std::make_tuple(2,5.4f),
      std::make_tuple(4,104.5f),
      std::make_tuple(8,15.3f) };
  Sequence< Tuple<int,float> > test { Key("seqtuple"), { {2,5.4f}, {4,104.5f}, {8,15.3f} } };
  std::size_t i{};
  for ( auto const& elem : test() ) {
    BOOST_CHECK_EQUAL( std::get<0>( elem ), std::get<0>( ref_vec.at(i) ) );
    BOOST_CHECK_EQUAL( std::get<1>( elem ), std::get<1>( ref_vec.at(i++) ) );
  }
}

// [9] Sequence< Tuple<T...>, SZ >
BOOST_AUTO_TEST_CASE( tuple_in_seq_2_t )
{
  std::array<std::tuple<int,float>,2> ref_vec { std::make_tuple(1,2.3f), std::make_tuple(9,3.2f) };

  Sequence< Tuple<int,float>, 2 > test { Key("seqtuple"), { {1,2.3f},{9,3.2f} } };
  std::size_t i{};
  for ( auto const& elem : test() ) {
    BOOST_CHECK_EQUAL( std::get<0>( elem ), std::get<0>( ref_vec.at(i  ) ) );
    BOOST_CHECK_EQUAL( std::get<1>( elem ), std::get<1>( ref_vec.at(i++) ) );
  }
}

// [10] Sequence< Sequence<T> >
BOOST_AUTO_TEST_CASE( seq_in_seq_t )
{
  auto ref_vec = std::vector<std::vector<int>>{ {1,5,7},{2} };
  Sequence< Sequence<int> > test { Key("seqseq"), { {1,5,7},{2} } };
  auto rval = test();
  std::size_t i{};
  for ( auto const& val : test() ) {
    auto ref = ref_vec.at(i++);
    BOOST_CHECK_EQUAL_COLLECTIONS( val.begin(), val.end(),
                                   ref.begin(), ref.end());
  }

  i = 0ul;
  for ( auto const& ref : ref_vec ) {
    auto val = test(i++);
    BOOST_CHECK_EQUAL_COLLECTIONS( val.begin(), val.end(),
                                   ref.begin(), ref.end());
  }
}

// [11] Sequence< Sequence<T,SZ> >
BOOST_AUTO_TEST_CASE( seq_2_in_seq_t )
{
  auto ref_vec = std::vector<std::array<int,2>>{ {1,2} };
  Sequence< Sequence<int,2> > test { Key("seqseq"), { {1,2} } };
  auto rval = test();
  std::size_t i{};
  for ( auto const& val : test() ) {
    auto ref = ref_vec.at(i++);
    BOOST_CHECK_EQUAL_COLLECTIONS( val.begin(), val.end(),
                                   ref.begin(), ref.end());
  }
}

// [12] Sequence< Sequence<T>,SZ >
BOOST_AUTO_TEST_CASE( seq_in_seq_2_t )
{
  std::array<std::vector<int>,2> ref_vec { std::vector<int>{4}, std::vector<int>{1,4,9,1} };

  Sequence< Sequence<int>, 2> test { Key("seqseq"), { {4}, {1,4,9,1} } };
  std::size_t i{};
  for ( auto const& val : test() ) {
    auto ref = ref_vec.at(i++);
    BOOST_CHECK_EQUAL_COLLECTIONS( val.begin(), val.end(),
                                   ref.begin(), ref.end());
  }
}

// [13] Sequence< Sequence<T,SZ>, SZ >
BOOST_AUTO_TEST_CASE( seq_2_in_seq_2_t )
{
  std::array<std::array<int,2>,2> ref_vec { std::array<int,2>{6,7}, std::array<int,2>{2,1} };

  Sequence< Sequence<int, 2>, 2> test { Key("seqseq"), { {6,7},{2,1} } };
  std::size_t i{};
  for ( auto const& val : test() ) {
    auto ref = ref_vec.at(i++);
    BOOST_CHECK_EQUAL_COLLECTIONS( val.begin(), val.end(),
                                   ref.begin(), ref.end());
  }
}
BOOST_AUTO_TEST_SUITE_END()
