cmake_minimum_required (VERSION 2.6)
project (plctask)
include_directories ("${PROJECT_SOURCE_DIR}")
include_directories ("${PROJECT_SOURCE_DIR}/nml_intf")
include_directories ("${PROJECT_SOURCE_DIR}/plcclient")

# collect the files in nml_intf directory
#include (nml_intf/CMakeLists.txt)
#foreach (FILE ${FILES})
#  set (nml_intf_files ${nml_intf_files} nml_intf/${FILE})
#endforeach (FILE)
add_subdirectory ("${PROJECT_SOURCE_DIR}/nml_intf")

add_executable (plctask plctask.cc plctaskmain.cc)
target_link_libraries (plctask plcmsg)

add_subdirectory ("${PROJECT_SOURCE_DIR}/plcclient")
add_subdirectory ("${PROJECT_SOURCE_DIR}/gcode")

add_executable (plcsvr plcsvr.cc)
target_link_libraries (plcsvr plcmsg)
