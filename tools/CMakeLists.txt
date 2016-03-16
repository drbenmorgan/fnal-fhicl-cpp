#-----------------------------------------------------------------------
# FHICL programs

add_executable(fhicl-dump fhicl-dump.cc)
target_link_libraries(fhicl-dump PRIVATE
  fhiclcpp
  cetlib::cetlib
  ${Boost_PROGRAM_OPTIONS_LIBRARY}
  )

add_executable(fhicl-expand fhicl-expand.cc)
target_link_libraries(fhicl-expand PRIVATE
  cetlib::cetlib
  ${Boost_PROGRAM_OPTIONS_LIBRARY}
  )

add_executable(fhicl-write-db fhicl-write-db.cc)
target_link_libraries(fhicl-write-db PRIVATE
  fhiclcpp
  SQLite::SQLite
  )



#include(CetMake)

#cet_make_exec(fhicl-dump
#  SOURCE fhicl-dump.cc
#  LIBRARIES
#  cetlib fhiclcpp ${Boost_PROGRAM_OPTIONS_LIBRARY}
#  )
#
#cet_make_exec(fhicl-expand
#  SOURCE fhicl-expand.cc
#  LIBRARIES
#  cetlib ${Boost_PROGRAM_OPTIONS_LIBRARY}
#  )

#cet_make_exec(fhicl-write-db
#  SOURCE fhicl-write-db.cc
#  LIBRARIES
#  fhiclcpp
#  ${SQLITE3}
#)

#install_headers()
#install_source()
