include(CetMake)

cet_make_exec(fhicl-dump
  SOURCE fhicl-dump.cc
  LIBRARIES
  cetlib fhiclcpp ${Boost_PROGRAM_OPTIONS_LIBRARY}
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
