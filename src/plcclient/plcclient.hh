#ifndef PLCCLIENT_HH_
#define PLCCLIENT_HH_

#include <string>
#include <timer.hh>
#include <cmd_msg.hh>
#include <stat_msg.hh>

#include "nml_intf/interpl.hh"
#include "nml_intf/plc_nml.hh"

class PLCClient {
  enum SendResult {
    Error = -1,
    Success = 0,
    Timeout = 1
  };
 public:
  PLCClient(double sleep_time);
  ~PLCClient();

  int Startup(std::string plc_nmlfile);
  int SendMsg(RCS_CMD_MSG &);
  void Shutdown();

 private:
  RCS_TIMER timer_;
  bool running_;

  // communication channel
  RCS_CMD_CHANNEL *plc_cmd_buffer_;
  RCS_STAT_CHANNEL *plc_stat_buffer_;
  NML *plc_err_buffer_;

  static int try_count;

};

#endif
