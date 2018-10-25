#ifndef DEV_GAS_GASINTERFACE_H_
#define DEV_GAS_GASINTERFACE_H_

#include <map>
#include <string>

enum GAS_ID {
  GAS_AIR,
  GAS_O2,
  GAS_N2,
  GAS_HIGH_AIR,
  GAS_HIGH_O2,
  GAS_HIGH_N2,
};

class GasInterface {
 public:
  virtual bool Open(int gas_id);
  virtual bool Close(int gas_id);
  virtual bool SetPressure(int gas_id, double pressure);
  virtual void Close();

  static std::map<int, std::string> gas_items;
  static std::map<int, int> gas_states;
  static std::map<int, std::string> CreateGasItems();
  static std::map<int, int> CreateGasState();

};

#endif
