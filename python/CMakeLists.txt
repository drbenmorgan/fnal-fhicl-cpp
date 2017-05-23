# - When user supplies "-DALT_CMAKE" use the non-CET/UPS system
if(ALT_CMAKE)
  include(altCMakeLists.cmake)
else()

include_directories($ENV{PYTHON_INCLUDE})
cet_make(LIBRARY_NAME fhicl LIBRARIES cetlib cetlib_except fhiclcpp ${PYTHON})
SET_TARGET_PROPERTIES(fhicl PROPERTIES PREFIX "")

endif() # ALT_CMAKE

