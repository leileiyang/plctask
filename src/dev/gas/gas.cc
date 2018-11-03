#include "gas.h"
#include "gas_interface.h"
#include "io_gas.h"
#include "plccfg.h"

#ifdef NML_OLC_COMPAT
#include <rcs_print.hh>
#else
#include <rcs_prnt.hh>
#endif

typedef std::pair<int, std::string> pair_t;

std::map<int, GasItem> Gas::gas_items = Gas::CreateGasItems();

static const pair_t gas_pairs[] = {
  pair_t(GAS_AIR, "Air"),
  pair_t(GAS_O2, "Oxygen"),
  pair_t(GAS_N2, "Nitrogen"),
  pair_t(GAS_HIGH_AIR, "HighAir"),
  pair_t(GAS_HIGH_O2, "HighOxygen"),
  pair_t(GAS_HIGH_N2, "HighNitrogen"),
};

std::map<int, GasItem> Gas::CreateGasItems() {
  std::map<int, GasItem> items;
  int size = sizeof(gas_pairs) / sizeof(gas_pairs[0]);
  GasItem item = {0, "Gas", 10, 0}; 
  for (int i = 0; i < size; i++) {
    item.id = gas_pairs[i].first;
    item.name = gas_pairs[i].second;
    items[item.id] = item;
  }
  return items;
}

Gas::Gas(): gas_intf_(NULL), working_gas_(-1) {}

Gas::~Gas() {
  delete gas_intf_;
}

int Gas::ConnectInterface(GasInterface *gas_intf) {
  if (gas_intf) {
    gas_intf_ = gas_intf;
    return 0;
  } else {
    return -1;
  }
}

int Gas::ConnectIoDevice(IoDevice* io_dev) {
  gas_intf_ = new IoGas(io_dev);
  return 0;
}

int Gas::Open(int gas_id) {
  if (gas_intf_->Open(gas_id)) {
    rcs_print("Gas Open %s\n", gas_items[gas_id].name.c_str());
    gas_items[gas_id].state = 1;
    int ret = GAS_OPEN_NO_DELAY;
    if (working_gas_ < 0) {
      ret = GAS_OPEN_FIRST_DELAY;
    } else if (working_gas_ != gas_id) {
      ret = GAS_OPEN_SWITCH_DELAY;
    }
    working_gas_ = gas_id;
    return ret; 
  } else {
    return -1;
  }
}

int Gas::Close(int gas_id) {
  if (gas_intf_->Close(gas_id)) {
    rcs_print("Gas Cloase %s\n", gas_items[gas_id].name.c_str());
    gas_items[gas_id].state = 0;
    return 0; 
  } else {
    return -1;
  }
}

int Gas::SetPressure(int gas_id, double pressure) {
  if (gas_intf_->SetPressure(gas_id, pressure)) {
    gas_items[gas_id].pressure = pressure;
    rcs_print("Set %s Pressure %f\n", gas_items[gas_id].name.c_str(), pressure);
    return 0; 
  } else {
    return -1;
  }
}

void Gas::Update() {
  gas_intf_->Update(gas_items);
}
