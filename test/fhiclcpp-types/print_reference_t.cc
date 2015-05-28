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
    Atom<std::string> test { Key("test"), Comment("this works") };
    Sequence<int> sequence { Key("sequence") };
    Sequence<double,5> array { Key("array") };
  };

  struct Test {
    Atom<int> value { Key("c") };
  };

  struct ForRootInput {
    Atom<std::string> primary { Key("a") };
    Sequence<std::string> secondaries { Key("b") };
    Sequence< Table<Test> > tablesequence2 { Key("tableSequence2") };
  };

  struct Configuration {
    Atom<bool> flag { Key("flag") };

    Tuple<double,std::string> pair { Key("pair") };
    Tuple<int,std::string,double> tup { Key("tuple") };

    Sequence< Tuple<int,double> > vt { Key("vecOfTuples") };
    Sequence< Tuple<int,double>, 2 > at { Key("arrayOfTuples") };

    Sequence< Sequence<int> >      vv { Key("vecOfVecs") };
    Sequence< Sequence<int, 2> >   va { Key("vecOfArrays") };
    Sequence< Sequence<int>, 2 >   av { Key("arrayOfVecs") };
    Sequence< Sequence<int,2>, 2 > aa { Key("arrayOfArrays") };

    Tuple< Sequence<double>, bool, int> tv { Key("tupleWithVec") };
    Tuple< Sequence<double,2>, bool, int> ta { Key("tupleWithArray") };
    Tuple< Tuple<int,double>, bool, int> tt { Key("tupleWithTuple") };

    Sequence< Table<ForRootInput> > vtable  { Key("vecOfTables") };
    Sequence< Table<ForRootInput>,2 > atable  { Key("arrayOfTables") };
    Tuple< Table<ForRootInput>, bool, int> tupleWithTable { Key("tupleWithTable") };
    Tuple< Sequence< Table<ForRootInput> >, double > tvtable { Key("tupleWithVecTable") };
    Tuple< Sequence< Table<ForRootInput>, 2>, double > tstable { Key("tupleWithArrTable") };
    Sequence< Tuple< Table<ForRootInput>, double> > vttable { Key("vecWithTupleTable") };
    Sequence< Tuple< Table<ForRootInput>, double>, 2 > attable { Key("arrWithTupleTable") };
    Table<PSet2> pset2 { Key("pset2") };
  };

}

int main()
{
  Table<Configuration> pset { Key("pset") };
  pset.print_reference(std::cout);
}
