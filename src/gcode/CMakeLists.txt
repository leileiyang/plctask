include_directories ("${PROJECT_SOURCE_DIR}/nml_intf")
add_executable (gcodetest main.cc)
target_link_libraries (gcodetest plcmsg rcs)
