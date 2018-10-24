#include "plctask.hh"

#include <stdio.h>
#include <string.h>

#include <nml_oi.hh>
#include "nml_intf/plc_nml.hh"
#include <rcs_prnt.hh>

#include <iostream>

PLCTask::PLCTask(double sleep_time):
    exec_state_(PLC_TASK_EXEC_DONE),
    timer_(sleep_time),
    running_(true),
    plc_cmd_buffer_(NULL),
    plc_stat_buffer_(NULL),
    plc_err_buffer_(NULL),
    plc_command_(NULL),
    plc_status_(NULL),
    plc_task_cmd_(NULL),
    plan_error_(0),
    execute_error_(0),
    task_eager_(0) {
    
  memset(error_, 0, NML_ERROR_LEN);
}

PLCTask::~PLCTask() {
  Shutdown();
}

int PLCTask::Startup(std::string plc_nmlfile) {
  double end;
  int good;
#define RETRY_TIME 10.0
#define RETRY_INTERVAL 1.0

  end = RETRY_TIME;
  good = 0;
  do {
    if (NULL != plc_cmd_buffer_) {
      delete plc_cmd_buffer_;
    }
    plc_cmd_buffer_ = 
        new RCS_CMD_CHANNEL(plcFormat, "plcCommand", "plc", plc_nmlfile.c_str());

    if (plc_cmd_buffer_->valid()) {
      good = 1;
      break; 
    }
    esleep(RETRY_INTERVAL);
    end -= RETRY_INTERVAL;
  } while (end > 0.0);

  if (!good) {
    return false;
  }

  end = RETRY_TIME;
  good = 0;
  do {
    if (NULL != plc_stat_buffer_) {
      delete plc_stat_buffer_;
    }
    plc_stat_buffer_ = 
        new RCS_STAT_CHANNEL(plcFormat, "plcStatus", "plc", plc_nmlfile.c_str());

    if (plc_stat_buffer_->valid()) {
      good = 1;
      break; 
    }
    esleep(RETRY_INTERVAL);
    end -= RETRY_INTERVAL;
  } while (end > 0.0);

  if (!good) {
    return false;
  }

  end = RETRY_TIME;
  good = 0;
  do {
    if (NULL != plc_err_buffer_) {
      delete plc_err_buffer_;
    }
    plc_err_buffer_ = 
        new NML(nmlErrorFormat, "plcError", "plc", plc_nmlfile.c_str());

    if (plc_err_buffer_->valid()) {
      good = 1;
      break; 
    }
    esleep(RETRY_INTERVAL);
    end -= RETRY_INTERVAL;
  } while (end > 0.0);

  if (!good) {
    return false;
  }
  plc_status_ = new PLC_STAT;

  plc_cmd_buffer_->clear();
  plc_stat_buffer_->clear();
  plc_err_buffer_->clear();
  plc_command_ = plc_cmd_buffer_->get_address();
  plc_status_->echo_serial_number = plc_command_->serial_number;

  return true;
}

void PLCTask::Shutdown() {
  running_ = false;
  if (NULL != plc_err_buffer_) {
    delete plc_err_buffer_;
    plc_err_buffer_ = NULL;
  }

  if (NULL != plc_stat_buffer_) {
    delete plc_stat_buffer_;
    plc_stat_buffer_ = NULL;
  }

  if (NULL != plc_cmd_buffer_) {
    delete plc_cmd_buffer_;
    plc_cmd_buffer_ = NULL;
  }
  if (NULL != plc_command_) {
    delete plc_command_;
    plc_command_ = NULL;
  }
  if (NULL != plc_status_) {
    delete plc_status_;
    plc_status_ = NULL;
  }
}

bool PLCTask::Run() {
  double start_time = etime();
  double end_time = 0;
  rcs_print("Plc task start running...!\n");
  while (running_) {
    end_time = etime();
    /// Job
    // 1 read a command
    if (0 != plc_cmd_buffer_->peek()) {
      // got a new command, clear the errors
      plan_error_ = 0;
      execute_error_ = 0;
    }
    if (0 != Plan()) {
      plan_error_ = 1;
    }
    if (0 != Execute()) {
      execute_error_ = 1;
    }

    UpdateTaskStatus();

    // do top level
    if (plan_error_ || execute_error_ || exec_state_ == PLC_TASK_EXEC_ERROR) {
      plc_status_->status_ = RCS_ERROR;
    } else if (!plan_error_ && !execute_error_ && 
        exec_state_ == PLC_TASK_EXEC_DONE && task_list_.len() == 0 &&
        plc_task_cmd_ == 0) {

      plc_status_->status_ = RCS_DONE;
    }

    plc_status_->echo_serial_number = plc_command_->serial_number;
    plc_status_->command_type = plc_command_->type;
    plc_stat_buffer_->write(plc_status_);
    
    //std::cout << end_time - start_time << std::endl;
    start_time = end_time;
    if (task_eager_) {
      task_eager_ = 0;
    } else {
      timer_.wait();
    }
  }
  return true;
}

