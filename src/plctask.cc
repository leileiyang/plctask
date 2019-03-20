#include "plctask.h"

#include <stdio.h>
#include <string.h>

#include <iostream>

#include <nml_oi.hh>
#include <rcs_prnt.hh>
#include <timer.hh>

#include "nml_intf/plc_nml.h"


PlcTask::PlcTask(double sleep_time):
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
    task_eager_(0),
    current_layer_(0),
    plc_args_(CRAFT_LAYERS, PlcArgs()),
    delay_timeout_(0.),
    delay_left_(0.) {
    
  memset(error_, 0, NML_ERROR_LEN);
}

PlcTask::~PlcTask() {
  Shutdown();
}

int PlcTask::Startup(std::string plc_nmlfile) {
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

void PlcTask::Shutdown() {
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

  if (NULL != plc_status_) {
    delete plc_status_;
    plc_status_ = NULL;
  }
}

bool PlcTask::Run() {
  //double start_time = etime();
  //double end_time = 0;
  //double elapse = 0;
  rcs_print("Plc task start running...!\n");
  while (running_) {
    //end_time = etime();
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
      plc_status_->status = RCS_ERROR;
    } else if (!plan_error_ && !execute_error_ && 
        exec_state_ == PLC_TASK_EXEC_DONE && task_list_.len() == 0 &&
        plc_task_cmd_ == 0) {

      plc_status_->status = RCS_DONE;
    } else {
      plc_status_->status = RCS_EXEC;
    }

    plc_status_->echo_serial_number = plc_command_->serial_number;
    plc_status_->command_type = plc_command_->type;
    plc_stat_buffer_->write(plc_status_);
    
    //elapse = end_time - start_time;
    //start_time = end_time;
    if (task_eager_) {
      task_eager_ = 0;
    } else {
      timer_.wait();
    }
  }
  return true;
}

int PlcTask::JobAbort() {
  task_list_.clear();
  plan_error_ = 0;
  execute_error_  = 0;
  exec_state_ = PLC_TASK_EXEC_DONE;
  plc_task_cmd_ = 0;
  return 0;
}

int PlcTask::TaskIssueCommand(NMLmsg *cmd) {
  int retval = 0;
  PLC_CMD_MSG *plc_cmd = static_cast<PLC_CMD_MSG *>(cmd);
  if (plc_cmd->exec_ == 0) { // a queue job plc command
    job_manager_.AppendCommand(cmd);
    task_eager_ = 1;
    return 0;
  } else { // a alone job plc command, execute immediately
    plc_status_->plc_cmd_id_ = plc_cmd->cmd_id_;
  }
  switch (cmd->type) {
    case FIRST_CMD_MSG_TYPE:
      rcs_print("Execute FIRST_CMD_MSG_TYPE!\n");
      break;
    case SECOND_CMD_MSG_TYPE:
      rcs_print("Execute SECOND_CMD_MSG_TYPE!\n");
      break;
    case PLC_EXEC_JOB_TYPE:
      retval = ExecuteJob(plc_cmd->job_id_);
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
    case GAS_OPEN_TYPE:
      retval = OpenGas(((GAS_OPEN *)cmd)->gas_id_);
      break;
    case GAS_OPEN_AUTO_TYPE:
      retval = OpenCuttingGas(((GAS_OPEN_AUTO *)cmd)->level_);
      break;
    case GAS_PRESSURE_AUTO_TYPE:
      retval = SetCuttingPressure(((GAS_PRESSURE_AUTO *)cmd)->level_);
      break;
    case DELAY_BLOW_AUTO_TYPE:
      retval = CuttingBlow(((DELAY_BLOW_AUTO *)cmd)->level_);
      break;
    case DELAY_STAY_AUTO_TYPE:
      retval = CuttingStay(((DELAY_STAY_AUTO *)cmd)->level_);
      break;
    case LASER_ON_TYPE:
      retval = LaserOn();
      break;
    case LASER_OFF_TYPE:
      retval = LaserOff();
      break;
    case LASER_SHUTTER_ON_TYPE:
      retval = ShutterOn();
      break;
    case LASER_SHUTTER_OFF_TYPE:
      retval = ShutterOff();
      break;
    case LASER_POWER_AUTO_TYPE:
      retval = SetCuttingPower(((LASER_POWER_AUTO *)cmd)->level_);
      break;
    case LASER_DUTYRATIO_AUTO_TYPE:
      retval = SetCuttingDutyRation(((LASER_DUTYRATIO_AUTO *)cmd)->level_);
      break;
    case LASER_PULSE_FREQ_AUTO_TYPE:
      retval = SetCuttingPulseFrequency(((LASER_PULSE_FREQ_AUTO *)cmd)->level_);
      break;
    case LASER_TYPE_AUTO_TYPE:
      retval = SetCuttingLaserType(((LASER_TYPE_AUTO *)cmd)->level_);
      break;
    default:
      break;
  }
  return retval;
}

