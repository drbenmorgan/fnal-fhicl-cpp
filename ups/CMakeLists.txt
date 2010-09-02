
CONFIGURE_FILE ( ${fhicl-cpp_SOURCE_DIR}/ups/fhiclcpp.table.in
                 ${fhicl-cpp_BINARY_DIR}/ups/fhiclcpp.table )

INSTALL (FILES
    ${fhicl-cpp_BINARY_DIR}/ups/fhiclcpp.table
    DESTINATION ups
    )