int PLCTask::JobAbort() {
  task_list_.clear();
  plan_error_ = 0;
  execute_error_  = 0;
  exec_state_ = PLC_TASK_EXEC_DONE;
  plc_task_cmd_ = 0;
  return 0;
}

int PLCTask::TaskIssueCommand(NMLmsg *cmd) {
  int retval = 0;
  JOB_CMD_MSG *job_cmd = static_cast<JOB_CMD_MSG *>(cmd);
  if (job_cmd) {
    if (job_cmd->job_id_ >= 0) { // a queue job plc command
      job_manager_.AppendCommand(cmd);
      task_eager_ = 1;
      return 0;
    } else { // a alone job plc command, execute immediately
      ;
    }
  } else { // not a job command, execute immediately
    ;
  }
  switch (cmd->type) {
    case FIRST_CMD_MSG_TYPE:
      rcs_print("Execute FIRST_CMD_MSG_TYPE!\n");
      break;
    case SECOND_CMD_MSG_TYPE:
      rcs_print("Execute SECOND_CMD_MSG_TYPE!\n");
      break;
    case JOB_ABORT_MSG_TYPE:
      retval = JobAbort();
      break;
    case MODBUS_READ_MSG_TYPE:
      if (ModbusRead(cmd) >= 0) {
        retval = 0;
      } else {
        retval = -1;
      }
      break;
    case MODBUS_WRITE_MSG_TYPE:
      if (ModbusWrite(cmd) >= 0) {
        retval = 0;
      } else {
        retval = -1;
      }
      break;
    case MODBUS_INIT_MSG_TYPE:
      if (ModbusInit(cmd) < 0) {
        retval = -1;
      }
      break;
    default:
      break;
  }
  return retval;
}

int PLCTask::TaskQueueCommand(NMLmsg *cmd) {
  if (cmd == 0) {
    return 0;
  }
  task_list_.append(cmd);
  return 0;
}

int PLCTask::Plan() {
  NMLTYPE type;
  int retval = 0;
  // check for new command
  if (plc_command_->serial_number != plc_status_->echo_serial_number) {
    type = plc_command_->type;
  } else {
    type = 0;
  }

  switch (type) {
    // no command
    case 0:
      break;
    // queue command
    case FIRST_CMD_MSG_TYPE:
    case SECOND_CMD_MSG_TYPE:
      retval = TaskQueueCommand(plc_command_);
      break;
    // immediate command
    case JOB_ABORT_MSG_TYPE:
    case MODBUS_INIT_MSG_TYPE:
    case MODBUS_READ_MSG_TYPE:
    case MODBUS_WRITE_MSG_TYPE:
      retval = TaskIssueCommand(plc_command_);
      break;
    default:
      break;
  }
  return retval;
}

int PLCTask::Execute() {
  int retval = 0;
  switch (exec_state_) {
    case PLC_TASK_EXEC_ERROR:
      task_list_.clear();
      exec_state_ = PLC_TASK_EXEC_DONE;
      break;
    case PLC_TASK_EXEC_DONE:
      if (0 == plc_task_cmd_) {
        plc_task_cmd_ = task_list_.get();
        if (0 != plc_task_cmd_) {
          task_eager_ = 1;
          exec_state_ = (enum PLC_TASK_EXEC_ENUM)
             CheckPreconditions(plc_task_cmd_);

        }
      } else {
        if (0 != TaskIssueCommand(plc_task_cmd_)) {
          exec_state_ = PLC_TASK_EXEC_ERROR;
          retval = -1;
        } else {
          exec_state_ = (enum PLC_TASK_EXEC_ENUM)
              CheckPostconditions(plc_task_cmd_);

          task_eager_ = 1;
        }
        plc_task_cmd_ = 0;
      }
      break;
    case PLC_TASK_EXEC_WAITING_FOR_DEVICES:
      break;
    default:
      break;
  }
  return retval;
}

