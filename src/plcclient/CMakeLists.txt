include_directories ("${PROJECT_SOURCE_DIR}/nml_intf")
include_directories ("${PROJECT_SOURCE_DIR}/plcclient")
add_executable (plcclient main.cc plcclient.cc sendmsgengine.cc)
target_link_libraries (plcclient plcmsg pthread)
