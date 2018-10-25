#ifndef JOB_PLCJOB_H_
#define JOB_PLCJOB_H_

#include "nml_intf/plc_nml.h"
#include "nml_intf/interpl.h"

class PlcJob {
 public:
  void ArrangeJob(NML_INTERP_LIST &task_list);
  int AppendCommand(NMLmsg *nml_msg);
  void Clear();

 private:
  NML_INTERP_LIST command_list_;
  NML_INTERP_LIST shadow_list_;

};

#endif // JOB_PLCJOB_H_
