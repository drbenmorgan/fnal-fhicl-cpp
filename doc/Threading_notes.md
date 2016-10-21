The ParameterSetRegistry class follows the Singleton pattern and is intended
to store information relevant to the entire art job in progress.

----------------------------------------------------------------
Interface of ParameterSetRegistry in pseudo-UML


class fhicl::detail::HashParameterSetID

  - hash_ : std::hash<std::string> hash_

  -----

  + operator()(ParameterSetID const& id) const : size_t


class fhicl::ParameterSetRegistry

  - primaryDB_ : sqlite3*
  - stmt_ : sqlite3_stmt*
  - registry_ : std::unordered_map<ParameterSetID, ParameterSet, detail::HashParameterSetID>

  -----

  + typedef collection_type : std::unordered_map<ParameterSetID, ParameterSet, detail::HashParameterSetID>
  + typedef key_type : collection_type::key_type
  + typedef mapped_type : collection_type::mapped_type
  + typedef value_type : collection_type::value_type
  + typedef size_type : collection_type::size_type
  + typedef const_iterator : collection_type::const_iterator

  -----

  + ~ParameterSetRegistry();
  - ParameterSetRegistry()
  + ParameterSetRegistry(ParameterSet const&) = delete;
  + ParameterSetRegistry(ParameterSet&&) = delete;
  + operator=(ParameterSet const&) = delete : ParameterSetRegistry&
  + operator=(ParameterSet&&) = delete : ParameterSetRegistry& 
  - find_(ParameterSetID const& id) : const_iterator
  - static instance_() : ParameterSetRegistry&
  + static importFrom(sqlite3* db) : void
  + static exportTo(sqlite3* db) : void
  + static stageIn() : void
  + static empty() : bool
  + static size() : size_type
  + static begin() : const_iterator
  + static end() : const_iterator
  + static cbegin() : const_iterator
  + static cend() : const_iterator
  + static get() noexcept : collection_type const& 
  + static get(ParameterSetID const& id) : ParameterSet const& 
  + static get(ParameterSetID const& id, ParameterSet& ps) : bool
  + static put(collection_type const& c) : void
  + static put(ParameterSet const& ps) : ParameterSetID const& 

  + template<typename FwdIt>
    static
    put(FwdIt begin, FwdIt end) : std::enable_if_t<std::is_same<typename std::iterator_traits<FwdIt>::value_type, mapped_type>::value>

  + template<typename FwdIt>
    static
    put(FwdIt begin, FwdIt end) : std::enable_if_t<std::is_same<typename std::iterator_traits<FwdIt>::value_type, value_type>::value>



----------------------------------------------------------------
Usage in art of the interface


// config_dumper use (standalone executable, no threading issues)
./art/art/Framework/IO/Root/config_dumper.cc:192:    fhicl::ParameterSetRegistry::put(pset);
./art/art/Framework/IO/Root/config_dumper.cc:201:    fhicl::ParameterSetRegistry::importFrom(sqliteDB);
./art/art/Framework/IO/Root/config_dumper.cc:202:    fhicl::ParameterSetRegistry::stageIn();
./art/art/Framework/IO/Root/config_dumper.cc:231:  for (auto const& pr : fhicl::ParameterSetRegistry::get())

// importFrom
./art/art/Framework/IO/Root/RootInputFile.cc:231:        fhicl::ParameterSetRegistry::importFrom(sqliteDB_);

// exportTo
./art/art/Framework/IO/Root/RootOutputFile.cc:837:  fhicl::ParameterSetRegistry::exportTo(rootFileDB_);
./fhiclcpp/fhiclcpp/DatabaseSupport.cc:57:  fhicl::ParameterSetRegistry::exportTo(out);

