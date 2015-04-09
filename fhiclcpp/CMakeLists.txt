include(CetMake)

add_subdirectory( detail )

cet_make(EXCLUDE fhicl-expand.cc fhicl-write-db.cc
  LIBRARIES
  cetlib
  ${SQLITE3}
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_REGEX_LIBRARY}
  WITH_STATIC_LIBRARY
  SUBDIRS detail
  )

cet_make_exec(fhicl-expand
  SOURCE fhicl-expand.cc
  LIBRARIES
   cetlib ${Boost_PROGRAM_OPTIONS_LIBRARY}
)

cet_make_exec(fhicl-write-db
  SOURCE fhicl-write-db.cc
  LIBRARIES
    fhiclcpp
    ${SQLITE3}
)

install_headers()
install_source()
