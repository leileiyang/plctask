cmake_minimum_required (VERSION 2.6)

file (GLOB NML_SOURCES
    "*.hh"
    "*.cc")

add_library(plcmsg SHARED ${NML_SOURCES})
