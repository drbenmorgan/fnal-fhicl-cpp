#include "fhiclcpp/ParameterSetRegistry.h"

#include "fhiclcpp/ParameterSetID.h"
#include "fhiclcpp/exception.h"
#include "fhiclcpp/make_ParameterSet.h"

using fhicl::detail::throwOnSQLiteFailure;

namespace {
  sqlite3 * openPrimaryDB()
  {
    sqlite3 * result;
    sqlite3_open(":memory:", &result);
    fhicl::detail::throwOnSQLiteFailure(result);
    char * errMsg = nullptr;
    sqlite3_exec(result,
                 "BEGIN TRANSACTION;"
                 "CREATE TABLE ParameterSets(ID PRIMARY KEY, PSetBlob); COMMIT;",
                 0, 0, &errMsg);
    throwOnSQLiteFailure(result, errMsg);
    return result;
  }
}

void
fhicl::detail::
throwOnSQLiteFailure(sqlite3 * db, char *msg)
{
  std::string msgString(msg ? msg : "" );
  sqlite3_free(msg);
  if (db == nullptr) {
    throw fhicl::exception(fhicl::error::cant_open_db,
                           "Can't open DB.");
  }
  auto errcode = sqlite3_errcode(db);
  if (errcode != SQLITE_OK) {
    // Caller's responsibility to make sure this really is an error and
    // not (say) SQLITE_ROW or SQLITE_DONE,
    throw exception(error::sql_error, "SQLite error:")
      << sqlite3_errstr(errcode)
      << " ("
      << errcode
      << "): "
      << sqlite3_errmsg(db)
      << (msgString.empty() ? "" : (std::string(". ") + msgString));
  }
}

fhicl::ParameterSetRegistry::
~ParameterSetRegistry()
{
  sqlite3_finalize(stmt_);
  throwOnSQLiteFailure(primaryDB_);
  int retcode;
  do {
    retcode = sqlite3_close(primaryDB_);
  } while (retcode == SQLITE_BUSY);
}

void
fhicl::ParameterSetRegistry::
importFrom(sqlite3 * db)
{
  // This does *not* cause anything new to be imported into the registry
  // itself, just its backing DB.
  sqlite3_stmt * iStmt, * oStmt;
  sqlite3 * primaryDB = instance_().primaryDB_;
  sqlite3_prepare_v2(db,
                     "SELECT ID, PSetBlob FROM ParameterSets;",
                     -1, &iStmt, NULL);
  throwOnSQLiteFailure(db);
  // Index constraint on ID will prevent duplicates via INSERT OR IGNORE.
  sqlite3_prepare_v2(primaryDB,
                     "INSERT OR IGNORE INTO ParameterSets(ID, PSetBlob) VALUES(?, ?);",
                     -1, &oStmt, NULL);
  throwOnSQLiteFailure(primaryDB);

  int retcode = 0;
  std::string idString;
  std::string psBlob;
  while ((retcode = sqlite3_step(iStmt)) == SQLITE_ROW) {
    idString = reinterpret_cast<char const *>
               (sqlite3_column_text(iStmt, 0));
    psBlob = reinterpret_cast<char const *>
             (sqlite3_column_text(iStmt, 1));
    sqlite3_bind_text(oStmt, 1, idString.c_str(), idString.size() + 1, SQLITE_STATIC);
    throwOnSQLiteFailure(primaryDB);
    sqlite3_bind_text(oStmt, 2, psBlob.c_str(), psBlob.size() + 1, SQLITE_STATIC);
    throwOnSQLiteFailure(primaryDB);
    switch (sqlite3_step(oStmt)) {
    case SQLITE_DONE:
      break; // OK
    default:
      throwOnSQLiteFailure(primaryDB);
    }
    sqlite3_reset(oStmt);
    throwOnSQLiteFailure(primaryDB);
  }
  sqlite3_finalize(oStmt);
  throwOnSQLiteFailure(primaryDB);
  sqlite3_finalize(iStmt);
  throwOnSQLiteFailure(db);
}

