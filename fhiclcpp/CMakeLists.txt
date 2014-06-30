include(CetMake)

cet_make(EXCLUDE fhicl-expand.cc
  LIBRARIES
  ${CETLIB}
  ${SQLITE3}
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_REGEX_LIBRARY}
  WITH_STATIC_LIBRARY
  )

cet_make_exec(fhicl-expand
  SOURCE fhicl-expand.cc
  LIBRARIES
   ${CETLIB} ${Boost_PROGRAM_OPTIONS_LIBRARY}
)

install_headers()
install_source()