// get
./art/art/Framework/Services/System/TriggerNamesService_service.cc:56:  if (! ParameterSetRegistry::get(triggerResults.parameterSetID(), pset))
./art/art/Framework/Services/UserInteraction/UserInteraction.cc:46:      fhicl::ParameterSetRegistry::get(md.parameterSetID(),
./art/art/Framework/Principal/Provenance.cc:20:  return fhicl::ParameterSetRegistry::get( *psetIDs().begin() );
./art/art/Framework/Principal/Event.cc:101:      ParameterSetRegistry::get(config.parameterSetID(), ps);
./art/art/Framework/Principal/Run.cc:41:      ParameterSetRegistry::get(id, temp);
./art/art/Framework/Core/OutputWorker.cc:20:                               fhicl::ParameterSetRegistry::get(description().parameterSetID()));
./art/art/Framework/Core/detail/get_failureToPut_flag.cc:11:    auto const& main_pset   = fhicl::ParameterSetRegistry::get( md.mainParameterSetID() );
./art/art/Framework/Core/detail/get_failureToPut_flag.cc:12:    auto const& pset        = fhicl::ParameterSetRegistry::get( md.parameterSetID() );
./fhiclcpp/fhiclcpp/ParameterSet.cc:57:    return ParameterSetRegistry::get(psid);
./fhiclcpp/fhiclcpp/ParameterSet.cc:71:    result = '{' + ParameterSetRegistry::get(psid).to_string() + '}';
./fhiclcpp/fhiclcpp/coding.cc:215:  result = ParameterSetRegistry::get(id);

// put
./art/art/Framework/Services/System/TriggerNamesService_service.cc:36:  ParameterSetRegistry::put(trigger_pset_);
./art/art/Framework/Art/run_art.cc:134:    fhicl::ParameterSetRegistry::put(main_pset);
./art/art/Framework/Art/run_art.cc:179:    fhicl::ParameterSetRegistry::put(main_pset);
./art/art/Framework/EventProcessor/ServiceDirector.cc:22:    fhicl::ParameterSetRegistry::put(tmp);
./art/art/Framework/IO/Root/RootInputFile.cc:224:      fhicl::ParameterSetRegistry::put(pset);
./fhiclcpp/fhiclcpp/DatabaseSupport.cc:56:  fhicl::ParameterSetRegistry::put(top);
./fhiclcpp/fhiclcpp/coding.cc:126:  return ParameterSetRegistry::put(value);
./fhiclcpp/fhiclcpp/detail/encode_extended_value.cc:54:    return ParameterSetRegistry::put(result);


----------------------------------------------------------------

inline
auto
fhicl::ParameterSetRegistry::
ParameterSetRegistry::instance_() -> ParameterSetRegistry&
{
  static ParameterSetRegistry s_registry;
  return s_registry;
}


The instance_() private static member function manages a function-local static
variable s_registry which is the process-wide single instance of the
ParameterSetRegistry class.  This means the private default constructor with
no arguments is run the first time the instance_() function is executed.

It is worth noting that the C++ Standard guarantees that function local static
initialization is thread safe and that g++ does correctly implement this
requirement using a critical section.

The first time the instance_() function is run appears to be in art::run_art()
when the parameter set created from the intermediate table is put into the
registry, but this is probably incorrect due to the need to assign parameter set
ids when creating tables.  This will need further chasing.

THREADING - Point of construction of the ParameterSetRegistry singleton instance.

fhicl::ParameterSetRegistry::
ParameterSetRegistry()
  : primaryDB_(openPrimaryDB())
  , stmt_(nullptr)
  , registry_()
{
}

The private default constructor with no arguments default initializes the
registry_ data member, sets stmt_ to the nullptr, and calls openPrimaryDB()
to initialize the primaryDB_ data member.

The openPrimaryDB() function is in an unnamed namespace.  It calls sqlite3_open()
to create an in-memory database, then calls sqlite3_exec() to create the
ParameterSets table.  On failure of either step throwOnSQLiteFailure() is called.
The creation of the ParameterSets table is done in a transaction so either it
appears, or on error it does not, there will never be a partially created table.

The throwOnSQLiteFailure() function is passed a local database pointer and a local
pointer to the sql error message which was created by sqlite3.  It copies the
message and uses sqlite3_free to release the message storage.  If the database
pointer is the nullptr it throws a fhicl::exception.  Otherwise it calls
sqlite3_errcode to retrieve the pending error code from sqlite and then uses
sqlite3_errstr to translate the code to a message.  Then it calls sqlite3_errmsg
passing it the local database pointer to retrieve further information.  Then it
throws a fhicl::exception.

THREADING - ParameterSetRegistry construction is not thread safe.

The uses of sqlite3 interfaces in the ParameterSetRegistry private default
constructor with no arguments form a transaction that must not be interleaved
with another attempted construction.  Furthermore the sqlite3 product is probably
not thread safe in and of itself and so construction of the ParameterSetRegistry
should not be interleaved with any other uses of the sqlite3 interfaces.
Careful thought must also be given to the interaction of exception handling with
the thread tasking system in use.


THREADING - Nearly all ParameterSetRegistry interfaces use instance_() and so may result in construction of the singleton instance.

