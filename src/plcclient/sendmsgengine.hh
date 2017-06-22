#ifndef SENDMSG_HH_
#define SENDMSG_HH_

#include <pthread.h>
#include <timer.hh>
#include <cmd_msg.hh>

#include "nml_intf/interpl.hh"

class PLCClient;

class SendMsgEngine {
 public:
  SendMsgEngine(PLCClient *client, double interval);
  int AppendMsg(RCS_CMD_MSG *msg, int instancy = 0);
  void Run();
  void Exit();

 private:
  PLCClient *client_;
  RCS_TIMER timer_;
  bool exit_;
  
  NML_INTERP_LIST imme_msgs_;
  NML_INTERP_LIST comm_msgs_;
  
  pthread_mutex_t imme_mutex_;
  pthread_mutex_t comm_mutex_;

  const static int kRetryCount;

};

#endif // SENDMSG_HH_
