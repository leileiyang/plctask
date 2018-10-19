#ifndef JOB_JOBMANAGER_H_
#define JOB_JOBMANAGER_H_

#include <map>

#include "job/plcjob.hh"
#include "nml_intf/plc_nml.hh"

class JobManager {
 public:
  ~JobManager();
  int AppendCommand(NMLmsg *msg);
  PlcJob* GetPlcJob(int job_id);
 private:
  std::map<int, PlcJob *> jobs_;

};

#endif // JOB_JOBMANAGER_H_
