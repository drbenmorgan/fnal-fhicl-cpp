
execute_process( COMMAND date
                 OUTPUT_VARIABLE datime
                 OUTPUT_STRIP_TRAILING_WHITESPACE )
execute_process( COMMAND  ups flavor
                 OUTPUT_VARIABLE flavor
                 OUTPUT_STRIP_TRAILING_WHITESPACE )

configure_file ( ${fhiclcpp_SOURCE_DIR}/ups/fhiclcpp-config.cmake.in
                 ${fhiclcpp_BINARY_DIR}/ups/fhiclcpp-config.cmake @ONLY )
configure_file ( ${fhiclcpp_SOURCE_DIR}/ups/fhiclcpp-config-version.cmake.in
                 ${fhiclcpp_BINARY_DIR}/ups/fhiclcpp-config-version.cmake @ONLY )

CONFIGURE_FILE ( ${fhiclcpp_SOURCE_DIR}/ups/fhiclcpp.table.in
                 ${fhiclcpp_BINARY_DIR}/ups/fhiclcpp.table @ONLY )
CONFIGURE_FILE ( ${fhiclcpp_SOURCE_DIR}/ups/fhiclcpp.version.in
                 ${fhiclcpp_BINARY_DIR}/ups/${version}.version @ONLY )

install (FILES ${fhiclcpp_BINARY_DIR}/ups/fhiclcpp-config.cmake
               ${fhiclcpp_BINARY_DIR}/ups/fhiclcpp-config-version.cmake
         DESTINATION ${flavorqual_dir}/lib/fhiclcpp-${version} )

INSTALL (FILES ${fhiclcpp_BINARY_DIR}/ups/fhiclcpp.table
         DESTINATION ${version}/ups)
INSTALL (FILES ${fhiclcpp_BINARY_DIR}/ups/${version}.version
         DESTINATION ${CMAKE_INSTALL_PREFIX} )

