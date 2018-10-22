#include "dev/modbus/modbus_station.hh"

#include <errno.h>
#include <string.h>
#include <rcs_print.hh>

#include "nml_intf/plc_nml.hh"

ModbusStation::~ModbusStation() {
  ReleaseModbus();
}

int ModbusStation::InitModbus(int type, const char *ip_device, int ip_port,
    int baud, char parity, int slave_id) {

  type_ = type;
  slave_id_ = slave_id;
  switch (type_) {
    case MB_BACKEND_TCP:
      modbus_ctx_ = modbus_new_tcp(ip_device, ip_port);
      break;
    case MB_BACKEND_TCP_PI:
      break;
    case MB_BACKEND_RTU:
      modbus_ctx_ = modbus_new_rtu(ip_device, baud, parity, 8, 1);
      if (modbus_ctx_) {
        modbus_set_slave(modbus_ctx_, slave_id);
      }
      break;
    default:
      break;
  }
  if (modbus_ctx_ == NULL) {
    rcs_print("Modbus Init failed!\n");
    memset(error_, 0, ERROR_MSG_LEN);
    sprintf(error_, "Modbus Init Failed!\n");
    return -1;
  } else {
    if (modbus_connect(modbus_ctx_) == -1) {
      rcs_print("Connection failed: %s\n", modbus_strerror(errno));
      memset(error_, 0, ERROR_MSG_LEN);
      sprintf(error_, "Connection failed: %s\n", modbus_strerror(errno));
      modbus_free(modbus_ctx_);
      return -2;
    }
  }
  return 0;
}

void ModbusStation::ReleaseModbus() {
  if (modbus_ctx_) {
    modbus_close(modbus_ctx_);
    modbus_free(modbus_ctx_);
  }
}

#define MODBUS_READ_WRITE(modbus_func) \
  int try_count = 3; \
  int rc = 0; \
  do { \
    rc = modbus_func(modbus_ctx_, addr, nb, table); \
    if (rc < 0) { \
      try_count--; \
    } else { \
      break; \
    } \
  } while (try_count > 0); \
\
  if (try_count == 0) { \
    rcs_print("%s failed: %s\n", func_name, modbus_strerror(errno)); \
    memset(error_, 0, ERROR_MSG_LEN); \
    sprintf(error_, "%s failed!\n", func_name); \
  } \
  return rc; \

int ModbusStation::ReadBits(int addr, int nb, uint8_t *table) {
  static const char *func_name = "modbus_read_bits";
  MODBUS_READ_WRITE(modbus_read_bits)
}

int ModbusStation::ReadInputBits(int addr, int nb, uint8_t *table) {
  static const char *func_name = "modbus_read_input_bits"; 
  MODBUS_READ_WRITE(modbus_read_input_bits)
}

int ModbusStation::ReadRegisters(int addr, int nb, uint16_t *table) {
  static const char *func_name = "modbus_read_registers";
  MODBUS_READ_WRITE(modbus_read_registers)
}

int ModbusStation::ReadInputRegisters(int addr, int nb, uint16_t *table) {
  static const char *func_name = "modbus_read_input_registers";
  MODBUS_READ_WRITE(modbus_read_input_registers)
}

int ModbusStation::WriteBits(int addr, int nb, const uint8_t *table) {
  static const char *func_name = "modbus_write_bits";
  MODBUS_READ_WRITE(modbus_write_bits)
}

int ModbusStation::WriteRegisters(int addr, int nb, const uint16_t *table) {
  static const char *func_name = "modbus_write_registers";
  MODBUS_READ_WRITE(modbus_write_registers)
}

int ModbusStation::WriteBit(int addr, int status) {
  static const char *func_name = "modbus_write_bit";
  int try_count = 3;
  int rc = 0;
  do {
    int rc = modbus_write_bit(modbus_ctx_, addr, status);
    if (rc < 0) {
      try_count--;
    } else {
      break;
    }
  } while (try_count > 0);

  if (try_count == 0) {
    rcs_print("%s failed: %s\n", func_name, modbus_strerror(errno));
    memset(error_, 0, ERROR_MSG_LEN);
    sprintf(error_, "%s failed!\n", func_name);
  }
  return rc;
}

int ModbusStation::WriteRegister(int addr, int value) {
  static const char *func_name = "modbus_write_register";
  int try_count = 3;
  int rc = 0;
  do {
    int rc = modbus_write_register(modbus_ctx_, addr, value);
    if (rc < 0) {
      try_count--;
    } else {
      break;
    }
  } while (try_count > 0);

  if (try_count == 0) {
    rcs_print("%s failed: %s\n", func_name, modbus_strerror(errno));
    memset(error_, 0, ERROR_MSG_LEN);
    sprintf(error_, "%s failed!\n", func_name);
  }
  return rc;
}
