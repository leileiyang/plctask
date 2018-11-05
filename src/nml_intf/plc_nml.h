#ifndef NML_INTF_PLC_NML_HH_
#define NML_INTF_PLC_NML_HH_

#ifndef WIN32
#include <cms.hh>
#endif

#include <stat_msg.hh>
#include <cmd_msg.hh>
#include <rcs.hh>
#include "plc.h"

#define MODBUS_REGISTER_SIZE 50
#define IO_PORT_SIZE 32 
#define JOB_SIZE 100 

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

class PLC_CMD_MSG: public RCS_CMD_MSG {
 public:
  PLC_CMD_MSG(NMLTYPE t, size_t s): RCS_CMD_MSG(t, s),
      cmd_id_(0),job_id_(-1), exec_(0) {}

  void update(CMS *cms);
  int cmd_id_;
  int job_id_;
  short exec_;
};

class MODBUS_CMD_MSG: public PLC_CMD_MSG {
 public:
  MODBUS_CMD_MSG(NMLTYPE t, size_t s): PLC_CMD_MSG(t, s),
      master_id_(0),slave_id_(0) {}

  void update(CMS *cms);
  int master_id_;
  int slave_id_;
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
  int job_id_;
  int plc_cmd_id_;

  DECLARE_NML_DYNAMIC_LENGTH_ARRAY(unsigned char, modbus_bits, MODBUS_REGISTER_SIZE)
  DECLARE_NML_DYNAMIC_LENGTH_ARRAY(unsigned char, modbus_input_bits, MODBUS_REGISTER_SIZE)
  DECLARE_NML_DYNAMIC_LENGTH_ARRAY(unsigned short, modbus_registers, MODBUS_REGISTER_SIZE)
  DECLARE_NML_DYNAMIC_LENGTH_ARRAY(unsigned short, modbus_input_registers, MODBUS_REGISTER_SIZE)
};

class JOB_ABORT_MSG: public PLC_CMD_MSG {
 public:
  JOB_ABORT_MSG();
  void update(CMS *cms);
};

class OPEN_GAS: public PLC_CMD_MSG {
 public:
  OPEN_GAS();
  void update(CMS *cms);

  int gas_id_;
};

class OPEN_CUTTING_GAS: public PLC_CMD_MSG {
 public:
  OPEN_CUTTING_GAS();
  void update(CMS *cms);

  int level_;
};

class SET_CUTTING_PRESSURE: public PLC_CMD_MSG {
 public:
  SET_CUTTING_PRESSURE();
  void update(CMS *cms);

  int level_;
};

class LHC_FOLLOW: public PLC_CMD_MSG {
 public:
  LHC_FOLLOW();
  void update(CMS *cms);

  int level_;
};

class LHC_PROGRESSIVE_FOLLOW: public PLC_CMD_MSG {
 public:
  LHC_PROGRESSIVE_FOLLOW();
  void update(CMS *cms);

  int level_;
};

class LHC_LIFT_CMD: public PLC_CMD_MSG {
 public:
  LHC_LIFT_CMD();
  void update(CMS *cms);
};

class LHC_Z_AXIS_HOLD_CMD: public PLC_CMD_MSG {
 public:
  LHC_Z_AXIS_HOLD_CMD();
  void update(CMS *cms);
};

class LASER_ON_CMD: public PLC_CMD_MSG {
 public:
  LASER_ON_CMD();
  void update(CMS *cms);
};

class LASER_OFF_CMD: public PLC_CMD_MSG {
 public:
  LASER_OFF_CMD();
  void update(CMS *cms);
};

class LASER_SHUTTER_ON_CMD: public PLC_CMD_MSG {
 public:
  LASER_SHUTTER_ON_CMD();
  void update(CMS *cms);
};

class LASER_SHUTTER_OFF_CMD: public PLC_CMD_MSG {
 public:
  LASER_SHUTTER_OFF_CMD();
  void update(CMS *cms);
};

class LASER_POWER: public PLC_CMD_MSG {
 public:
  LASER_POWER();
  void update(CMS *cms);

  int level_;
};

class LASER_DUTYRATIO: public PLC_CMD_MSG {
 public:
  LASER_DUTYRATIO();
  void update(CMS *cms);

  int level_;
};

class LASER_PULSE_FREQUENCY: public PLC_CMD_MSG {
 public:
  LASER_PULSE_FREQUENCY();
  void update(CMS *cms);

  int level_;
};

class LASER_TYPE: public PLC_CMD_MSG {
 public:
  LASER_TYPE();
  void update(CMS *cms);

  int level_;
};

class IO_CFG_MSG: public PLC_CMD_MSG {
 public:
  IO_CFG_MSG();
  void update(CMS *cms);

  int dev_id_;
  DECLARE_NML_DYNAMIC_LENGTH_ARRAY(short, port_no, IO_PORT_SIZE)
  DECLARE_NML_DYNAMIC_LENGTH_ARRAY(short, func_id, IO_PORT_SIZE)
};

class CUTTING_DELAY_BLOW: public PLC_CMD_MSG {
 public:
  CUTTING_DELAY_BLOW();
  void update(CMS *cms);

  int level_;
};

class CUTTING_DELAY_STAY: public PLC_CMD_MSG {
 public:
  CUTTING_DELAY_STAY();
  void update(CMS *cms);

  int level_;
};

class FOCUS_POSITION: public PLC_CMD_MSG {
 public:
  FOCUS_POSITION();
  void update(CMS *cms);

  int level_;
};

class PLC_JOB_MSG: public PLC_CMD_MSG {
 public:
  PLC_JOB_MSG();
  void update(CMS *cms);
  int plc_job_id_;

  DECLARE_NML_DYNAMIC_LENGTH_ARRAY(short, plc_cmds, JOB_SIZE)
};

int plcFormat(NMLTYPE type, void *buffer, CMS *cms);

#endif
