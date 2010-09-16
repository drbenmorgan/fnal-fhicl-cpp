
FILE (GLOB includeFiles
    ${fhiclcpp_SOURCE_DIR}/interface/*.h
    )

# installing headers for users
INSTALL (FILES ${includeFiles}
    DESTINATION ${version}/include/fhiclcpp
    )

# making a copy of the source code
INSTALL (FILES ${includeFiles} CMakeLists.txt
    DESTINATION ${version}/src/fhiclcpp/interface
    )