int PLCTask::CheckPreconditions(NMLmsg *cmd) {
  if (0 == cmd) {
    return PLC_TASK_EXEC_DONE;
  }
  switch (cmd->type) {
    case FIRST_CMD_MSG_TYPE:
    case SECOND_CMD_MSG_TYPE:
      return PLC_TASK_EXEC_DONE;
    default:
      return PLC_TASK_EXEC_DONE;
  }
  return PLC_TASK_EXEC_DONE;
}

int PLCTask::CheckPostconditions(NMLmsg *cmd) {
  if (0 == cmd) {
    return PLC_TASK_EXEC_DONE;
  }
  switch (cmd->type) {
    default:
      return PLC_TASK_EXEC_DONE;
  }
  return PLC_TASK_EXEC_DONE;

}

int PLCTask::UpdateTaskStatus() {
  if (strlen(error_)) {
    NML_ERROR error_msg;
    sprintf(error_msg.error, "%s", error_);
    memset(error_, 0, NML_ERROR_LEN);
    error_msg.error[NML_ERROR_LEN - 1] = 0;
    plc_err_buffer_->write(error_msg);
  }
  return 0;
}



#define ERROR_REPORT(err_msg) \
  memset(error_, 0, NML_ERROR_LEN); \
  rcs_print(err_msg);               \
  sprintf(error_, "%s", err_msg)
 
ModbusManager *PLCTask::GetModbusMaster(int master_id) {
  if (master_id == 0) {
    return &thc_master_;
  }
  ERROR_REPORT("no such modbus master workstation\n");
  return NULL;
}

int PLCTask::ModbusInit(NMLmsg *cmd) {
  MODBUS_INIT_MSG *msg = (MODBUS_INIT_MSG *)cmd;
  char ip_device[100] = {0};
  memcpy(ip_device, msg->ip_device, msg->ip_device_length);

  ModbusManager *manager = GetModbusMaster(msg->master_id_);
  if (!manager) {
    return 0;
  }

  ModbusStation *station = new ModbusStation;
  int ret = station->InitModbus(msg->type_, ip_device, msg->ip_port_, msg->baud_,
      msg->parity_, msg->slave_id_);

  if (ret < 0) {
    memset(error_, 0, NML_ERROR_LEN);
    station->GetErrMsg(error_);
    delete station;
  } else {
    manager->Register(msg->slave_id_, station);
    rcs_print("Modbus Init Success!\n");
  }
  return ret;
}

int PLCTask::ModbusRead(NMLmsg *cmd) {
  MODBUS_READ_MSG *read_msg = (MODBUS_READ_MSG *)cmd;
  ModbusManager *manager = GetModbusMaster(read_msg->master_id_);
  if (!manager) {
    return 0;
  }

  int rc = 0;
  switch (read_msg->type_) {
    case MB_REGISTER_BITS:
      rc = ModbusReadBits(manager, read_msg->slave_id_, read_msg->addr_,
          read_msg->nb_);

      break;
    case MB_REGISTER_INPUT_BITS:
      rc = ModbusReadInputBits(manager, read_msg->slave_id_, read_msg->addr_,
          read_msg->nb_);

      break;
    case MB_REGISTER_REGISTERS:
      rc = ModbusReadRegisters(manager, read_msg->slave_id_, read_msg->addr_,
          read_msg->nb_);

      break;
    case MB_REGISTER_INPUT_REGISTERS:
      rc = ModbusReadInputRegisters(manager, read_msg->slave_id_,
          read_msg->addr_, read_msg->nb_);
      
      break;
    default:
      break;
  }
  return rc;
}

#define MODBUS_MANAGER_RW(modbus_func) \
  int ret = 0; \
  ModbusStation *station = manager->GetStation(slave_id); \
  if (station) { \
    ret = station->modbus_func(addr, nb, tab_rp_regs); \
    if (ret > 0) { \
      reply_nb = ret; \
    } else { \
      memset(error_, 0, NML_ERROR_LEN); \
      station->GetErrMsg(error_); \
    } \
  } else { \
    memset(error_, 0, NML_ERROR_LEN); \
    sprintf(error_, "no such modbus slave workstation\n"); \
  } \
  return ret;