int PlcTask::TaskQueueCommand(NMLmsg *cmd) {
  if (cmd == 0) {
    return 0;
  }
  task_list_.append(cmd);
  return 0;
}

int PlcTask::Plan() {
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
    case PLC_EXEC_JOB_TYPE:
    case JOB_ABORT_MSG_TYPE:
    case GAS_OPEN_TYPE:
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

int PlcTask::Execute() {
  int retval = 0;
  switch (exec_state_) {
    case PLC_TASK_EXEC_ERROR:
      task_list_.clear();
      plc_task_cmd_ = 0;
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
    case PLC_TASK_EXEC_WAITING_FOR_DELAY:
      delay_left_ = delay_timeout_ - etime();
      if (etime() >= delay_timeout_) {
        exec_state_ = PLC_TASK_EXEC_DONE;
        delay_timeout_ = 0;
        delay_left_ = 0;
      }
      break;
    default:
      break;
  }
  return retval;
}

int PlcTask::CheckPreconditions(NMLmsg *cmd) {
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

int PlcTask::CheckPostconditions(NMLmsg *cmd) {
  if (0 == cmd) {
    return PLC_TASK_EXEC_DONE;
  }
  switch (cmd->type) {
    case DELAY_STAY_AUTO_TYPE:
      return PLC_TASK_EXEC_WAITING_FOR_DELAY;
    case DELAY_BLOW_AUTO_TYPE: {
      DELAY_BLOW_AUTO *blow = (DELAY_BLOW_AUTO *)cmd;
      if (plc_args_[current_layer_].delay_args.blow_enable_[blow->level_]) {
        return PLC_TASK_EXEC_WAITING_FOR_DELAY;
      } else {
        return PLC_TASK_EXEC_DONE;
      }
                          }
      break;
    case GAS_OPEN_AUTO_TYPE:
      if (delay_timeout_ > 0) {
        return PLC_TASK_EXEC_WAITING_FOR_DELAY;
      } else {
        return PLC_TASK_EXEC_DONE;
      }
      break;
    default:
      return PLC_TASK_EXEC_DONE;
  }
  return PLC_TASK_EXEC_DONE;
}

int PlcTask::UpdateTaskStatus() {
  if (strlen(error_)) {
    NML_ERROR error_msg;
    sprintf(error_msg.error, "%s", error_);
    memset(error_, 0, NML_ERROR_LEN);
    error_msg.error[NML_ERROR_LEN - 1] = 0;
    plc_err_buffer_->write(error_msg);
  }
  gas_.UpdateStatus(plc_status_->gas_stat_);
  return 0;
}


int PlcTask::ExecuteJob(int job_id) {
  PlcJob *job = job_manager_.GetPlcJob(job_id);
  if (job) {
    job->ArrangeJob(task_list_);
    plc_status_->job_id_ = job_id;
  } else {
    memset(error_, 0, NML_ERROR_LEN);
    sprintf(error_, "no such job with id %d\n", job_id);
    return -1;
  }
  return 0;
}

int PlcTask::ModbusInit(NMLmsg *cmd) {
  MODBUS_INIT_MSG *msg = (MODBUS_INIT_MSG *)cmd;
  char ip_device[100] = {0};
  memcpy(ip_device, msg->ip_device, msg->ip_device_length);

  if (modbus_manager_.IsMasterExist(msg->master_id_)) {
    modbus_manager_.DeleteMaster(msg->master_id_);
  }

  ModbusStation *station = new ModbusStation;
  int ret = station->InitModbus(msg->type_, ip_device, msg->ip_port_,
      msg->baud_, msg->parity_, msg->slave_id_);

  if (ret < 0) {
    memset(error_, 0, NML_ERROR_LEN);
    station->GetErrMsg(error_);
    delete station;
  } else {
    modbus_manager_.Register(msg->master_id_, station);
    rcs_print("Modbus Init Success!\n");
  }
  return ret;
}

int PlcTask::ModbusRead(NMLmsg *cmd) {
  MODBUS_READ_MSG *read_msg = (MODBUS_READ_MSG *)cmd;
  int rc = 0;
  switch (read_msg->type_) {
    case MB_REGISTER_BITS:
      rc = ModbusReadBits(read_msg->master_id_, read_msg->slave_id_,
          read_msg->addr_, read_msg->nb_);

      break;
    case MB_REGISTER_INPUT_BITS:
      rc = ModbusReadInputBits(read_msg->master_id_, read_msg->slave_id_,
          read_msg->addr_, read_msg->nb_);

      break;
    case MB_REGISTER_REGISTERS:
      rc = ModbusReadRegisters(read_msg->master_id_, read_msg->slave_id_,
          read_msg->addr_, read_msg->nb_);

      break;
    case MB_REGISTER_INPUT_REGISTERS:
      rc = ModbusReadInputRegisters(read_msg->master_id_, read_msg->slave_id_,
          read_msg->addr_, read_msg->nb_);
      
      break;
    default:
      break;
  }
  return rc;
}

#define MODBUS_MANAGER_RW(modbus_func) \
  int ret = 0; \
  ModbusStation *station = modbus_manager_.GetStation(master_id); \
  if (station) { \
    if (station->SlaveId() != slave_id) { \
      station->SetSlaveId(slave_id); \
    } \
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

int PlcTask::ModbusReadBits(int master_id, int slave_id, int addr, int nb) {
  uint8_t *tab_rp_regs = (uint8_t *)(plc_status_->modbus_bits);
  int &reply_nb = plc_status_->modbus_bits_length ;

  MODBUS_MANAGER_RW(ReadBits);
}

int PlcTask::ModbusReadInputBits(int master_id, int slave_id,
    int addr, int nb) {

  uint8_t *tab_rp_regs = (uint8_t *)(plc_status_->modbus_input_bits);
  int &reply_nb = plc_status_->modbus_input_bits_length ;

  MODBUS_MANAGER_RW(ReadInputBits)
}

int PlcTask::ModbusReadRegisters(int master_id, int slave_id,
    int addr, int nb) {

  uint16_t *tab_rp_regs = \
      (uint16_t *)(plc_status_->modbus_registers);

  int &reply_nb = plc_status_->modbus_registers_length;

  MODBUS_MANAGER_RW(ReadRegisters)
}

int PlcTask::ModbusReadInputRegisters(int master_id, int slave_id,
    int addr, int nb) {

  uint16_t *tab_rp_regs = \
      (uint16_t *)(plc_status_->modbus_input_registers);

  int &reply_nb = plc_status_->modbus_input_registers_length;

  MODBUS_MANAGER_RW(ReadInputRegisters)
}

int PlcTask::ModbusWriteBits(int master_id, int slave_id,
    int addr, int nb, const unsigned char *src) {

  uint8_t *tab_rp_regs = (uint8_t *)src;
  int reply_nb = 0;

  MODBUS_MANAGER_RW(WriteBits)
}

int PlcTask::ModbusWriteRegisters(int master_id, int slave_id,
    int addr, int nb, const unsigned short *src) {

  uint16_t *tab_rp_regs = (uint16_t *)src;
  int reply_nb = 0;

  MODBUS_MANAGER_RW(WriteRegisters)
}

int PlcTask::ModbusWriteBit(int master_id, int slave_id,
    int addr, int status) {

  int ret = 0;
  ModbusStation *station = modbus_manager_.GetStation(slave_id);
  if (station) {
    if (station->SlaveId() != slave_id) {
      station->SetSlaveId(slave_id);
    }
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

int PlcTask::ModbusWriteRegister(int master_id, int slave_id,
    int addr, int value) {
 
  int ret = 0;
  ModbusStation *station = modbus_manager_.GetStation(slave_id);
  if (station) {
    if (station->SlaveId() != slave_id) {
      station->SetSlaveId(slave_id);
    }
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

int PlcTask::ModbusWrite(NMLmsg *cmd) {
  MODBUS_WRITE_MSG *write_msg = (MODBUS_WRITE_MSG *)cmd;
  int rc = 0;
  switch (write_msg->type_) {
    case MB_REGISTER_BITS:
      if (write_msg->nb_ == 1) {
        rc = ModbusWriteBit(write_msg->master_id_, write_msg->slave_id_,
            write_msg->addr_, write_msg->bits[0]);

      } else {
        rc = ModbusWriteBits(write_msg->master_id_, write_msg->slave_id_,
            write_msg->addr_, write_msg->nb_, write_msg->bits);

      }
      break;
    case MB_REGISTER_REGISTERS:
      if (write_msg->nb_ == 1) {
        rc = ModbusWriteRegister(write_msg->master_id_, write_msg->slave_id_,
            write_msg->addr_, write_msg->registers[0]);

      } else {
        rc = ModbusWriteRegisters(write_msg->master_id_, write_msg->slave_id_,
            write_msg->addr_, write_msg->nb_, write_msg->registers);

      }
      break;
    default:
      break;
  }
  return rc;
}

int PlcTask::OpenGas(int gas_id) {
  gas_.Open(gas_id);
  return 0;
}

int PlcTask::OpenCuttingGas(int level) {
  int ret = gas_.Open(plc_args_[current_layer_].gas_args.gas_[level]);
  if (ret < 0) {
    return -1;
  } else {
    if (ret == GAS_OPEN_DELAY_FIRST) {
      delay_timeout_ = etime() + plc_global_args_.first_open_gas_delay;
    } else if (ret == GAS_OPEN_DELAY_SWITCH) {
      delay_timeout_ = etime() + plc_global_args_.switch_gas_delay;
    } else if (ret == GAS_OPEN_DELAY) {
      delay_timeout_ = etime() + plc_global_args_.open_gas_delay;
    }
  }
  return 0;
}

int PlcTask::SetCuttingPressure(int level) {
  return gas_.SetPressure(plc_args_[current_layer_].gas_args.gas_[level],
      plc_args_[current_layer_].gas_args.pressure_[level]);

}

int PlcTask::CuttingStay(int level) {
  delay_timeout_ = etime() + plc_args_[current_layer_].delay_args.stay_[level];
  return 0;
}

int PlcTask::CuttingBlow(int level) {
  if (plc_args_[current_layer_].delay_args.blow_enable_[level]) {
    delay_timeout_ = etime() + \
        plc_args_[current_layer_].delay_args.laser_off_blow_time_[level];
  }
  return 0;
}

int PlcTask::LaserOn() {
  return laser_.LaserOn();
}

int PlcTask::LaserOff() {
  return laser_.LaserOn();
}

int PlcTask::ShutterOn() {
  return laser_.ShutterOn();
}

int PlcTask::ShutterOff() {
  return laser_.ShutterOff();
}

int PlcTask::SetCuttingPower(int level) {
  return laser_.SetPower(\
      plc_args_[current_layer_].laser_args.peak_power_[level]);

}

int PlcTask::SetCuttingDutyRation(int level) {
  return laser_.SetDutyRatio(\
      plc_args_[current_layer_].laser_args.duty_ratio_[level]);
}

int PlcTask::SetCuttingPulseFrequency(int level) {
  return laser_.SetPulseFrequency(\
      plc_args_[current_layer_].laser_args.pulse_frequency_[level]);
}

int PlcTask::SetCuttingLaserType(int level) {
  return laser_.SetType(plc_args_[current_layer_].laser_args.type_[level]);
}
