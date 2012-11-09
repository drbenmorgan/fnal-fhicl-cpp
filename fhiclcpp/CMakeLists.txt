
include(CetMake)

# define contents of interest:
set( includeFiles
  ParameterSet.h
  ParameterSetID.h
  ParameterSetRegistry.h
  coding.h
  exception.h
  extended_value.h
  fwd.h
  intermediate_table.h
  make_ParameterSet.h
  parse.h
  tokens.h
  type_traits.h
)

set( srcFiles
  ParameterSet.cc
  ParameterSetID.cc
  coding.cc
  exception.cc
  extended_value.cc
  intermediate_table.cc
  make_ParameterSet.cc
  parse.cc
)

if (qualifier STREQUAL "e1")
  set_source_files_properties(parse.cc PROPERTIES COMPILE_FLAGS -std=c++98)
endif()

# install public headers for users:
install( FILES ${includeFiles}
         DESTINATION ${product}/${version}/include/fhiclcpp
)

# install a copy of all source code:
install( FILES ${includeFiles} ${srcFiles}
         DESTINATION ${product}/${version}/source/fhiclcpp
)

cet_make_library( LIBRARY_NAME fhiclcpp
        	  SOURCE ${srcFiles}
        	  LIBRARIES ${CETLIB}
        	  WITH_STATIC_LIBRARY )

# Build and install executables:
add_executable( fhicl-expand fhicl-expand.cc )
target_link_libraries( fhicl-expand ${CETLIB} ${Boost_PROGRAM_OPTIONS_LIBRARY} )
install( TARGETS fhicl-expand
         DESTINATION ${flavorqual_dir}/bin
)

# ======================================================================
