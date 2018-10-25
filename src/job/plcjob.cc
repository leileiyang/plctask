#include "job/plcjob.h"

#include <string.h>

void PlcJob::ArrangeJob(NML_INTERP_LIST &task_list) {
  NMLmsg *msg = NULL;
  if (command_list_.len()) {
    while (command_list_.len()) {
      msg = command_list_.get();
      task_list.append(msg);
      shadow_list_.append(msg);
    }
    command_list_.clear();
  } else {
    while (shadow_list_.len()) {
      msg = command_list_.get();
      task_list.append(msg);
      command_list_.append(msg);
    }
    shadow_list_.clear();
  }
}

int PlcJob::AppendCommand(NMLmsg *nml_msg) {
  command_list_.append(nml_msg);
  return 0;
}

void PlcJob::Clear() {
  command_list_.clear();
  shadow_list_.clear();
}
