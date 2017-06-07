#ifndef PLCTASK_HH_
#define PLCTASK_HH_

#include <string>
#include <timer.hh>
#include <cmd_msg.hh>
#include <stat_msg.hh>

#include "nml_intf/interpl.hh"
#include "nml_intf/plc_nml.hh"

enum PLC_STATE_ENUM {
  PLC_STATE_IDLE = 1,
  PLC_STATE_EXECUTING = 2,
  PLC_STATE_PAUSED = 3,
  PLC_STATE_WAITING = 4,

};

enum PLC_TASK_EXEC_ENUM {
  PLC_TASK_EXEC_ERROR = 1,
  PLC_TASK_EXEC_DONE = 2,
  PLC_TASK_EXEC_WAITING_FOR_DEVICES = 3,

};



class PLCTask {
 public:
  PLCTask(double sleep_time);
  ~PLCTask();

  bool Run();
  void Shutdown();
  int Startup(std::string plc_nmlfile);

 private:
  PLC_TASK_EXEC_ENUM exec_state_;
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
  PLC_STAT *plc_status_;
  NMLmsg *plc_task_cmd_;
  int plan_error_;
  int execute_error_;

  int Plan();
  int Execute();
  int TaskIssueCommand(NMLmsg *cmd);
  int TaskQueueCommand(NMLmsg *cmd);
  int TaskCheckPreconditions(NMLmsg *cmd);
  int TaskCheckPostconditions(NMLmsg *cmd);
  int UpdateDevicesStatus();

};

#endif
