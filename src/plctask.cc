#include "plctask.hh"

#include <nml_oi.hh>
#include "nml_intf/plc_nml.hh"

#include <iostream>


PLCTask::PLCTask(double sleep_time):
    state_(PLC_STATE_IDLE),
    timer_(sleep_time),
    running_(true),
    plc_cmd_buffer_(NULL),
    plc_stat_buffer_(NULL),
    plc_err_buffer_(NULL),
    plc_command_(NULL) {}

PLCTask::~PLCTask() {}

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
  plc_command_ = plc_cmd_buffer_->get_address();

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
  //plc_status_ = new RCS_STAT_MSG;

  return true;

}

int PLCTask::Plan() {
  return 0;
}

bool PLCTask::Run() {
  double start_time = etime();
  double end_time = 0;
  while (running_) {
    end_time = etime();
    /// Job
    // 1 read a command
    if (0 != plc_cmd_buffer_->peek()) {
    }
    
    std::cout << end_time - start_time << std::endl; 
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
}
