# - When user supplies "-DALT_CMAKE" use the non-CET/UPS system
if(ALT_CMAKE)
  include(altCMakeLists.cmake)
else()

include(CetMake)

add_subdirectory(detail types)

cet_make(
  LIBRARIES
  cetlib
  ${SQLITE3}
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_REGEX_LIBRARY}
  WITH_STATIC_LIBRARY
  SUBDIRS
  detail
  types
  types/detail
  )

install_headers(SUBDIRS types types/detail)
install_source(SUBDIRS types types/detail)
# Testing
add_subdirectory(test)

endif() # ALT_CMAKE
