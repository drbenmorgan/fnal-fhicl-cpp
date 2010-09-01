
CONFIGURE_FILE ( ${FHiCLCpp_SOURCE_DIR}/ups/fhiclcpp.table.in
                 ${FHiCLCpp_BINARY_DIR}/ups/fhiclcpp.table )

INSTALL (FILES
    ${FHiCLCpp_BINARY_DIR}/ups/fhiclcpp.table
    DESTINATION ups
    )

