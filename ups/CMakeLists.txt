
process_ups_files()

configure_file ( ${CMAKE_CURRENT_SOURCE_DIR}/fhiclcpp-config.cmake.in
                 ${CMAKE_CURRENT_BINARY_DIR}/fhiclcpp-config.cmake @ONLY )
configure_file ( ${CMAKE_CURRENT_SOURCE_DIR}/fhiclcpp-config-version.cmake.in
                 ${CMAKE_CURRENT_BINARY_DIR}/fhiclcpp-config-version.cmake @ONLY )

install (FILES ${CMAKE_CURRENT_BINARY_DIR}/fhiclcpp-config.cmake
               ${CMAKE_CURRENT_BINARY_DIR}/fhiclcpp-config-version.cmake
         DESTINATION ${flavorqual_dir}/lib/fhiclcpp-${version} )
