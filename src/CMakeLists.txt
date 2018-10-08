cmake_minimum_required (VERSION 2.6)
project (plctask)
include_directories ("${PROJECT_SOURCE_DIR}")
include_directories ("${PROJECT_SOURCE_DIR}/nml_intf")
include_directories ("${PROJECT_SOURCE_DIR}/plcclient")

message ("sourse dir:" ${PROJECT_SOURCE_DIR})

add_executable (plctask plctask.cc plctaskmain.cc)
target_link_libraries (plctask plcmsg rcs)

add_executable (plcsvr plcsvr.cc)
target_link_libraries (plcsvr plcmsg rcs)

add_subdirectory ("${PROJECT_SOURCE_DIR}/nml_intf")
add_subdirectory ("${PROJECT_SOURCE_DIR}/plcclient")
add_subdirectory ("${PROJECT_SOURCE_DIR}/gcode")

add_custom_command (
    TARGET plcsvr
    PRE_BUILD
    COMMAND java
    ARGS -jar ${PROJECT_SOURCE_DIR}/nml_intf/CodeGenCmdLine.jar update_with_name=false ${PROJECT_SOURCE_DIR}/nml_intf/plc_nml.hh -o ${PROJECT_SOURCE_DIR}/nml_intf/plc_nml.cc)
