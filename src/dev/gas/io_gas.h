#ifndef DEV_GAS_IOGAS_H_
#define DEV_GAS_IOGAS_H_

#include "gas_interface.h"

class IoDevice;

class IOGas: public GasInterface {
 public:
  explicit IOGas(IoDevice *io_dev): io_dev_(io_dev) {}
  virtual bool Open(int gas_id);
  virtual bool Close(int gas_id);
  virtual bool SetPressure(int gas_id, double pressure);
  virtual bool Update(std::map<int, GasItem> &gas_items);

 private:
  IoDevice *io_dev_;

};

#endif
