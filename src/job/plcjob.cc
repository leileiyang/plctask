#include "job/plcjob.hh"

#include <string.h>

void PlcJob::ArrangeJob(NML_INTERP_LIST &task_list) {
  NMLmsg *msg = NULL;
  if (command_list_.len()) {
    while (command_list_.len()) {
      NMLmsg *msg = command_list_.get();
      task_list.append(msg);
      shadow_list_.append(msg);
    }
    command_list_.clear();
  } else {
    while (shadow_list_.len()) {
      NMLmsg *msg = command_list_.get();
      task_list.append(msg);
      command_list_.append(msg);
    }
    shadow_list_.clear();
  }
}

int PlcJob::AppendCommand(NMLmsg *nml_msg) {
  switch (nml_msg->type) {
    case JOB_MODBUS_WRITE_MSG_TYPE: {
      JOB_MODBUS_WRITE_MSG *cmd = (JOB_MODBUS_WRITE_MSG *)nml_msg;
      MODBUS_WRITE_MSG msg;
      msg.master_id_ = cmd->master_id_;
      msg.slave_id_ = cmd->slave_id_;
      msg.type_ = cmd->type_;
      msg.addr_ = cmd->addr_;
      msg.nb_ = cmd->nb_;
      msg.bits_length = cmd->bits_length;
      msg.registers_length = cmd->registers_length;
      memmove(msg.bits, cmd->bits, sizeof(unsigned char) * cmd->bits_length);
      memmove(msg.registers, cmd->registers,
          sizeof(unsigned short) * cmd->registers_length);

      command_list_.append(msg);
                                    }
      break;
    default:
      break;
  }
}

void PlcJob::Clear() {
  command_list_.clear();
  shadow_list_.clear();
}
