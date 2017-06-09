#ifndef PLC_NML_HH_
#define PLC_NML_HH_

#include <stat_msg.hh>
#include "plc.hh"

class PLC_STAT_MSG: public RCS_STAT_MSG {
 public:
  PLC_STAT_MSG(NMLTYPE t, size_t s): RCS_STAT_MSG(t, s) {};

  void update(CMS *cms);
};

class PLC_STAT: public PLC_STAT_MSG {
 public:
   PLC_STAT();

   void update(CMS *cms);
   int x;
};

class TEST_CMD_MSG: public RCS_CMD_MSG {
 public:
   TEST_CMD_MSG();

   void update(CMS *cms);
   int x;
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

  DECLARE_NML_DYNAMIC_LENGTH_ARRAY(G_ORDER_MSG, gcode_array_, 30);
};

int plcFormat(NMLTYPE type, void *buffer, CMS *cms);

#endif
