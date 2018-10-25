#ifndef DEV_MODBUS_MODBUSMANAGER_H_
#define DEV_MODBUS_MODBUSMANAGER_H_

#include <map>

#include "dev/modbus/modbus_station.h"

class ModbusManager {
 public:
  ModbusManager() {}
  ~ModbusManager();
  void Register(int slave_id, ModbusStation *station);
  ModbusStation *GetStation(int slave_id);
 private:
  std::map<int, ModbusStation *> stations_;

};

#endif // DEV_MODBUS_MODBUSMANAGER_H_
