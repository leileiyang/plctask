#ifndef PLCTASK_HH_
#define PLCTASK_HH_

#include <string>
#include <timer.hh>
#include <cmd_msg.hh>
#include <stat_msg.hh>

#include "nml_intf/interpl.hh"

enum PLC_STATE_ENUM {
  PLC_STATE_IDLE = 1,
  PLC_STATE_EXECUTING = 2,
  PLC_STATE_PAUSED = 3,
  PLC_STATE_WAITING = 4,

};


class PLCTask {
 public:
  PLCTask(double sleep_time);
  ~PLCTask();

  bool Run();
  void Shutdown();
  int Startup(std::string plc_nmlfile);

 private:
  PLC_STATE_ENUM state_;
  RCS_TIMER timer_;
  NML_INTERP_LIST plc_list_;
  bool running_;

  // communication channel
  RCS_CMD_CHANNEL *plc_cmd_buffer_;
  RCS_STAT_CHANNEL *plc_stat_buffer_;
  NML *plc_err_buffer_;

  // communication message
  RCS_CMD_MSG *plc_command_;
  //RCS_STAT_MSG *plc_status_;

  int Plan();
  int Execute();

};

#endif
