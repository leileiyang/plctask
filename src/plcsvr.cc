#include <timer.hh>
#include <rcs_prnt.hh>
#include <nml_srv.hh>
#include <cmd_msg.hh>
#include <nml_oi.hh>
#include <stat_msg.hh>
#include <math.h>

#include "nml_intf/plc_nml.hh"

static RCS_CMD_CHANNEL *plcCommandChannel = NULL;
static RCS_STAT_CHANNEL *plcStatusChannel = NULL;
static NML *plcErrorChannel = NULL;

int main(int argc, char *argv[]) {
  double start_time = etime();
  char *plc_nmlfile = argv[1];
  while (fabs(etime() - start_time) < 10.0 && (plcCommandChannel == NULL ||
      plcStatusChannel == NULL || plcErrorChannel == NULL)) {

    if (NULL == plcCommandChannel) {
      rcs_print("plcCommmandChannel == NULL, attempt to create\n");
      plcCommandChannel =
          new RCS_CMD_CHANNEL(plcFormat, "plcCommand", "plcsvr", plc_nmlfile);

    }
    if (NULL == plcStatusChannel) {
      rcs_print("plcStatueChannel == NULL, attempt to create\n");
      plcStatusChannel = 
          new RCS_STAT_CHANNEL(plcFormat, "plcStatus", "plcsvr", plc_nmlfile);

    }
    if (NULL == plcErrorChannel) {
      plcErrorChannel =
          new NML(nmlErrorFormat, "plcError", "plcsvr", plc_nmlfile);

    }

    if (!plcCommandChannel->valid()) {
      delete plcCommandChannel;
      plcCommandChannel = NULL;
    }
    if (!plcStatusChannel->valid()) {
      delete plcStatusChannel;
      plcStatusChannel = NULL;
    }
    if (!plcErrorChannel->valid()) {
      delete plcErrorChannel;
      plcErrorChannel = NULL;
    }
    esleep(0.200);
  }

  set_rcs_print_destination(RCS_PRINT_TO_STDERR);

  if (NULL == plcCommandChannel) {
    rcs_print("plcCommmandChannel == NULL, attempt to create\n");
    plcCommandChannel =
      new RCS_CMD_CHANNEL(plcFormat, "plcCommand", "plcsvr", plc_nmlfile);

  }
  if (NULL == plcStatusChannel) {
    rcs_print("plcStatueChannel == NULL, attempt to create\n");
    plcStatusChannel = 
      new RCS_STAT_CHANNEL(plcFormat, "plcStatus", "plcsvr", plc_nmlfile);

  }
  if (NULL == plcErrorChannel) {
    plcErrorChannel =
      new NML(nmlErrorFormat, "plcError", "plcsvr", plc_nmlfile);

  }
  run_nml_servers();

  return 0;
}
