cmake_minimum_required(VERSION 3.3)
project(fhiclcpp VERSION 4.6.5)

# cetbuildtools contains our cmake modules
# - Cetbuildtools, version2
find_package(cetbuildtools2 0.4.0 REQUIRED)
set(CMAKE_MODULE_PATH ${cetbuildtools2_MODULE_PATH})
set(CET_COMPILER_CXX_STANDARD_MINIMUM 14)
include(CetInstallDirs)
include(CetCMakeSettings)
include(CetCompilerSettings)


# these are minimum required versions, not the actual product versions
find_package(cetlib 3.0.0 REQUIRED)
find_package(cetlib_except 1.0.0 REQUIRED)
find_package(Boost 1.60 REQUIRED unit_test_framework program_options)
find_package(SQLite 3.7.15 REQUIRED)

# Here be dragons - we must match the python interpreter and library
# otherwise "import fhicl" will give a
#   Fatal Python error: PyThreadState_Get: no current thread
#   Abort trap: 6
# error. Docs for FindPythonInterp/Libs say to find the interpreter
# first to have consistency(?).
find_package(PythonInterp 2.7 REQUIRED)
find_package(PythonLibs 2.7 REQUIRED)

# source code
add_subdirectory(fhiclcpp)
add_subdirectory(python)
add_subdirectory(tools)

#-----------------------------------------------------------------------
# Documentation
#
find_package(Doxygen 1.8)
if(DOXYGEN_FOUND)
  set(DOXYGEN_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/Doxygen")
  configure_file(Doxyfile.in Doxyfile @ONLY)
  add_custom_command(
    OUTPUT "${DOXYGEN_OUTPUT_DIR}/html/index.html"
    COMMAND "${DOXYGEN_EXECUTABLE}" "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile"
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile" fhiclcpp
    COMMENT "Generating Doxygen docs for ${PROJECT_NAME}"
    )
  add_custom_target(doc ALL DEPENDS "${DOXYGEN_OUTPUT_DIR}/html/index.html")
  install(DIRECTORY "${DOXYGEN_OUTPUT_DIR}/"
    DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/API"
    )
endif()

# Packaging
# TODO
#include(UseCPack)
