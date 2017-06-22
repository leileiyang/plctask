#include "sendmsgengine.hh"

#include "plcclient.hh"

const int SendMsgEngine::kRetryCount = 3;

SendMsgEngine::SendMsgEngine(PLCClient *client, double interval):
    client_(client), timer_(interval), exit_(false) {

  imme_msgs_.clear();
  comm_msgs_.clear();

  pthread_mutex_init(&imme_mutex_, NULL);
  pthread_mutex_init(&comm_mutex_, NULL);
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

void SendMsgEngine::Run() {
  NMLmsg *msg = NULL;
  int ret = 0;
  int i = kRetryCount;
  while (!exit_) {
    // Get immediate message if there is one.
    if (ret == PLCClient::SUCCESS) {
      pthread_mutex_lock(&imme_mutex_);
      msg = imme_msgs_.get();
      pthread_mutex_unlock(&imme_mutex_);
      i = kRetryCount;
    }
    if (msg) {
      ret = client_->SendMsg(*(RCS_CMD_MSG*)msg);
    } else {
      // Get common message if there is one
      if (ret == PLCClient::SUCCESS) {
        pthread_mutex_lock(&comm_mutex_);
        msg = comm_msgs_.get();
        pthread_mutex_unlock(&comm_mutex_);
        i = kRetryCount;
      }
      if (msg) {
        ret = client_->SendMsg(*(RCS_CMD_MSG*)msg);
      }
    }
    if (ret == PLCClient::TIMEOUT) {
      i--;
      if (i < 0) {
        // Skip the current message
        ret = PLCClient::SUCCESS;
      }
    }

    timer_.wait();
  }
}