int PLCTask::ModbusReadBits(ModbusManager *manager, int slave_id,
    int addr, int nb) {

  uint8_t *tab_rp_regs = (uint8_t *)(plc_status_->modbus_bits);
  int &reply_nb = plc_status_->modbus_bits_length ;

  MODBUS_MANAGER_RW(ReadBits);
}

int PLCTask::ModbusReadInputBits(ModbusManager *manager, int slave_id,
    int addr, int nb) {

  uint8_t *tab_rp_regs = (uint8_t *)(plc_status_->modbus_input_bits);
  int &reply_nb = plc_status_->modbus_input_bits_length ;

  MODBUS_MANAGER_RW(ReadInputBits)
}

int PLCTask::ModbusReadRegisters(ModbusManager *manager, int slave_id,
    int addr, int nb) {

  uint16_t *tab_rp_regs = \
      (uint16_t *)(plc_status_->modbus_registers);

  int &reply_nb = plc_status_->modbus_registers_length;

  MODBUS_MANAGER_RW(ReadRegisters)
}

int PLCTask::ModbusReadInputRegisters(ModbusManager *manager, int slave_id,
    int addr, int nb) {

  uint16_t *tab_rp_regs = \
      (uint16_t *)(plc_status_->modbus_input_registers);

  int &reply_nb = plc_status_->modbus_input_registers_length;

  MODBUS_MANAGER_RW(ReadInputRegisters)
}

int PLCTask::ModbusWriteBits(ModbusManager *manager, int slave_id,
    int addr, int nb, const unsigned char *src) {

  uint8_t *tab_rp_regs = (uint8_t *)src;
  int reply_nb = 0;

  MODBUS_MANAGER_RW(WriteBits)
}

int PLCTask::ModbusWriteRegisters(ModbusManager *manager, int slave_id,
    int addr, int nb, const unsigned short *src) {

  uint16_t *tab_rp_regs = (uint16_t *)src;
  int reply_nb = 0;

  MODBUS_MANAGER_RW(WriteRegisters)
}

int PLCTask::ModbusWriteBit(ModbusManager *manager, int slave_id,
    int addr, int status) {

  int ret = 0;
  ModbusStation *station = manager->GetStation(slave_id);
  if (station) {
    ret = station->WriteBit(addr, status);
    if (ret < 0) {
      memset(error_, 0, NML_ERROR_LEN);
      station->GetErrMsg(error_);
    }
  } else {
    memset(error_, 0, NML_ERROR_LEN);
    sprintf(error_, "no such modbus slave workstation\n");
  }
  return ret;
}

int PLCTask::ModbusWriteRegister(ModbusManager *manager, int slave_id,
    int addr, int value) {
 
  int ret = 0;
  ModbusStation *station = manager->GetStation(slave_id);
  if (station) {
    ret = station->WriteRegister(addr, value);
    if (ret < 0) {
      memset(error_, 0, NML_ERROR_LEN);
      station->GetErrMsg(error_);
    }
  } else {
    memset(error_, 0, NML_ERROR_LEN);
    sprintf(error_, "no such modbus slave workstation\n");
  }
  return ret;
}

int PLCTask::ModbusWrite(NMLmsg *cmd) {
  MODBUS_WRITE_MSG *write_msg = (MODBUS_WRITE_MSG *)cmd;
  ModbusManager *manager = GetModbusMaster(write_msg->master_id_);
  if (!manager) {
    return 0;
  }
  int rc = 0;
  switch (write_msg->type_) {
    case MB_REGISTER_BITS:
      if (write_msg->nb_ == 1) {
        rc = ModbusWriteBit(manager, write_msg->slave_id_,
            write_msg->addr_, write_msg->bits[0]);

      } else {
        rc = ModbusWriteBits(manager, write_msg->slave_id_,
            write_msg->addr_, write_msg->nb_, write_msg->bits);

      }
      break;
    case MB_REGISTER_REGISTERS:
      if (write_msg->nb_ == 1) {
        rc = ModbusWriteRegister(manager, write_msg->slave_id_,
            write_msg->addr_, write_msg->registers[0]);

      } else {
        rc = ModbusWriteRegisters(manager, write_msg->slave_id_,
            write_msg->addr_, write_msg->nb_, write_msg->registers);

      }
      break;
    default:
      break;
  }
  return rc;
}
