#include <iostream>
#include "plctask.hh"

int main() {
  PLCTask plcTask(0.01);
  plcTask.Startup("plctask.nml");
  plcTask.Run();
  std::cout << "Done!" << std::endl;
  return 0;
}
