#include "io_gas.h"

#include "dev/gpio/io_device.h"

bool IOGas::Open(int gas_id) {
  io_dev_->Open(gas_id);
  return true;
}

bool IOGas::Close(int gas_id) {
  io_dev_->Close(gas_id);
  return true;
}

bool IOGas::SetPressure(int gas_id, double pressure) {
  return true;
}

bool IOGas::Update(std::map<int, GasItem> &gas_items) {
  std::map<int, GasItem>::iterator it = gas_items.begin();
  for (; it != gas_items.end(); it++) {
    it->second.state = io_dev_->GetPortState(it->first);
  }
  return true;
}
