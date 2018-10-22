#ifndef PLC_NML_HH_
#define PLC_NML_HH_

#ifndef WIN32
#include <cms.hh>
#endif

#include <stat_msg.hh>
#include <cmd_msg.hh>
#include <rcs.hh>
#include "plc.hh"

#define MODBUS_REGISTER_SIZE 50

enum MB_REGISTER_TYPE {
  MB_REGISTER_BITS,
  MB_REGISTER_INPUT_BITS,
  MB_REGISTER_REGISTERS,
  MB_REGISTER_INPUT_REGISTERS,
};

enum MB_BACKEND_TYPE {
  MB_BACKEND_TCP,
  MB_BACKEND_TCP_PI,
  MB_BACKEND_RTU,
};

class PLC_STAT_MSG: public RCS_STAT_MSG {
 public:
  PLC_STAT_MSG(NMLTYPE t, size_t s): RCS_STAT_MSG(t, s) {};

  void update(CMS *cms);
};

class FIRST_CMD_MSG: public RCS_CMD_MSG {
 public:
  FIRST_CMD_MSG();

  void update(CMS *cms);
  int x;
};

class SECOND_CMD_MSG: public RCS_CMD_MSG {
 public:
  SECOND_CMD_MSG();

  void update(CMS *cms);
  int x;
};

class MODBUS_CMD_MSG: public RCS_CMD_MSG {
 public:
  MODBUS_CMD_MSG(NMLTYPE t, size_t s): RCS_CMD_MSG(t, s),
      master_id_(0),slave_id_(0) {}

  void update(CMS *cms);
  int master_id_;
  int slave_id_;
};

class JOB_CMD_MSG: public RCS_CMD_MSG {
 public:
  JOB_CMD_MSG(NMLTYPE t, size_t s): RCS_CMD_MSG(t, s),
      id_(0),job_id_(0) {}

  void update(CMS *cms);
  int id_;
  int job_id_;
};

class MODBUS_READ_MSG: public MODBUS_CMD_MSG {
 public:
  MODBUS_READ_MSG();

  void update(CMS *cms);
  int type_;
  int addr_;
  int nb_;
};

class MODBUS_INIT_MSG: public MODBUS_CMD_MSG {
 public:
  MODBUS_INIT_MSG();

  void update(CMS *cms);
  int type_;
  int ip_port_;
  int baud_;
  int slave_id_;
  char parity_;

  DECLARE_NML_DYNAMIC_LENGTH_ARRAY(char, ip_device, 100)

};

class MODBUS_WRITE_MSG: public MODBUS_CMD_MSG {
 public:
  MODBUS_WRITE_MSG();

  void update(CMS *cms);
  int type_;
  int addr_;
  int nb_;

  DECLARE_NML_DYNAMIC_LENGTH_ARRAY(unsigned char, bits, MODBUS_REGISTER_SIZE)
  DECLARE_NML_DYNAMIC_LENGTH_ARRAY(unsigned short, registers, MODBUS_REGISTER_SIZE)

};

class PLC_STAT: public PLC_STAT_MSG {
 public:
  PLC_STAT();

  void update(CMS *cms);

  int status_;

  DECLARE_NML_DYNAMIC_LENGTH_ARRAY(unsigned char, modbus_bits, MODBUS_REGISTER_SIZE)
  DECLARE_NML_DYNAMIC_LENGTH_ARRAY(unsigned char, modbus_input_bits, MODBUS_REGISTER_SIZE)
  DECLARE_NML_DYNAMIC_LENGTH_ARRAY(unsigned short, modbus_registers, MODBUS_REGISTER_SIZE)
  DECLARE_NML_DYNAMIC_LENGTH_ARRAY(unsigned short, modbus_input_registers, MODBUS_REGISTER_SIZE)
};

class JOB_MODBUS_WRITE_MSG: public JOB_CMD_MSG {
 public:
  JOB_MODBUS_WRITE_MSG();

  void update(CMS *cms);

  int master_id_;
  int slave_id_;
  int type_;
  int addr_;
  int nb_;

  DECLARE_NML_DYNAMIC_LENGTH_ARRAY(unsigned char, bits, MODBUS_REGISTER_SIZE)
  DECLARE_NML_DYNAMIC_LENGTH_ARRAY(unsigned short, registers, MODBUS_REGISTER_SIZE)
};

class JOB_ABORT_MSG: public RCS_CMD_MSG {
 public:
  JOB_ABORT_MSG();
  void update(CMS *cms);
};

int plcFormat(NMLTYPE type, void *buffer, CMS *cms);

#endif
