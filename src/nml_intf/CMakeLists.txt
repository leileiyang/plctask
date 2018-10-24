cmake_minimum_required (VERSION 2.6)

add_custom_command (
  OUTPUT
    ${PROJECT_SOURCE_DIR}/nml_intf/plc_nml.cc
  COMMAND java
    ARGS -jar ${PROJECT_SOURCE_DIR}/nml_intf/CodeGenCmdLine.jar update_with_name=true
    ${PROJECT_SOURCE_DIR}/nml_intf/plc_nml.hh -o ${PROJECT_SOURCE_DIR}/nml_intf/plc_nml.cc
  DEPENDS plc_nml.hh
  VERBATIM
  )

add_custom_target (
  generate-msg-cc
  DEPENDS ${PROJECT_SOURCE_DIR}/nml_intf/plc_nml.cc
  )

add_library(plcmsg SHARED interpl.cc plc_nml.cc)
add_dependencies (plcmsg generate-msg-cc)
