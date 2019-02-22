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

class PLC_GAS_STAT {
 public:
  int status_;
  int current_gas_;
  double current_pressure_;
  unsigned short gas_status_;
  unsigned short alarm_status_;
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
      cmd_id_(0),job_id_(-1), exec_(1) {}

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

class GAS_OPEN: public PLC_CMD_MSG {
 public:
  GAS_OPEN();
  void update(CMS *cms);

  int gas_id_;
};

class GAS_OPEN_AUTO: public PLC_CMD_MSG {
 public:
  GAS_OPEN_AUTO();
  void update(CMS *cms);

  int level_;
};

class GAS_PRESSURE_AUTO: public PLC_CMD_MSG {
 public:
  GAS_PRESSURE_AUTO();
  void update(CMS *cms);

  int level_;
};



class LHC_FOLLOW: public PLC_CMD_MSG {
 public:
  LHC_FOLLOW();
  void update(CMS *cms);

  double height_;
};

class LHC_FOLLOW_AUTO: public PLC_CMD_MSG {
 public:
  LHC_FOLLOW_AUTO();
  void update(CMS *cms);

  int level_;
};

class LHC_INCR_FOLLOW_AUTO: public PLC_CMD_MSG {
 public:
  LHC_INCR_FOLLOW_AUTO();
  void update(CMS *cms);

  int level_;
};

class LHC_LIFT_AUTO: public PLC_CMD_MSG {
 public:
  LHC_LIFT_AUTO();
  void update(CMS *cms);
};

class LHC_AXIS_HOLD: public PLC_CMD_MSG {
 public:
  LHC_AXIS_HOLD();
  void update(CMS *cms);
};

class LASER_ON_MANUAL: public PLC_CMD_MSG {
 public:
  LASER_ON_MANUAL();
  void update(CMS *cms);
  double power_;
  double duty_ratio_;
  double frequency_;
  double duration_;
};

class LASER_ON: public PLC_CMD_MSG {
 public:
  LASER_ON();
  void update(CMS *cms);
};

class LASER_OFF: public PLC_CMD_MSG {
 public:
  LASER_OFF();
  void update(CMS *cms);
};

class LASER_SHUTTER_ON: public PLC_CMD_MSG {
 public:
  LASER_SHUTTER_ON();
  void update(CMS *cms);
};

class LASER_SHUTTER_OFF: public PLC_CMD_MSG {
 public:
  LASER_SHUTTER_OFF();
  void update(CMS *cms);
};

class LASER_POWER_AUTO: public PLC_CMD_MSG {
 public:
  LASER_POWER_AUTO();
  void update(CMS *cms);

  int level_;
};

class LASER_DUTYRATIO_AUTO: public PLC_CMD_MSG {
 public:
  LASER_DUTYRATIO_AUTO();
  void update(CMS *cms);

  int level_;
};

class LASER_PULSE_FREQ_AUTO: public PLC_CMD_MSG {
 public:
  LASER_PULSE_FREQ_AUTO();
  void update(CMS *cms);

  int level_;
};

class LASER_TYPE_AUTO: public PLC_CMD_MSG {
 public:
  LASER_TYPE_AUTO();
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

class DELAY_BLOW_AUTO: public PLC_CMD_MSG {
 public:
  DELAY_BLOW_AUTO();
  void update(CMS *cms);

  int level_;
};

class DELAY_STAY_AUTO: public PLC_CMD_MSG {
 public:
  DELAY_STAY_AUTO();
  void update(CMS *cms);

  int level_;
};

class FOCUS_POSITION_AUTO: public PLC_CMD_MSG {
 public:
  FOCUS_POSITION_AUTO();
  void update(CMS *cms);

  int level_;
};

class PLC_JOB_MSG: public PLC_CMD_MSG {
 public:
  PLC_JOB_MSG();
  void update(CMS *cms);

  DECLARE_NML_DYNAMIC_LENGTH_ARRAY(short, plc_cmds, JOB_SIZE)
};

class PLC_EXEC_JOB: public PLC_CMD_MSG {
 public:
  PLC_EXEC_JOB();
  void update(CMS *cms);
};

int plcFormat(NMLTYPE type, void *buffer, CMS *cms);

#endif
