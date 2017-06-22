#ifndef SENDMSG_HH_
#define SENDMSG_HH_

#include <pthread.h>
#include <timer.hh>
#include <cmd_msg.hh>

#include "nml_intf/interpl.hh"

class SendMsgEngine {
  enum SendResult {
    ERROR = -1,
    SUCCESS = 0,
    TIMEOUT = 1
  };

 public:
  SendMsgEngine(double interval);
  int AppendMsg(RCS_CMD_MSG *msg, int instancy = 0);
  void SetCmdChannel(RCS_CMD_CHANNEL *cmd_channel);
  void Start();
  void Run();
  void Exit();

 private:
  RCS_TIMER timer_;
  bool exit_;
  RCS_CMD_CHANNEL *cmd_channel_;
  
  NML_INTERP_LIST imme_msgs_;
  NML_INTERP_LIST comm_msgs_;
  
  pthread_mutex_t imme_mutex_;
  pthread_mutex_t comm_mutex_;

  const static int kRetryCount;

  int SendMsg(RCS_CMD_MSG &msg);

};

#endif // SENDMSG_HH_
