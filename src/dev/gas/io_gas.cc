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

/*
void IOGas::Update(PLC_STATUS status, int gas_id, int &on) {
  status = PLC_DONE;
  // if the gas id does not map to any io port,
  // you will get the port 0 state
  on = io_dev_->GetPortState(gas_id);
}
*/
