#include "sendmsgengine.hh"

#include "plcclient.hh"

const int SendMsgEngine::kRetryCount = 3;

SendMsgEngine::SendMsgEngine(double interval):
    timer_(interval), exit_(false), cmd_channel_(NULL) {

  imme_msgs_.clear();
  comm_msgs_.clear();

  pthread_mutex_init(&imme_mutex_, NULL);
  pthread_mutex_init(&comm_mutex_, NULL);
}

void SendMsgEngine::SetCmdChannel(RCS_CMD_CHANNEL *cmd_channel) {
  cmd_channel_ = cmd_channel;
}

int SendMsgEngine::AppendMsg(RCS_CMD_MSG *msg, int instancy) {
  int ret = 0;
  if (instancy) {
    pthread_mutex_lock(&imme_mutex_);
    ret = imme_msgs_.append(msg);  
    pthread_mutex_unlock(&imme_mutex_);
  } else {
    pthread_mutex_lock(&comm_mutex_);
    ret = comm_msgs_.append(msg);
    pthread_mutex_unlock(&comm_mutex_);
  }
  return ret;
}

void SendMsgEngine::Exit() {
  exit_ = true;
}

int SendMsgEngine::SendMsg(RCS_CMD_MSG &msg) {
  int i = 0;
  while ((cmd_channel_->check_if_read() == 0) && i < kRetryCount) {
    i++;
    timer_.wait();
  }
  if (i == kRetryCount) {
    return TIMEOUT;
  }
  return cmd_channel_->write(msg);
}

void SendMsgEngine::Run() {
  NMLmsg *msg = NULL;
  int ret = 0;
  int i = kRetryCount;
  while (!exit_) {
    // Get immediate message if there is one.
    if (ret == SUCCESS) {
      pthread_mutex_lock(&imme_mutex_);
      msg = imme_msgs_.get();
      pthread_mutex_unlock(&imme_mutex_);
      i = kRetryCount;
    }
    if (msg) {
      ret = SendMsg(*(RCS_CMD_MSG*)msg);
    } else {
      // Get common message if there is one
      if (ret == SUCCESS) {
        pthread_mutex_lock(&comm_mutex_);
        msg = comm_msgs_.get();
        pthread_mutex_unlock(&comm_mutex_);
        i = kRetryCount;
      }
      if (msg) {
        ret = SendMsg(*(RCS_CMD_MSG*)msg);
      }
    }
    if (ret == TIMEOUT) {
      i--;
      if (i < 0) {
        // Skip the current message
        ret = SUCCESS;
      }
    }

    timer_.wait();
  }
}
