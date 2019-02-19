#include "dev/modbus/modbus_manager.h"

ModbusManager::~ModbusManager() {
  std::map<int, ModbusStation *>::iterator it = stations_.begin();
  for (;it != stations_.end(); it++) {
    delete it->second;
  }
  stations_.clear();
}

void ModbusManager::Register(int master_id, ModbusStation *station) {
  std::map<int, ModbusStation *>::iterator it = stations_.find(master_id);
  if (it != stations_.end()) {
    if (it->second) {
      delete it->second;
    }
    stations_.erase(it);
  }
  stations_[master_id] = station;
}

ModbusStation* ModbusManager::GetStation(int master_id) {
  return stations_[master_id];
}

bool ModbusManager::IsMasterExist(int master_id) {
  std::map<int, ModbusStation *>::iterator it = stations_.find(master_id);
  if (it != stations_.end()) {
    return true;
  } else {
    return false;
  }
}

void ModbusManager::DeleteMaster(int master_id) {
  if (stations_[master_id]) {
    delete stations_[master_id];
    stations_.erase(master_id);
  }
}