void
fhicl::ParameterSetRegistry::
exportTo(sqlite3 * db)
{
  char * errMsg = nullptr;
  sqlite3_exec(db,
               "BEGIN TRANSACTION; DROP TABLE IF EXISTS ParameterSets;"
               "CREATE TABLE ParameterSets(ID PRIMARY KEY, PSetBlob); COMMIT;",
               0, 0, &errMsg);
  throwOnSQLiteFailure(db, errMsg);
  sqlite3_stmt * oStmt;
  sqlite3_prepare_v2(db, "INSERT OR IGNORE INTO ParameterSets(ID, PSetBlob) VALUES(?, ?);", -1, &oStmt, NULL);
  throwOnSQLiteFailure(db);
  for (auto const & p : instance_().registry_) {
    std::string id(p.first.to_string());
    std::string psBlob(p.second.to_compact_string());
    sqlite3_bind_text(oStmt, 1, id.c_str(), id.size() + 1, SQLITE_STATIC);
    throwOnSQLiteFailure(db);
    sqlite3_bind_text(oStmt, 2, psBlob.c_str(), psBlob.size() + 1, SQLITE_STATIC);
    throwOnSQLiteFailure(db);
    switch (sqlite3_step(oStmt)) {
    case SQLITE_DONE:
      sqlite3_reset(oStmt);
      throwOnSQLiteFailure(db);
      break; // OK
    default:
      throwOnSQLiteFailure(db);
    }
  }
  sqlite3_stmt * iStmt;
  sqlite3 * primaryDB = instance_().primaryDB_;
  sqlite3_prepare_v2(primaryDB,
                     "SELECT ID,PSetBlob FROM ParameterSets",
                     -1, &iStmt, NULL);
  throwOnSQLiteFailure(primaryDB);
  while (sqlite3_step(iStmt) == SQLITE_ROW) {
    std::string idString = reinterpret_cast<char const *>
                           (sqlite3_column_text(iStmt, 0));
    std::string psBlob = reinterpret_cast<char const *>
                         (sqlite3_column_text(iStmt, 1));
    sqlite3_bind_text(oStmt, 1, idString.c_str(), idString.size() + 1, SQLITE_STATIC);
    throwOnSQLiteFailure(db);
    sqlite3_bind_text(oStmt, 2, psBlob.c_str(), psBlob.size() + 1, SQLITE_STATIC);
    throwOnSQLiteFailure(db);
    switch (sqlite3_step(oStmt)) {
    case SQLITE_DONE:
      sqlite3_reset(oStmt);
      throwOnSQLiteFailure(db);
      break; // OK
    default:
      throwOnSQLiteFailure(db);
    }
  }
  sqlite3_finalize(iStmt);
  throwOnSQLiteFailure(primaryDB);
  sqlite3_finalize(oStmt);
  throwOnSQLiteFailure(db);
}

void
fhicl::ParameterSetRegistry::
stageIn()
{
  sqlite3_stmt * stmt;
  sqlite3 * primaryDB = instance_().primaryDB_;
  auto & registry = instance_().registry_;
  sqlite3_prepare_v2(primaryDB,
                     "SELECT ID, PSetBlob FROM ParameterSets;",
                     -1, &stmt, NULL);
  throwOnSQLiteFailure(primaryDB);
  int retcode = 0;
  while ((retcode = sqlite3_step(stmt)) == SQLITE_ROW) {
    auto idString = reinterpret_cast<char const *>
               (sqlite3_column_text(stmt, 0));
    auto psBlob = reinterpret_cast<char const *>
                  (sqlite3_column_text(stmt, 1));
    ParameterSet pset;
    fhicl::make_ParameterSet(psBlob, pset);
    // Put into the registry without triggering ParameterSet::id().
    (void) registry.emplace(ParameterSetID(idString), pset).first;
  }
  sqlite3_finalize(stmt);
  throwOnSQLiteFailure(primaryDB);
}

fhicl::ParameterSetRegistry::
ParameterSetRegistry()
:
  primaryDB_(openPrimaryDB()),
  stmt_(nullptr),
  registry_()
{
}

auto
fhicl::ParameterSetRegistry::
find_(ParameterSetID const & id)
-> const_iterator
{
  const_iterator it = registry_.find(id);
  if (it == registry_.cend()) {
    // Look in primary DB for this ID and its contained IDs.
    if (stmt_ == nullptr) {
      sqlite3_prepare_v2(primaryDB_,
                         "SELECT PSetBlob FROM ParameterSets WHERE ID = ?;",
                         -1, &stmt_, NULL);
      throwOnSQLiteFailure(primaryDB_);
    }
    auto idString = id.to_string();
    auto result = sqlite3_bind_text(stmt_, 1, idString.c_str(),
                                    idString.size() + 1, SQLITE_STATIC);
    throwOnSQLiteFailure(primaryDB_);
    result = sqlite3_step(stmt_);
    switch (result) {
    case SQLITE_ROW: // Found the ID in the DB.
    {
      ParameterSet pset;
      fhicl::make_ParameterSet(reinterpret_cast<char const *>(sqlite3_column_text(stmt_, 0)),
                               pset);
      // Put into the registry without triggering ParameterSet::id().
      it = registry_.emplace(id, pset).first;
    }
    break;
    case SQLITE_DONE:
      break; // Not here.
    default:
      throwOnSQLiteFailure(primaryDB_);
    }
    sqlite3_reset(stmt_);
  }
  return it;
}
