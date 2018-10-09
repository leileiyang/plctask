#!/bin/bash

#java -jar CodeGenCmdLine.jar update_with_name=false plc_nml.hh -o plc_nml.cc
java -jar CodeGenCmdLine.jar plc_nml.hh -o plc_nml_win.cc
