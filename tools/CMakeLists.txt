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

# Install
install(TARGETS fhicl-dump fhicl-expand fhicl-write-db
  EXPORT ${PROJECT_NAME}Targets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  )

