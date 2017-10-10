# - When user supplies "-DALT_CMAKE" use the non-CET/UPS system
if(ALT_CMAKE)
  include(altCMakeLists.cmake)
else()

include(CetMake)

cet_make(
  LIBRARIES
  cetlib
  cetlib_except
  ${SQLITE3}
  WITH_STATIC_LIBRARY
  SUBDIRS
  detail
  types
  types/detail
  )

install_headers(SUBDIRS detail types types/detail)
install_source(SUBDIRS detail types types/detail)
# Testing
add_subdirectory(test)

endif() # ALT_CMAKE
