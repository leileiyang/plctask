#ifndef DEV_GAS_GASINTERFACE_H_
#define DEV_GAS_GASINTERFACE_H_

class GasInterface {
 public:
  virtual bool Open(int gas_id) = 0;
  virtual bool Close(int gas_id) = 0;
  virtual bool SetPressure(int gas_id, double pressure) = 0;
  virtual ~GasInterface() {}
};

#endif
