#include <iostream>

#include <plcclient.hh>
#include "../nml_intf/plc_nml.hh"

int main() {
  PLCClient client(0.01);
  int ret = client.Startup("plctask.nml");
  FIRST_CMD_MSG msg;
  msg.x = 1;
  msg.serial_number = 1;
  client.SendMsg(msg);
  SECOND_CMD_MSG msg2;
  msg2.x = 2;
  msg2.serial_number = 2;
  client.SendMsg(msg2);
  std::cout << ret << std::endl;
  return 0;

}
