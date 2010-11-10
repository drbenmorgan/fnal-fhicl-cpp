# define contents of interest:
set ( includeFiles
    exception.h
    type_traits.h
    ParameterSet.h
    ParameterSetID.h
    ParameterSetParser.h
    ParameterSetRegistry.h
    Parser.h
    PSet.h
    )

set (srcFiles
    exception.cc
    ParameterSet.cc
    ParameterSetID.cc
    ParameterSetParser.cc
    ParameterSetRegistry.cc
    )

# install public headers for users:
install (FILES ${includeFiles}
    DESTINATION ${product}/${version}/include/fhiclcpp
    )

# install a copy of all source code:
file (GLOB hFiles *.h)
file (GLOB ccFiles *.cc)
install (FILES ${hFiles} ${ccFiles}
    DESTINATION ${product}/${version}/source/fhiclcpp
    )

# build shared library:
add_library (fhiclcppD SHARED ${srcFiles})
target_link_libraries (fhiclcppD
    cetlib
    )
set_target_properties (fhiclcppD
    PROPERTIES OUTPUT_NAME fhiclcpp
    )

# build static library:
add_library (fhiclcpp  STATIC ${srcFiles})
target_link_libraries (fhiclcpp
    cetlib
    )
set_target_properties (fhiclcpp
    PROPERTIES OUTPUT_NAME fhiclcpp
    )

SET_TARGET_PROPERTIES(fhiclcppD PROPERTIES CLEAN_DIRECT_OUTPUT 1)
SET_TARGET_PROPERTIES(fhiclcpp  PROPERTIES CLEAN_DIRECT_OUTPUT 1)

# install libraries:
install (TARGETS fhiclcppD fhiclcpp
    RUNTIME DESTINATION ${flavorqual_dir}/bin
    LIBRARY DESTINATION ${flavorqual_dir}/lib
    ARCHIVE DESTINATION ${flavorqual_dir}/lib
    )
