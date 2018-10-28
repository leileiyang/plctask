#ifndef DEV_GAS_GAS_H_
#define DEV_GAS_GAS_H_

#include <string>

#include "gas_interface.h"

enum GAS_ID {
  GAS_AIR,
  GAS_O2,
  GAS_N2,
  GAS_HIGH_AIR,
  GAS_HIGH_O2,
  GAS_HIGH_N2,
};

class Gas {
 public:
  Gas();
  ~Gas();
  int Open(int gas_id);
  int Close(int gas_id);
  int SetPressure(int gas_id, double pressure);

  // Before using the gas device, you must connect a
  // gas interface class instances first.
  int ConnectInterface(GasInterface *gas_intf);
  void Update();

  static std::map<int, GasItem> gas_items;
  static std::map<int, GasItem> CreateGasItems();

 private:
  GasInterface *gas_intf_;
  int working_gas_;

};

#endif
