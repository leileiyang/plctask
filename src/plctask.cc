#include "plctask.hh"

#include <stdio.h>
#include <string.h>
#include <errno.h>

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
  ModbusRelease();
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

int PLCTask::TaskIssueCommand(NMLmsg *cmd) {
  int retval = 0;
  switch (cmd->type) {
    case FIRST_CMD_MSG_TYPE:
      rcs_print("Execute FIRST_CMD_MSG_TYPE!\n");
      break;
    case SECOND_CMD_MSG_TYPE:
      rcs_print("Execute SECOND_CMD_MSG_TYPE!\n");
      break;
    case MODBUS_READ_MSG_TYPE:
      if (ModbusRead(cmd) >= 0) {
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
    case MODBUS_INIT_MSG_TYPE:
    case MODBUS_READ_MSG_TYPE:
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

        }
        plc_task_cmd_ = 0;
      }
      break;
    case PLC_TASK_EXEC_WAITING_FOR_DEVICES:
      break;
    default:
      break;
  }
  return 0;
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

void PLCTask::ModbusRelease() {
  if (modbus_ctx_) {
    modbus_close(modbus_ctx_);
    modbus_free(modbus_ctx_);
  }
}

int PLCTask::ModbusInit(NMLmsg *cmd) {
  MODBUS_INIT_MSG *msg = (MODBUS_INIT_MSG *)cmd;
  char ip_device[100] = {0};
  memcpy(ip_device, msg->ip_device, msg->ip_device_length);
  switch (msg->type_) {
    case MB_BACKEND_TCP:
      modbus_ctx_ = modbus_new_tcp(ip_device, msg->ip_port_);
      break;
    case MB_BACKEND_TCP_PI:
      break;
    case MB_BACKEND_RTU:
      modbus_ctx_ = modbus_new_rtu(ip_device, msg->baud_, 'N', 8, 1);
      if (modbus_ctx_) {
        modbus_set_slave(modbus_ctx_, msg->slave_id_);
      }
      break;
    default:
      break;
  }
  if (modbus_ctx_ == NULL) {
    memset(error_, 0, NML_ERROR_LEN);
    sprintf(error_, "Modbus Init Failed!\n");
    rcs_print("Modbus Init failed!\n");
    return -1;
  } else {
    if (modbus_connect(modbus_ctx_) == -1) {
      rcs_print("Connection failed: %s\n", modbus_strerror(errno));
      memset(error_, 0, NML_ERROR_LEN);
      sprintf(error_, "Connection failed: %s\n", modbus_strerror(errno));
      modbus_free(modbus_ctx_);
      return -2;
    }
  }
  rcs_print("Modbus Init Success!\n");
  return 0;
}

int PLCTask::ModbusRead(NMLmsg *cmd) {
  MODBUS_READ_MSG *read_msg = (MODBUS_READ_MSG *)cmd;
  int rc = 0;
  switch (read_msg->type_) {
    case MB_REGISTER_BITS:
      rc = ModbusReadBits(read_msg->addr_, read_msg->nb_);
      break;
    case MB_REGISTER_INPUT_BITS:
      rc = ModbusReadInputBits(read_msg->addr_, read_msg->nb_);
      break;
    case MB_REGISTER_REGISTERS:
      rc = ModbusReadRegisters(read_msg->addr_, read_msg->nb_);
      break;
    case MB_REGISTER_INPUT_REGISTERS:
      rc = ModbusReadInputRegisters(read_msg->addr_, read_msg->nb_);
      break;
    default:
      break;
  }
  return rc;
}

int PLCTask::ModbusReadBits(int addr, int nb) {
  uint8_t *tab_rp_bits = (uint8_t *)(plc_status_->modbus_registers_.bits);
  int try_count = 3;
  int rc = 0;
  do {
    int rc = modbus_read_bits(modbus_ctx_, addr, nb, tab_rp_bits);
    if (rc < 0) { // read error;
      try_count--; 
    } else {
      plc_status_->modbus_registers_.bits_length = rc;
      break;
    }
  } while (try_count > 0);
  if (try_count == 0) {
    // connect error
    rcs_print("modbus_read_bits failed!\n");
    memset(error_, 0, NML_ERROR_LEN);
    sprintf(error_, "modbus_read_bits failed!\n");
  }
  return rc;
}

int PLCTask::ModbusReadInputBits(int addr, int nb) {
  uint8_t *tab_rp_bits = (uint8_t *)(plc_status_->modbus_registers_.input_bits);
  int try_count = 3;
  int rc = 0;
  do {
    rc = modbus_read_input_bits(modbus_ctx_, addr, nb, tab_rp_bits);
    if (rc < 0) { // read error;
      try_count--; 
    } else {
      plc_status_->modbus_registers_.input_bits_length = rc;
      break;
    }
  } while (try_count > 0);
  if (try_count == 0) {
    // connect error
    rcs_print("modbus_read_input_bits failed!\n");
    memset(error_, 0, NML_ERROR_LEN);
    sprintf(error_, "modbus_read_input_bits failed!\n");
  }  
  return rc;
}

int PLCTask::ModbusReadRegisters(int addr, int nb) {
  uint16_t *tab_rp_registers = \
      (uint16_t *)(plc_status_->modbus_registers_.registers);

  int rc = 0;
  int try_count = 3;
  do {
    rc = modbus_read_registers(modbus_ctx_, addr, nb, tab_rp_registers);
    if (rc < 0) { // read error;
      try_count--; 
    } else {
      plc_status_->modbus_registers_.registers_length = rc;
      break;
    }
  } while (try_count > 0);
  if (try_count == 0) {
    // connect error
    rcs_print("modbus_read_registers failed!\n");
    memset(error_, 0, NML_ERROR_LEN);
    sprintf(error_, "modbus_read_registers failed!\n");
  }
  return rc;
}

int PLCTask::ModbusReadInputRegisters(int addr, int nb) {
  uint16_t *tab_rp_registers = \
      (uint16_t *)(plc_status_->modbus_registers_.input_registers);

  int try_count = 3;
  int rc = 0;
  do {
    rc = modbus_read_input_registers(modbus_ctx_, addr, nb, tab_rp_registers);
    if (rc < 0) { // read error;
      try_count--; 
    } else {
      plc_status_->modbus_registers_.input_registers_length = rc;
      break;
    }
  } while (try_count > 0);
  if (try_count == 0) {
    // connect error
    rcs_print("modbus_read_input_registers failed!\n");
    memset(error_, 0, NML_ERROR_LEN);
    sprintf(error_, "modbus_read_input_registers failed!\n");
  }
  return rc;
}
