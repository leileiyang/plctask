cmake_minimum_required (VERSION 2.6)

file (GLOB NML_SOURCES
    "*.hh"
    "*.cc")

add_custom_command (
  OUTPUT
    ${PROJECT_SOURCE_DIR}/nml_intf/plc_nml.cc
  COMMAND java
    ARGS -jar ${PROJECT_SOURCE_DIR}/nml_intf/CodeGenCmdLine.jar update_with_name=fase
    ${PROJECT_SOURCE_DIR}/nml_intf/plc_nml.hh -o ${PROJECT_SOURCE_DIR}/nml_intf/plc_nml.cc
  VERBATIM
  )

add_custom_target (
  generate-msg-cc
  DEPENDS ${PROJECT_SOURCE_DIR}/nml_intf/plc_nml.cc
  )

add_library(plcmsg SHARED ${NML_SOURCES} plc_nml.cc)
add_dependencies (plcmsg generate-msg-cc)
