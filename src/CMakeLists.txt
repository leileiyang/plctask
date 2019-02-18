cmake_minimum_required (VERSION 2.6)
project (plctask)
include_directories ("${PROJECT_SOURCE_DIR}")
#include_directories ("${PROJECT_SOURCE_DIR}/nml_intf")
#include_directories ("/home/debian/cnc/include/linuxcnc")
#link_directories ("/home/debian/cnc/lib")

message ("sourse dir:" ${PROJECT_SOURCE_DIR})

if (CMAKE_COMPILER_IS_GNUCC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
elseif (MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
endif()

#file (GLOB DEV_MODBUS_SOURCES
#  "${PROJECT_SOURCE_DIR}/dev/modbus/*.h"
#  "${PROJECT_SOURCE_DIR}/dev/modbus/*.cc")

file (GLOB JOB_SOURCES
  "${PROJECT_SOURCE_DIR}/job/*.h"
  "${PROJECT_SOURCE_DIR}/job/*.cc")

add_executable (plctask plctask.cc plctaskmain.cc ${JOB_SOURCES})
target_link_libraries (plctask plcdevice plcmsg rcs modbus)

add_executable (plcsvr plcsvr.cc)
target_link_libraries (plcsvr plcmsg rcs)

add_subdirectory ("${PROJECT_SOURCE_DIR}/nml_intf")
add_subdirectory ("${PROJECT_SOURCE_DIR}/dev")
