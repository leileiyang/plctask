#ifndef PLCTASK_H_
#define PLCTASK_H_

#include <string>
#include <timer.hh>

#include "job/jobmanager.h"
#include "dev/modbus/modbus_manager.h"
#include "dev/gas/gas.h"
#include "dev/gpio/io_device.h"
#include "nml_intf/interpl.h"
#include "nml_intf/plc_nml.h"

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
  ModbusManager modbus_manager_;
  JobManager job_manager_;

  int ModbusInit(NMLmsg *cmd);
  int ModbusRead(NMLmsg *cmd);
  int ModbusReadBits(int master_id, int slave_id, int addr, int nb);
  int ModbusReadInputBits(int master_id, int slave_id, int addr, int nb);
  int ModbusReadRegisters(int master_id, int slave_id, int addr, int nb);
  int ModbusReadInputRegisters(int master_id, int slave_id, int addr, int nb);

  int ModbusWrite(NMLmsg *cmd);
  int ModbusWriteBit(int master_id, int slave_id, int addr, int status);
  int ModbusWriteBits(int master_id, int slave_id, int addr, int nb,
      const unsigned char *src);

  int ModbusWriteRegister(int master_id, int slave_id, int addr, int value);
  int ModbusWriteRegisters(int master_id, int slave_id, int addr, int nb,
      const unsigned short *src);

  int JobAbort();
  char error_[256];

  IoDevice out_dev_;
  Gas gas_;
  int OpenGas(int gas_id);
  int OpenCuttingGas(int level);
  int SetCuttingPressure(int level);

};

#endif
