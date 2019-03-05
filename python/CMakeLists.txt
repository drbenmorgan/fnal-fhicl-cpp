set(PYTHON_MODULE_EXTENSION ".so")
find_package(PythonLibs REQUIRED)
find_package(pybind11 REQUIRED CONFIG)
include_directories($ENV{PYBIND11_DIR}/include)

include_directories($ENV{PYTHON_INCLUDE})
cet_make(LIBRARY_NAME fhicl LIBRARIES cetlib cetlib_except fhiclcpp ${PYTHON})
SET_TARGET_PROPERTIES(fhicl PROPERTIES PREFIX "" SUFFIX ".so")
