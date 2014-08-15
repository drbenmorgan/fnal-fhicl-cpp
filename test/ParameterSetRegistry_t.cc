#define BOOST_TEST_MODULE ( ParameterSetRegistry_t )
#include "boost/test/auto_unit_test.hpp"

#include "fhiclcpp/ParameterSetRegistry.h"
#include "fhiclcpp/make_ParameterSet.h"

#include "sqlite3.h"

using namespace fhicl;

using fhicl::detail::throwOnSQLiteFailure;

BOOST_AUTO_TEST_SUITE(ParameterSetRegistry_t)

BOOST_AUTO_TEST_CASE(Typedefs)
{
  using ctype = std::unordered_map<ParameterSetID, ParameterSet, detail::HashParameterSetID>;
  static_assert(std::is_same<ctype, ParameterSetRegistry::collection_type>::value, "collection_type failure");
  static_assert(std::is_same<ParameterSetID, ParameterSetRegistry::key_type>::value, "key_type failure");
  static_assert(std::is_same<ParameterSet, ParameterSetRegistry::mapped_type>::value, "mapped_type failure");
  static_assert(std::is_same<std::pair<ParameterSetID const, ParameterSet>, ParameterSetRegistry::value_type>::value, "value_type failure");
  static_assert(std::is_same<std::size_t, ParameterSetRegistry::size_type>::value, "size_type failure");
  static_assert(std::is_same<ctype::const_iterator, ParameterSetRegistry::const_iterator>::value, "const_iterator failure");
}

BOOST_AUTO_TEST_CASE(MakeAndAdd)
{
  BOOST_REQUIRE(ParameterSetRegistry::empty());
  BOOST_REQUIRE_EQUAL(ParameterSetRegistry::size(), 0ul);
  ParameterSet pset;
  make_ParameterSet("x: 5 y: { a: \"oops\" b: 9 } z: { c: \"Looooong striiiiiing.\" }", pset);
  BOOST_REQUIRE(!ParameterSetRegistry::empty());
  BOOST_REQUIRE_EQUAL(ParameterSetRegistry::size(), 2ul);
  ParameterSetRegistry::put(pset);
  BOOST_REQUIRE_EQUAL(ParameterSetRegistry::size(), 3ul);
  auto ps2 = ParameterSetRegistry::get(pset.id());
  BOOST_REQUIRE_EQUAL(pset.id(), ps2.id());
  BOOST_REQUIRE(pset == ps2);
}

BOOST_AUTO_TEST_CASE(Iterators)
{
  auto const & coll = ParameterSetRegistry::get();
  BOOST_REQUIRE(ParameterSetRegistry::begin() == coll.begin());
  BOOST_REQUIRE(ParameterSetRegistry::begin() == coll.cbegin());
  BOOST_REQUIRE(ParameterSetRegistry::cbegin() == coll.cbegin());
  BOOST_REQUIRE(ParameterSetRegistry::end() == coll.end());
  BOOST_REQUIRE(ParameterSetRegistry::end() == coll.cend());
  BOOST_REQUIRE(ParameterSetRegistry::cend() == coll.cend());
  BOOST_REQUIRE(ParameterSetRegistry::cbegin() != ParameterSetRegistry::cend());
}

BOOST_AUTO_TEST_CASE(AddFromIterAndGet)
{
  std::vector<ParameterSet> v1;
  auto expected_size = ParameterSetRegistry::size() + 2ul; // a3, x.
  for (auto s : { "a1: 4 b1: 7.0 c1: [ 5, 4, 3 ]",
        "a2: [ oh, my, stars ]",
        "a3: { x: { y: [ now, is, the, time, for, all, good, men ] } }"}) {
    ParameterSet pset;
    make_ParameterSet(s, pset);
    v1.emplace_back(pset);
  }
  BOOST_REQUIRE_EQUAL(ParameterSetRegistry::size(), expected_size );
  std::vector<ParameterSetRegistry::value_type> v2;
  std::string const f = "filler";
  for (auto p : v1 ) {
    p.put("f", f);
    v2.emplace_back(p.id(), std::move(p));
  }
  expected_size += 3;
  ParameterSetRegistry::put(v1.cbegin(), v1.cend());
  BOOST_REQUIRE_EQUAL(ParameterSetRegistry::size(), expected_size );
  expected_size += 3;
  ParameterSetRegistry::put(v2.cbegin(), v2.cend());
  BOOST_REQUIRE_EQUAL(ParameterSetRegistry::size(), expected_size);

  ParameterSet pset;
  ParameterSetRegistry::get(v2.front().first, pset);
  BOOST_REQUIRE(pset == v2.front().second);
  auto p2 = ParameterSetRegistry::get(v2.back().first);
  BOOST_REQUIRE(p2 == v2.back().second);
}

