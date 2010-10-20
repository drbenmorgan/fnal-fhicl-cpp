set( includeFiles
                  PSet.h
                  ParameterSet.h
                  ParameterSetID.h
                  ParameterSetRegistry.h
                  Parser.h
                  type_traits.h
    )

# installing public headers for users
install (FILES ${includeFiles}
    DESTINATION ${version}/include/fhiclcpp
    )

# install a copy of all source code
file (GLOB hFiles *.h)
file (GLOB ccFiles *.cc)
install (FILES ${hFiles} ${ccFiles}
    DESTINATION ${version}/source/fhiclcpp
    )

set (srcFiles
    ParameterSet.cc
    ParameterSetID.cc
    ParameterSetParser.cc
    )

add_library (fhiclcppD SHARED ${srcFiles})
add_library (fhiclcpp         ${srcFiles})

target_link_libraries (fhiclcppD
    cetlib
    )

target_link_libraries (fhiclcpp
    cetlib
    )

set_target_properties (fhiclcppD
    PROPERTIES OUTPUT_NAME fhiclcpp
    )

set_target_properties (fhiclcpp
    PROPERTIES OUTPUT_NAME fhiclcpp
    )

install (TARGETS fhiclcppD fhiclcpp
    RUNTIME DESTINATION ${flavorqual_dir}/bin
    LIBRARY DESTINATION ${flavorqual_dir}/lib
    ARCHIVE DESTINATION ${flavorqual_dir}/lib
    )
