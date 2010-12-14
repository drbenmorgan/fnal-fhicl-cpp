# define contents of interest:
set( includeFiles
  ParameterSet.h
  ParameterSetID.h
  ParameterSetRegistry.h
  exception.h
  extended_value.h
  intermediate_table.h
  make_ParameterSet.h
  parse.h
  tokens.h
  type_traits.h
)

set( srcFiles
  ParameterSet.cc
  ParameterSetID.cc
  ParameterSetRegistry.cc
  exception.cc
  extended_value.cc
  intermediate_table.cc
  make_ParameterSet.cc
  parse.cc
)

# install public headers for users:
install( FILES ${includeFiles}
         DESTINATION ${product}/${version}/include/fhiclcpp
)

# install a copy of all source code:
#file( GLOB hFiles *.h )
#file( GLOB ccFiles *.cc )
install( FILES ${includeFiles} ${srcFiles}
         DESTINATION ${product}/${version}/source/fhiclcpp
)

# build shared library:
add_library( fhiclcppD SHARED ${srcFiles} )
target_link_libraries( fhiclcppD cetlib )
set_target_properties( fhiclcppD PROPERTIES OUTPUT_NAME fhiclcpp )

# build static library:
add_library (fhiclcpp  STATIC ${srcFiles})
target_link_libraries (fhiclcpp cetlib )
set_target_properties (fhiclcpp PROPERTIES OUTPUT_NAME fhiclcpp )

SET_TARGET_PROPERTIES(fhiclcppD PROPERTIES CLEAN_DIRECT_OUTPUT 1)
SET_TARGET_PROPERTIES(fhiclcpp  PROPERTIES CLEAN_DIRECT_OUTPUT 1)

# install libraries:
install( TARGETS fhiclcppD fhiclcpp
         RUNTIME DESTINATION ${flavorqual_dir}/bin
         LIBRARY DESTINATION ${flavorqual_dir}/lib
         ARCHIVE DESTINATION ${flavorqual_dir}/lib
)
