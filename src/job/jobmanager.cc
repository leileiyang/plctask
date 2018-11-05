#include "job/jobmanager.h"

JobManager::~JobManager() {
  std::map<int, PlcJob *>::iterator it = jobs_.begin();
  for (; it != jobs_.end(); it++) {
    delete it->second;
  }
  jobs_.clear();
}

int JobManager::AppendCommand(NMLmsg *msg) {
  PLC_CMD_MSG *cmd = (PLC_CMD_MSG *)msg;
  int cmd_id = cmd->cmd_id_;
  int job_id = cmd->job_id_;
  cmd->exec_ = 1; // set the executable flag 
  
  std::map<int, PlcJob *>::iterator it = jobs_.find(job_id);
  if (it != jobs_.end()) { // find the job
    if (cmd_id == 0) {
      if (it->second) {
        delete it->second;
      }
      jobs_.erase(it);
      PlcJob *new_job = new PlcJob;
      new_job->AppendCommand(msg);
      jobs_[job_id] = new_job;
    } else {
      (it->second)->AppendCommand(msg);
    }
  } else {
    PlcJob *new_job = new PlcJob;
    new_job->AppendCommand(msg);
    jobs_[job_id] = new_job;
  }
  return 0;
}

PlcJob* JobManager::GetPlcJob(int job_id) {
  return jobs_[job_id];
}

int JobManager::ParsePlcJob(NMLmsg *msg) {
  PLC_JOB_MSG *job = (PLC_JOB_MSG *)msg;
  std::map<int, PlcJob *>::iterator it = jobs_.find(job->plc_job_id_);
  if (it != jobs_.end()) { // find the job
    if (it->second) {
      delete it->second;
    }
    jobs_.erase(it);
  }

  PlcJob *new_job = new PlcJob;
  for (int i = 0; i < job->plc_cmds_length; i++) {
    new_job->AppendCommand(job->plc_cmds[i]);
  }
  jobs_[job->plc_job_id_] = new_job;
  return 0;
}
