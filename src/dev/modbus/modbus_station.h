#ifndef DEV_MODBUS_MODBUSSTATION_H_
#define DEV_MODBUS_MODBUSSTATION_H_

#define ERROR_MSG_LEN 256

#include <stdio.h>
#include <modbus/modbus.h>

class ModbusStation {
 public:
  ModbusStation() {}
  ~ModbusStation();
  int InitModbus(int type, const char *ip_device, int ip_port, int baud,
      char parity, int slave_id);

  void ReleaseModbus();
  int ReadBits(int addr, int nb, uint8_t *table);
  int ReadInputBits(int addr, int nb, uint8_t *table);
  int ReadRegisters(int addr, int nb, uint16_t *table);
  int ReadInputRegisters(int addr, int nb, uint16_t *table);

  int WriteBit(int addr, int status);
  int WriteBits(int addr, int nb, const uint8_t *table);
  int WriteRegister(int addr, int value);
  int WriteRegisters(int addr, int nb, const uint16_t *table);

  void GetErrMsg(char *error) {
    sprintf(error, "%s", error_); 
  }

 private:
  int type_;
  int slave_id_;
  modbus_t *modbus_ctx_;
  char error_[ERROR_MSG_LEN];

};

#endif // DEV_MODBUS_MODBUSSTATION_H_
