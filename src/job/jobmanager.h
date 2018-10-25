#ifndef JOB_JOBMANAGER_H_
#define JOB_JOBMANAGER_H_

#include <map>

#include "job/plcjob.h"
#include "nml_intf/plc_nml.h"

class JobManager {
 public:
  ~JobManager();
  int AppendCommand(NMLmsg *msg);
  PlcJob* GetPlcJob(int job_id);
 private:
  std::map<int, PlcJob *> jobs_;

};

#endif // JOB_JOBMANAGER_H_
