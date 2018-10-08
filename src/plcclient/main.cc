#include <iostream>

#include <plcclient.hh>
#include "../nml_intf/plc_nml.hh"

int main() {
  int i = 1;
  PLCClient client(0.01);
  int ret = client.Startup("plctask.nml");
  for (int j = 0; j < 10; j++) {
    FIRST_CMD_MSG msg;
    msg.x = 1;
    msg.serial_number = i++;
    ret = client.SendMsg(msg);
    std::cout << ret << std::endl;
    SECOND_CMD_MSG msg2;
    msg2.x = 2;
    msg2.serial_number = i++;
    ret = client.SendMsg(msg2);
    std::cout << ret << std::endl;
  }
  std::cin >> i;
  return 0;

}
