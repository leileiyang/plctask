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

int plcFormat(NMLTYPE type, void *buffer, CMS *cms);

#endif
