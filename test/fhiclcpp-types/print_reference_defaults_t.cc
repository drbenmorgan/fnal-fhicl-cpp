#include "fhiclcpp/types/Atom.h"
#include "fhiclcpp/types/Sequence.h"
#include "fhiclcpp/types/Table.h"
#include "fhiclcpp/types/Tuple.h"

#include <iostream>

namespace {

  using namespace fhicl;
  //============================================================================
  // Job configuration
  //

  struct PSet2 {
    Atom<std::string> test { Key("test"), "a" };
    Sequence<int> sequence { Key("sequence"), Comment("Just to see if this works"), {1,3,67,5} };
    Sequence<double,5> array { Key("array"),  {1.4,2.3,4.,8.,123098.} };
  };

  struct Test {
    Atom<int> value { Key("c"), 4 };
  };

  struct ForRootInput {
    Atom<std::string> primary { Key("a"), "cool" };
    Sequence<std::string> secondaries { Key("b") };
  };

  struct Configuration {

    Atom<bool> flag { Key("flag"), true };
    Atom<int> value { Key("value"), 7 };
    Atom<std::string> str { Key("string"), "hi mom" };

    Tuple<double,std::string> pair { Key("pair"),     {2.2345678, "Finally, I know what I'm doing."} };
    Tuple<int,std::string,double> tup { Key("tuple"), {5,"some string",16. } };

    Sequence< Tuple<int,double> >    vt { Key("vecOfTuples")  , { {3,27.} } };
    Sequence< Tuple<int,double>, 2 > at { Key("arrayOfTuples"), { {4,15.}, {2,17.} }  };

    Sequence< Sequence<int> >      vv { Key("vecOfVecs")    , { {1,3,5} }       };
    Sequence< Sequence<int, 2> >   va { Key("vecOfArrays")  , { {1,2} }         };
    Sequence< Sequence<int>, 2 >   av { Key("arrayOfVecs")  , { {1,2,3}, {1} }  };
    Sequence< Sequence<int,2>, 2 > aa { Key("arrayOfArrays"), { {0,2 }, {2,3} } };

    Tuple< Sequence<double>, bool, int>   tv { Key("tupleWithVec")  , { {4.2,7.3},false, 5} };
    Tuple< Sequence<double,2>, bool, int> ta { Key("tupleWithArray"), { {3.6,5.3},false, 5} };
    Tuple< Tuple<int,double>, bool, int>  tt { Key("tupleWithTuple"), { {4,5.9}  , true, 8} };

    Table<PSet2> pset2 { Key("pset2"), Comment("This should work now\ndouble line even.") };
  };

}

int main()
{
  Table<Configuration> pset { Key("pset") };
  pset.print_reference(std::cout);
}
