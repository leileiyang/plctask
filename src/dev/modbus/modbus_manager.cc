#include "dev/modbus/modbus_manager.h"

ModbusManager::~ModbusManager() {
  std::map<int, ModbusStation *>::iterator it = stations_.begin();
  for (;it != stations_.end(); it++) {
    delete it->second;
  }
  stations_.clear();
}

void ModbusManager::Register(int slave_id, ModbusStation *station) {
  std::map<int, ModbusStation *>::iterator it = stations_.find(slave_id);
  if (it != stations_.end()) {
    if (it->second) {
      delete it->second;
    }
    stations_.erase(it);
  }
  stations_[slave_id] = station;
}

ModbusStation* ModbusManager::GetStation(int slave_id) {
  return stations_[slave_id];
}
