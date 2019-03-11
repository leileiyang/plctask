cmake_minimum_required (VERSION 2.6)

add_custom_command (
  OUTPUT
    ${PROJECT_SOURCE_DIR}/nml_intf/plc_nml.cc
  COMMAND java
    ARGS -jar ${PROJECT_SOURCE_DIR}/nml_intf/CodeGenCmdLine.jar update_with_name=true
    ${PROJECT_SOURCE_DIR}/nml_intf/plc_nml.h -o ${PROJECT_SOURCE_DIR}/nml_intf/plc_nml.cc
  DEPENDS plc.h plc_nml.h
  VERBATIM
  )

add_custom_target (
  generate-msg-cc
  DEPENDS ${PROJECT_SOURCE_DIR}/nml_intf/plc_nml.cc
  )

add_library(plcmsg SHARED interpl.cc plc_nml.cc)
add_dependencies (plcmsg generate-msg-cc)

install (TARGETS plcmsg DESTINATION /usr/local/lib
  PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
              GROUP_READ GROUP_EXECUTE
              WORLD_READ WORLD_EXECUTE)

file (GLOB PLC_HEADERS
  "*.h"
  "*.hh")

install (FILES ${PLC_HEADERS} DESTINATION /usr/local/include/plcmsg)
