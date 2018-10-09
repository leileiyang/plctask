#ifndef PLC_NML_HH_
#define PLC_NML_HH_

#include <stat_msg.hh>
#include <cmd_msg.hh>
#include "plc.hh"

#define MODBUS_REGISTER_SIZE 30

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

class MODBUS_READ_MSG: public RCS_CMD_MSG {
 public:
  MODBUS_READ_MSG();

  void update(CMS *cms);
  int type_;
  int addr_;
  int nb_;
};

class MODBUS_INIT_MSG: public RCS_CMD_MSG {
 public:
  MODBUS_INIT_MSG();

  void update(CMS *cms);
  int type_;
  int ip_port_;
  int baud_;
  int slave_id_;

  DECLARE_NML_DYNAMIC_LENGTH_ARRAY(char, ip_device, 100)

};


class G_ORDER_MSG: public RCS_CMD_MSG {
 public:
  G_ORDER_MSG();
  void update(CMS *cms);

  unsigned int show_line_;
  unsigned int line_no_in_total_file_;
  unsigned int name_;
  unsigned int m_type_;
  unsigned short piercing_hole_;

  double x0_;
  double y0_;
  double x_;
  double y_;
  double i_;
  double j_;
  double f_;
  double r_;

  float start_angle_;
  float end_angle_;
  float length_;
  float angle_ration;

  double offset_m07_;
  double offset_m08_;

};

class G_CODE_MSG: public RCS_CMD_MSG {
 public:
  G_CODE_MSG();
  void update(CMS *cms);

  DECLARE_NML_DYNAMIC_LENGTH_ARRAY(G_ORDER_MSG, gcode_array_, 30)
};

class MODBUS_REGISTER_STAT: public PLC_STAT_MSG {
 public:
  MODBUS_REGISTER_STAT();
  void update(CMS *cms);

  DECLARE_NML_DYNAMIC_LENGTH_ARRAY(unsigned char, bits, MODBUS_REGISTER_SIZE)
  DECLARE_NML_DYNAMIC_LENGTH_ARRAY(unsigned char, input_bits, MODBUS_REGISTER_SIZE)
  DECLARE_NML_DYNAMIC_LENGTH_ARRAY(unsigned short, registers, MODBUS_REGISTER_SIZE)
  DECLARE_NML_DYNAMIC_LENGTH_ARRAY(unsigned short, input_registers, MODBUS_REGISTER_SIZE)

};

class PLC_STAT: public PLC_STAT_MSG {
 public:
  PLC_STAT();

  void update(CMS *cms);
  MODBUS_REGISTER_STAT modbus_registers_;

};

int plcFormat(NMLTYPE type, void *buffer, CMS *cms);

#endif
