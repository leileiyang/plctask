#include <iostream>

#include <plcclient.hh>
#include "../nml_intf/plc_nml.hh"

int main() {
  PLCClient client(1);
  int ret = client.Startup("plctask.nml");
  TEST_CMD_MSG msg;
  msg.x = 1;
  msg.serial_number = 1;
  client.SendMsg(msg);
  msg.x = 2;
  msg.serial_number = 2;
  client.SendMsg(msg);
  std::cout << ret << std::endl;
  return 0;

}
