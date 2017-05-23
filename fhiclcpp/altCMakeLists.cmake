# - Create dynamic/Static lib targets
add_library(fhiclcpp SHARED "")
add_library(fhiclcpp-static STATIC "")
set_target_properties(fhiclcpp-static PROPERTIES OUTPUT_NAME fhiclcpp)

# - Apply source lists to libraries
set(fhiclcpp_SOURCES
  DatabaseSupport.cc
  DatabaseSupport.h
  ParameterSet.cc
  ParameterSet.h
  ParameterSetID.cc
  ParameterSetID.h
  ParameterSetRegistry.cc
  ParameterSetRegistry.h
  ParameterSetWalker.h
  Protection.cc
  Protection.h
  coding.cc
  coding.h
  exception.cc
  exception.h
  extended_value.cc
  extended_value.h
  fwd.h
  intermediate_table.cc
  intermediate_table.h
  make_ParameterSet.cc
  make_ParameterSet.h
  parse.cc
  parse.h
  parse_shims.cc
  parse_shims.h
  parse_shims_opts.h
  stdmap_shims.h
  tokens.h
  type_traits.h
  detail/Indentation.h
  detail/KeyAssembler.cc
  detail/KeyAssembler.h
  detail/ParameterSetImplHelpers.h
  detail/Prettifier.cc
  detail/Prettifier.h
  detail/PrettifierAnnotated.cc
  detail/PrettifierAnnotated.h
  detail/PrettifierPrefixAnnotated.cc
  detail/PrettifierPrefixAnnotated.h
  detail/binding_modifier.h
  detail/encode_extended_value.cc
  detail/encode_extended_value.h
  detail/print_mode.h
  detail/printing_helpers.cc
  detail/printing_helpers.h
  detail/try_blocks.h
  types/Atom.h
  types/Comment.h
  types/ConfigPredicate.h
  types/DelegatedParameter.cc
  types/DelegatedParameter.h
  types/KeysToIgnore.h
  types/Name.cc
  types/Name.h
  types/OptionalAtom.h
  types/OptionalDelegatedParameter.cc
  types/OptionalDelegatedParameter.h
  types/OptionalSequence.h
  types/OptionalTable.h
  types/OptionalTuple.h
  types/OptionalTupleAs.h
  types/Sequence.h
  types/Table.h
  types/TableFragment.h
  types/Tuple.h
  types/TupleAs.h
  types/detail/Atom.icc
  types/detail/AtomBase.h
  types/detail/DelegateBase.h
  types/detail/MaybeDisplayParent.cc
  types/detail/MaybeDisplayParent.h
  types/detail/NameStackRegistry.cc
  types/detail/NameStackRegistry.h
  types/detail/OptionalAtom.icc
  types/detail/OptionalSequence.icc
  types/detail/OptionalTable.icc
  types/detail/ParameterArgumentTypes.h
  types/detail/ParameterBase.h
  types/detail/ParameterMetadata.h
  types/detail/ParameterWalker.h
  types/detail/PrintAllowedConfiguration.cc
  types/detail/PrintAllowedConfiguration.h
  types/detail/SearchAllowedConfiguration.cc
  types/detail/SearchAllowedConfiguration.h
  types/detail/SeqVectorBase.h
  types/detail/Sequence.icc
  types/detail/SequenceBase.h
  types/detail/Table.icc
  types/detail/TableBase.h
  types/detail/TableMemberRegistry.h
  types/detail/ValidateThenSet.cc
  types/detail/ValidateThenSet.h
  types/detail/optional_parameter_message.h
  types/detail/ostream_helpers.cc
  types/detail/ostream_helpers.h
  types/detail/strip_containing_names.cc
  types/detail/strip_containing_names.h
  types/detail/type_traits_error_msgs.h
  types/detail/validationException.h
  )

# - Set target sources. Done in one go here for clarity and
# simplicity, but note how we could recurse into/include
# subdirs and add sources step by step.
# NB: ANother alternative is to build each sudbir as an OBJECT
# library, then group.
target_sources(fhiclcpp PRIVATE ${fhiclcpp_SOURCES})
target_sources(fhiclcpp-static PRIVATE ${fhiclcpp_SOURCES})

# - Define Include/Link Interfaces
target_include_directories(fhiclcpp
  PUBLIC
   $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
   )
target_link_libraries(fhiclcpp PUBLIC
  cetlib::cetlib
  SQLite::SQLite
  Boost::boost
  )

# - Static
target_include_directories(fhiclcpp-static
  PUBLIC
   $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
   )
target_link_libraries(fhiclcpp-static PUBLIC
  cetlib::cetlib
  SQLite::SQLite
  Boost::boost
  )

# - Install
# Targets
install(TARGETS fhiclcpp fhiclcpp-static
  EXPORT ${PROJECT_NAME}Targets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  )

# Headers
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/"
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}"
  FILES_MATCHING PATTERN "*.h" PATTERN "*.icc"
  PATTERN "test" EXCLUDE
  )

# Support files
#-----------------------------------------------------------------------
# Create exports file(s)
include(CMakePackageConfigHelpers)

# - Common to both trees
write_basic_package_version_file(
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  COMPATIBILITY SameMajorVersion
  )

# - Build tree (EXPORT only for now, config file needs some thought,
#   dependent on the use of multiconfig)
export(
  EXPORT ${PROJECT_NAME}Targets
  NAMESPACE ${PROJECT_NAME}::
  FILE "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake"
  )

# - Install tree
configure_package_config_file("${PROJECT_SOURCE_DIR}/${PROJECT_NAME}Config.cmake.in"
  "${PROJECT_BINARY_DIR}/InstallCMakeFiles/${PROJECT_NAME}Config.cmake"
  INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
  PATH_VARS CMAKE_INSTALL_INCLUDEDIR
  )

install(
  FILES
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    "${PROJECT_BINARY_DIR}/InstallCMakeFiles/${PROJECT_NAME}Config.cmake"
  DESTINATION
    "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
    )

install(
  EXPORT ${PROJECT_NAME}Targets
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
  )

# Testing
if(BUILD_TESTING)
  add_subdirectory(test)
endif()

