#include "plctask.hh"

#include <nml_oi.hh>
#include "nml_intf/plc_nml.hh"
#include <rcs_prnt.hh>

#include <iostream>

PLCTask::PLCTask(double sleep_time):
    exec_state_(PLC_TASK_EXEC_DONE),
    state_(PLC_STATE_IDLE),
    timer_(sleep_time),
    running_(true),
    plc_cmd_buffer_(NULL),
    plc_stat_buffer_(NULL),
    plc_err_buffer_(NULL),
    plc_command_(NULL),
    plc_status_(NULL),
    plc_task_cmd_(NULL),
    plan_error_(0),
    execute_error_(0) {}

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
        new NML(nmlErrorFormat, "plcStatus", "plc", plc_nmlfile.c_str());

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

  return true;

}

int PLCTask::TaskIssueCommand(NMLmsg *cmd) {
  switch (cmd->type) {
    case FIRST_CMD_MSG_TYPE:
      rcs_print("Execute FIRST_CMD_MSG_TYPE!\n");
      break;
    case SECOND_CMD_MSG_TYPE:
      rcs_print("Execute SECOND_CMD_MSG_TYPE!\n");
      break;
    default:
      break;
  }
  return 0;
}

int PLCTask::TaskQueueCommand(NMLmsg *cmd) {
  if (cmd == 0) {
    return 0;
  }
  plc_list_.append(cmd);
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
      // immediate command
      // queue command
    case TEST_CMD_MSG_TYPE:
    case FIRST_CMD_MSG_TYPE:
    case SECOND_CMD_MSG_TYPE:
      TaskQueueCommand(plc_command_);
      break;
    default:
      break;
  }
  return 0;
}

int PLCTask::Execute() {
  int retval = 0;
  switch (exec_state_) {
    case PLC_TASK_EXEC_ERROR:
      plc_list_.clear();
      exec_state_ = PLC_TASK_EXEC_DONE;
      break;
    case PLC_TASK_EXEC_DONE:
      if (0 == plc_task_cmd_) {
        plc_task_cmd_ = plc_list_.get();
        if (0 != plc_task_cmd_) {
          exec_state_ = (enum PLC_TASK_EXEC_ENUM)
              TaskCheckPreconditions(plc_task_cmd_);

        }
      } else {
        if (0 != TaskIssueCommand(plc_task_cmd_)) {
          exec_state_ = PLC_TASK_EXEC_ERROR;
          retval = -1;
        } else {
          exec_state_ = (enum PLC_TASK_EXEC_ENUM)
              TaskCheckPostconditions(plc_task_cmd_);

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


int PLCTask::TaskCheckPreconditions(NMLmsg *cmd) {
  if (0 == cmd) {
    return PLC_TASK_EXEC_DONE;
  }
  switch (cmd->type) {
    case TEST_CMD_MSG_TYPE:
    case FIRST_CMD_MSG_TYPE:
    case SECOND_CMD_MSG_TYPE:
      return PLC_TASK_EXEC_DONE;
    default:
      return PLC_TASK_EXEC_WAITING_FOR_DEVICES;
  }
  return PLC_TASK_EXEC_DONE;
}

int PLCTask::TaskCheckPostconditions(NMLmsg *cmd) {
  if (0 == cmd) {
    return PLC_TASK_EXEC_DONE;
  }
  switch (cmd->type) {
    default:
      return PLC_TASK_EXEC_DONE;
  }
  return PLC_TASK_EXEC_DONE;

}

bool PLCTask::Run() {
  double start_time = etime();
  double end_time = 0;
  while (running_) {
    end_time = etime();
    /// Job
    // 1 read a command
    if (0 < plc_cmd_buffer_->read()) {
      plc_command_ = plc_cmd_buffer_->get_address();
      plan_error_ = 0;
      execute_error_ = 0;
    }
    if (0 != Plan()) {
      plan_error_ = 0;
    }
    if (0 != Execute()) {
      execute_error_ = 0;
    }

    UpdateDevicesStatus();

    // do top level
    plc_status_->echo_serial_number = plc_command_->serial_number;
    plc_status_->command_type = plc_command_->type;
    
    //std::cout << end_time - start_time << std::endl;
    start_time = end_time;
    timer_.wait();
  }
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
}

int PLCTask::UpdateDevicesStatus() {
  return 0;
}
