#include <timer.hh>
#include <rcs_prnt.hh>
#include <nml_srv.hh>
#include <cmd_msg.hh>
#include <nml_oi.hh>
#include <stat_msg.hh>
#include <math.h>

#include "nml_intf/plc_nml.h"

static RCS_CMD_CHANNEL *plcCommandChannel = NULL;
static RCS_STAT_CHANNEL *plcStatusChannel = NULL;
static NML *plcErrorChannel = NULL;

int main(int argc, char *argv[]) {
  double start_time = etime();
  //char *plc_nmlfile = argv[1];
  const char *plc_nmlfile = "plctask.nml";
  while (fabs(etime() - start_time) < 10.0 && (plcCommandChannel == NULL ||
      plcStatusChannel == NULL || plcErrorChannel == NULL)) {

    if (NULL == plcCommandChannel) {
      rcs_print("plcCommandChannel == NULL, attempt to create\n");
      plcCommandChannel =
          new RCS_CMD_CHANNEL(plcFormat, "plcCommand", "plcsvr", plc_nmlfile);

    }
    if (NULL == plcStatusChannel) {
      rcs_print("plcStatusChannel == NULL, attempt to create\n");
      plcStatusChannel =
          new RCS_STAT_CHANNEL(plcFormat, "plcStatus", "plcsvr", plc_nmlfile);

    }
    if (NULL == plcErrorChannel) {
      rcs_print("plcErrorChannel == NULL, attempt to create\n");
      plcErrorChannel =
          new NML(nmlErrorFormat, "plcError", "plcsvr", plc_nmlfile);

    }

    if (!plcCommandChannel->valid()) {
      delete plcCommandChannel;
      plcCommandChannel = NULL;
      rcs_print("plcCommandChannel failed!\n");
    }
    if (!plcStatusChannel->valid()) {
      delete plcStatusChannel;
      plcStatusChannel = NULL;
      rcs_print("plcStatusChannel failed!\n");
    }
    if (!plcErrorChannel->valid()) {
      delete plcErrorChannel;
      rcs_print("plcErrorChannel failed!\n");
      plcErrorChannel = NULL;
    }
    esleep(0.200);
  }

  set_rcs_print_destination(RCS_PRINT_TO_STDERR);

  if (NULL == plcCommandChannel) {
    rcs_print("plcCommandChannel == NULL, attempt to create\n");
    plcCommandChannel =
      new RCS_CMD_CHANNEL(plcFormat, "plcCommand", "plcsvr", plc_nmlfile);

  }
  if (NULL == plcStatusChannel) {
    rcs_print("plcStatusChannel == NULL, attempt to create\n");
    plcStatusChannel =
      new RCS_STAT_CHANNEL(plcFormat, "plcStatus", "plcsvr", plc_nmlfile);

  }
  if (NULL == plcErrorChannel) {
    plcErrorChannel =
      new NML(nmlErrorFormat, "plcError", "plcsvr", plc_nmlfile);

  }
  if (!plcCommandChannel->valid() || !plcStatusChannel->valid() || !plcErrorChannel->valid()) {
    rcs_print("Build NML Channels Failed!\n");
    return -1;
  }
  rcs_print("Build NML Channels Success!\n");
  run_nml_servers();

  return 0;
}
