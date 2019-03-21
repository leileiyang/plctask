#include <iostream>
#include "plctask.h"

int main() {
  PlcTask plcTask(0.01);
  plcTask.Startup("plctask.nml");
  plcTask.InitDevices();
  plcTask.Run();
  std::cout << "Done!" << std::endl;
  return 0;
}