BOOST_AUTO_TEST_CASE(TestImport)
{
  auto expected_size = ParameterSetRegistry::size();
  sqlite3 * db = nullptr;
  BOOST_REQUIRE(!sqlite3_open(":memory:", &db));
  throwOnSQLiteFailure(db);
  char * errMsg;
  sqlite3_exec(db,
               "BEGIN TRANSACTION; DROP TABLE IF EXISTS ParameterSets;"
               "CREATE TABLE ParameterSets(ID PRIMARY KEY, PSetBlob); COMMIT;",
               0, 0, &errMsg);
  throwOnSQLiteFailure(db, errMsg);
  sqlite3_exec(db,
               "BEGIN TRANSACTION;", 0, 0, &errMsg);
  throwOnSQLiteFailure(db, errMsg);
  sqlite3_stmt * oStmt;
  sqlite3_prepare_v2(db, "INSERT INTO ParameterSets(ID, PSetBlob) VALUES(?, ?);", -1, &oStmt, NULL);
  throwOnSQLiteFailure(db);
  std::vector<std::pair<ParameterSet, bool> > v1;
  // testData: bool represents whether the top-level set is already
  // expected to be in the registry or not.
  std::vector<std::pair<std::string, bool> > testData
  { { "x: [ 1, 3, 5, 7 ]", false },
    { "a2: [ oh, my, stars ]", true }, // Should match a2, above.
    { "y1: \"Oh, home on the range\" y2: \"Where the deer and the antelope roam\"", false } };
  for (auto const &  p : testData) {
    ParameterSet pset;
    make_ParameterSet(p.first, pset);
    v1.emplace_back(pset, p.second);
    std::string id(pset.id().to_string());
    std::string psBlob(pset.to_compact_string());
    sqlite3_bind_text(oStmt, 1, id.c_str(), id.size() + 1, SQLITE_STATIC);
    throwOnSQLiteFailure(db);
    sqlite3_bind_text(oStmt, 2, psBlob.c_str(), psBlob.size() + 1, SQLITE_STATIC);
    throwOnSQLiteFailure(db);
    BOOST_REQUIRE_EQUAL(sqlite3_step(oStmt), SQLITE_DONE);
    sqlite3_reset(oStmt);
    throwOnSQLiteFailure(db);
    if (p.second) {
      // Should be in registry already.
      BOOST_REQUIRE(ParameterSetRegistry::get().find(pset.id()) !=
                    ParameterSetRegistry::get().cend());
    } else {
      // Should *not* be in registry.
      BOOST_REQUIRE(ParameterSetRegistry::get().find(pset.id()) ==
                    ParameterSetRegistry::get().cend());
    }
  }
  sqlite3_finalize(oStmt);
  throwOnSQLiteFailure(db);
  BOOST_REQUIRE_EQUAL(ParameterSetRegistry::size(), expected_size);
  ParameterSetRegistry::importFrom(db);
  // Make sure the registry didn't expand as a result of the insert.
  BOOST_REQUIRE_EQUAL(ParameterSetRegistry::size(), expected_size);
  BOOST_REQUIRE_EQUAL(sqlite3_close(db), SQLITE_OK);
  for (auto const & p : v1) {
    if (p.second) {
      // Should be in registry already.
      BOOST_REQUIRE(ParameterSetRegistry::get().find(p.first.id()) !=
                    ParameterSetRegistry::get().cend());
    } else {
      // Make sure the import didn't inject them into the registry.
      BOOST_REQUIRE(ParameterSetRegistry::get().find(p.first.id()) ==
                    ParameterSetRegistry::get().cend());
      // We expect the get() call below to increase the size of the
      // registry by pulling the entry in from the backing DB.
      ++expected_size;
    }
    ParameterSet p2 = ParameterSetRegistry::get(p.first.id());
    BOOST_REQUIRE(p2 == p.first);
    BOOST_REQUIRE_EQUAL(ParameterSetRegistry::size(), expected_size);
  }
}

BOOST_AUTO_TEST_CASE(TestExport)
{
  sqlite3 * db = nullptr;
  BOOST_REQUIRE(!sqlite3_open(":memory:", &db));
  // Check empty!
  sqlite3_stmt * stmt;
  // Make sure we get our own fresh and empty DB.
  sqlite3_prepare_v2(db, "SELECT 1 from sqlite_master where type='table' and name='ParameterSets';",
                     -1, &stmt, NULL);
  BOOST_REQUIRE_EQUAL(sqlite3_step(stmt), SQLITE_DONE); // No rows.
  sqlite3_reset(stmt);
  ParameterSetRegistry::exportTo(db);
  BOOST_REQUIRE_EQUAL(sqlite3_step(stmt), SQLITE_ROW); // Found table.
  sqlite3_finalize(stmt);
  sqlite3_prepare_v2(db, "SELECT COUNT(*) from ParameterSets;", -1, &stmt, NULL);
  BOOST_REQUIRE_EQUAL(sqlite3_step(stmt), SQLITE_ROW);
  BOOST_REQUIRE_EQUAL(sqlite3_column_int64(stmt, 0), 13l);
  sqlite3_finalize(stmt);
  sqlite3_close(db);
}

BOOST_AUTO_TEST_SUITE_END()
