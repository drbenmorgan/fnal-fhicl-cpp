# - Build Python wrapping for fhiclcpp
# This is the absolute minimum, so note the following:
# 1) Should the library type be MODULE?
# 2) Should the module link to libpython?
# 3) Where to install (CBT installs alongside the main lib)
# 4) See also the comments in the top level altCMakeLists.cmake file
#    on matching Python/libpython (that's mostly for testing purposes).
# 5) Could this be implemented as a setup.py system with CMake
#    as driver?
add_library(fhicl SHARED fhiclmodule.cc)
target_include_directories(fhicl PUBLIC ${PYTHON_INCLUDE_DIRS})
target_link_libraries(fhicl fhiclcpp ${PYTHON_LIBRARIES})

# Change {pre,suf}fix of module so it's importable (UNIX only)
set_target_properties(fhicl
  PROPERTIES
    PREFIX ""
    SUFFIX ".so"
    )

