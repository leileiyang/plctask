cmake_minimum_required (VERSION 2.6)

file (GLOB DEV_SOURCES
  "${PROJECT_SOURCE_DIR}/dev/gas/*.h"
  "${PROJECT_SOURCE_DIR}/dev/gas/*.cc"
  "${PROJECT_SOURCE_DIR}/dev/gpio/*.h"
  "${PROJECT_SOURCE_DIR}/dev/gpio/*.cc"
  "${PROJECT_SOURCE_DIR}/dev/laser/*.h"
  "${PROJECT_SOURCE_DIR}/dev/laser/*.cc"
  "${PROJECT_SOURCE_DIR}/dev/modbus/*.h"
  "${PROJECT_SOURCE_DIR}/dev/modbus/*.cc")

add_library(plcdevice ${DEV_SOURCES})
