configure_file ( ${fhiclcpp_SOURCE_DIR}/ups/fhiclcpp-config.cmake.in
                 ${fhiclcpp_BINARY_DIR}/ups/fhiclcpp-config.cmake @ONLY )
configure_file ( ${fhiclcpp_SOURCE_DIR}/ups/fhiclcpp-config-version.cmake.in
                 ${fhiclcpp_BINARY_DIR}/ups/fhiclcpp-config-version.cmake @ONLY )

CONFIGURE_FILE ( ${fhiclcpp_SOURCE_DIR}/ups/fhiclcpp.table.in
                 ${fhiclcpp_BINARY_DIR}/ups/fhiclcpp.table @ONLY )

install (FILES ${fhiclcpp_BINARY_DIR}/ups/fhiclcpp-config.cmake
               ${fhiclcpp_BINARY_DIR}/ups/fhiclcpp-config-version.cmake
         DESTINATION lib/fhiclcpp-${version} )

INSTALL (FILES
    ${fhiclcpp_BINARY_DIR}/ups/fhiclcpp.table
    DESTINATION ups
    )

