# - Build Python wrapping for fhiclcpp
# This is the absolute minimum, so note the following:
# 1) Where to install (CBT installs alongside the main lib)
#    Should site-packages be used? But don't want to install
#    outside CMAKE_INSTALL_PREFIX!
# 2) See also the comments in the top level altCMakeLists.cmake file
#    on matching Python/libpython (that's mostly for testing purposes).
# Use MODULE type because this is a true plugin
# Also, because of CET no-undefined symbol policy for shared libs, have an
# otherwise awkward workaround of setting/unsetting CMAKE_SHARED_LINKER_FLAGS.
add_library(fhicl MODULE fhiclmodule.cc)
target_include_directories(fhicl PRIVATE ${PYTHON_INCLUDE_DIRS})
# Don't link to libpython, see for why (also applies to linux):
# http://blog.tim-smith.us/2015/09/python-extension-modules-os-x/
# ... but make sure we use dynamic lookup for symbols on macOS
target_link_libraries(fhicl PRIVATE fhiclcpp)
target_link_libraries(fhicl PRIVATE "$<$<PLATFORM_ID:Darwin>:-undefined dynamic_lookup>")

# Change {pre,suf}fix of module so it's importable (UNIX only)
set_target_properties(fhicl PROPERTIES PREFIX "" SUFFIX ".so")

# Match UPS install, and install module alongside C++ library
# Ideally, should derive a suitable "site-packages" path from compiled against
# Python version, e.g. "lib/pythonX.Y/(site|dist)-packages" Should be under install
# prefix for relocatability.
# Some Homebrew, but reasonably general, advice here:
# https://docs.brew.sh/Python-for-Formula-Authors.html
# and also here for a calling-python method
# https://stackoverflow.com/questions/1242904/finding-python-site-packages-directory-with-cmake
install(TARGETS fhicl DESTINATION ${CMAKE_INSTALL_LIBDIR})

