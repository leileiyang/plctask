cmake_minimum_required (VERSION 2.6)
project (plctask)
include_directories ("${PROJECT_SOURCE_DIR}")
include_directories ("${PROJECT_SOURCE_DIR}/nml_intf")

# collect the files in nml_intf directory
include (nml_intf/CMakeLists.txt)
foreach (FILE ${FILES})
  set (nml_intf_files ${nml_intf_files} nml_intf/${FILE})
endforeach (FILE)

add_executable (plctask plctask.cc plctaskmain.cc ${nml_intf_files})
target_link_libraries (plctask rcs)
