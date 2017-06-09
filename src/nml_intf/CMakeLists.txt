cmake_minimum_required (VERSION 2.6)

#set (FILES plc_nml.cc interpl.cc)
add_library (plcmsg SHARED plc_nml.cc interpl.cc)
