cmake_minimum_required (VERSION 2.6)
project (plctask)
include_directories ("${PROJECT_SOURCE_DIR}")
include_directories ("${PROJECT_SOURCE_DIR}/nml_intf")
message ("sourse dir:" ${PROJECT_SOURCE_DIR})

# collect the files in nml_intf directory
file (GLOB NML_SOURCES
    "nml_intf/*.hh"
    "nml_intf/*.cc")

add_executable (plctask plctask.cc plctaskmain.cc ${NML_SOURCES})
target_link_libraries (plctask rcs)

add_executable (plcsvr plcsvr.cc ${NML_SOURCES})
target_link_libraries (plcsvr rcs)

add_custom_command (
    TARGET plcsvr
    PRE_BUILD
    COMMAND java
    ARGS -jar ${PROJECT_SOURCE_DIR}/nml_intf/CodeGenCmdLine.jar update_with_name=false ${PROJECT_SOURCE_DIR}/nml_intf/plc_nml.hh -o ${PROJECT_SOURCE_DIR}/nml_intf/plc_nml.cc)
