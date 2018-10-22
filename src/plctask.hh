#ifndef PLCTASK_HH_
#define PLCTASK_HH_

#include <string>
#include <timer.hh>

#include "job/jobmanager.hh"
#include "dev/modbus/modbus_manager.hh"
#include "nml_intf/interpl.hh"
#include "nml_intf/plc_nml.hh"

enum PLC_TASK_EXEC_ENUM {
  PLC_TASK_EXEC_ERROR = 1,
  PLC_TASK_EXEC_DONE = 2,
  PLC_TASK_EXEC_WAITING_FOR_DEVICES = 3,

};



class PLCTask {
 public:
  PLCTask(double sleep_time);
  virtual ~PLCTask();

  bool Run();
  void Shutdown();
  int Startup(std::string plc_nmlfile);

 protected:
  PLC_TASK_EXEC_ENUM exec_state_;
  RCS_TIMER timer_;
  NML_INTERP_LIST task_list_;
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
  int task_eager_;

  int Plan();
  int Execute();
  int TaskIssueCommand(NMLmsg *cmd);
  int TaskQueueCommand(NMLmsg *cmd);
  int CheckPreconditions(NMLmsg *cmd);
  int CheckPostconditions(NMLmsg *cmd);
  int UpdateTaskStatus();

 private:
  ModbusManager thc_master_;
  ModbusManager *GetModbusMaster(int master_id);
  JobManager job_manager_;

  int ModbusInit(NMLmsg *cmd);
  int ModbusRead(NMLmsg *cmd);
  int ModbusReadBits(ModbusManager *manager, int slave_id, int addr, int nb);
  int ModbusReadInputBits(ModbusManager *manager, int slave_id,
      int addr, int nb);

  int ModbusReadRegisters(ModbusManager *manager, int slave_id,
      int addr, int nb);

  int ModbusReadInputRegisters(ModbusManager *manager, int slave_id,
      int addr, int nb);

  int ModbusWrite(NMLmsg *cmd);
  int ModbusWriteBit(ModbusManager *manager, int slave_id,
      int addr, int status);

  int ModbusWriteBits(ModbusManager *manager, int slave_id,
      int addr, int nb, const unsigned char *src);

  int ModbusWriteRegister(ModbusManager *manager, int slave_id,
      int addr, int value);

  int ModbusWriteRegisters(ModbusManager *manager, int slave_id,
      int addr, int nb, const unsigned short *src);

  int JobAbort();
  char error_[256];

};

#endif
