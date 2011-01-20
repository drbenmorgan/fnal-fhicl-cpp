
execute_process( COMMAND date
                 OUTPUT_VARIABLE datime
                 OUTPUT_STRIP_TRAILING_WHITESPACE )

configure_file ( ${CMAKE_CURRENT_SOURCE_DIR}/fhiclcpp-config.cmake.in
                 ${CMAKE_CURRENT_BINARY_DIR}/fhiclcpp-config.cmake @ONLY )
configure_file ( ${CMAKE_CURRENT_SOURCE_DIR}/fhiclcpp-config-version.cmake.in
                 ${CMAKE_CURRENT_BINARY_DIR}/fhiclcpp-config-version.cmake @ONLY )

configure_file ( ${CMAKE_CURRENT_SOURCE_DIR}/fhiclcpp.table.in
                 ${CMAKE_CURRENT_BINARY_DIR}/fhiclcpp.table @ONLY )
configure_file ( ${CMAKE_CURRENT_SOURCE_DIR}/fhiclcpp.version.in
                 ${CMAKE_CURRENT_BINARY_DIR}/${UPSFLAVOR}_${qualifier} @ONLY )

install (FILES ${CMAKE_CURRENT_BINARY_DIR}/fhiclcpp-config.cmake
               ${CMAKE_CURRENT_BINARY_DIR}/fhiclcpp-config-version.cmake
         DESTINATION ${flavorqual_dir}/lib/fhiclcpp-${version} )

install (FILES ${CMAKE_CURRENT_BINARY_DIR}/fhiclcpp.table
         DESTINATION ${product}/${version}/ups)
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/${UPSFLAVOR}_${qualifier}
         DESTINATION ${product}/${version}.version )

