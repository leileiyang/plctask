#include "io_gas.h"

#include "dev/gpio/io_device.h"

bool IoGas::Open(int gas_id) {
  io_dev_->Open(gas_id);
  return true;
}

bool IoGas::Close(int gas_id) {
  io_dev_->Close(gas_id);
  return true;
}

bool IoGas::SetPressure(int gas_id, double pressure) {
  return true;
}

bool IoGas::Update(std::map<int, GasItem> &gas_items) {
  std::map<int, GasItem>::iterator it = gas_items.begin();
  for (; it != gas_items.end(); it++) {
    it->second.state = io_dev_->GetPortState(it->first);
  }
  return true;
}
