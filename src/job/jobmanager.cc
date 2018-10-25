#include "job/jobmanager.h"

JobManager::~JobManager() {
  std::map<int, PlcJob *>::iterator it = jobs_.begin();
  for (; it != jobs_.end(); it++) {
    delete it->second;
  }
  jobs_.clear();
}

int JobManager::AppendCommand(NMLmsg *msg) {
  JOB_CMD_MSG *cmd = (JOB_CMD_MSG *)msg;
  int cmd_id = cmd->cmd_id_;
  int job_id = cmd->job_id_;
  cmd->job_id_ = -1; // erase the job id
  
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
