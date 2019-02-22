#include "job/plcjob.h"

#include "plccfg.h"

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
      msg = shadow_list_.get();
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

#define APPEND_COMMAND(COMMAND_TYPE, COMMAND_ID) { \
  COMMAND_TYPE cmd; \
  cmd.cmd_id_ = COMMAND_ID; \
  command_list_.append(cmd); \
}

#define APPEND_COMMAND_LEVEL(COMMAND_TYPE, COMMAND_ID, BASE_ID) { \
  COMMAND_TYPE cmd; \
  cmd.cmd_id_ = COMMAND_ID; \
  cmd.level_ = COMMAND_ID - BASE_ID; \
  command_list_.append(cmd); \
}

int PlcJob::AppendCommand(short plc_cmd_id) {
  switch (plc_cmd_id) {
    case PLC_GAS_OPEN_CUTTING:
    case PLC_GAS_OPEN_FIRST:
    case PLC_GAS_OPEN_SECOND:
    case PLC_GAS_OPEN_THIRD:
      APPEND_COMMAND_LEVEL(GAS_OPEN_AUTO, plc_cmd_id, PLC_GAS_OPEN_CUTTING)
      break;
    case PLC_GAS_CLOSE_CUTTING:
    case PLC_GAS_CLOSE_FIRST:
    case PLC_GAS_CLOSE_SECOND:
    case PLC_GAS_CLOSE_THIRD:
      break;
    case PLC_GAS_PRESSURE_CUTTING:
    case PLC_GAS_PRESSURE_FIRST:
    case PLC_GAS_PRESSURE_SECOND:
    case PLC_GAS_PRESSURE_THIRD:
      APPEND_COMMAND_LEVEL(GAS_PRESSURE_AUTO, plc_cmd_id,
          PLC_GAS_PRESSURE_CUTTING)

      break;
    // Follower Command
    case PLC_LHC_FOLLOW_CUTTING:
    case PLC_LHC_FOLLOW_FIRST:
    case PLC_LHC_FOLLOW_SECOND:
    case PLC_LHC_FOLLOW_THIRD:
      APPEND_COMMAND_LEVEL(LHC_FOLLOW_AUTO, plc_cmd_id, PLC_LHC_FOLLOW_CUTTING)
      break;
    case PLC_LHC_PROGRESSIVE_CUTTING:
    case PLC_LHC_PROGRESSIVE_FIRST:
    case PLC_LHC_PROGRESSIVE_SECOND:
    case PLC_LHC_PROGRESSIVE_THIRD:
      APPEND_COMMAND_LEVEL(LHC_INCR_FOLLOW_AUTO,
          plc_cmd_id, PLC_LHC_PROGRESSIVE_CUTTING)

      break;
    case PLC_LHC_LIFT:
      APPEND_COMMAND(LHC_LIFT_AUTO, PLC_LHC_LIFT)
      break;
    case PLC_LHC_Z_AXIS_HOLD:
      APPEND_COMMAND(LHC_AXIS_HOLD, PLC_LHC_Z_AXIS_HOLD)
      break;
    // Laser Command
    case PLC_LASER_ON:
      APPEND_COMMAND(LASER_ON, PLC_LASER_ON)
      break;
    case PLC_LASER_OFF:
      APPEND_COMMAND(LASER_OFF, PLC_LASER_OFF)
      break;
    case PLC_LASER_SHUTTER_ON:
      APPEND_COMMAND(LASER_SHUTTER_ON, PLC_LASER_SHUTTER_ON)
      break;
    case PLC_LASER_SHUTTER_OFF:
      APPEND_COMMAND(LASER_SHUTTER_OFF, PLC_LASER_SHUTTER_OFF)
      break;

    case PLC_LASER_POWER_CUTTING:
    case PLC_LASER_POWER_FIRST:
    case PLC_LASER_POWER_SECOND:
    case PLC_LASER_POWER_THIRD:
      APPEND_COMMAND_LEVEL(LASER_POWER_AUTO, plc_cmd_id, PLC_LASER_POWER_CUTTING)
      break;
    case PLC_LASER_DUTYRATIO_CUTTING:
    case PLC_LASER_DUTYRATIO_FIRST:
    case PLC_LASER_DUTYRATIO_SECOND:
    case PLC_LASER_DUTYRATIO_THIRD:
      APPEND_COMMAND_LEVEL(LASER_DUTYRATIO_AUTO, plc_cmd_id,
          PLC_LASER_DUTYRATIO_CUTTING)

      break;
    case PLC_LASER_PULSEFREQ_CUTTING:
    case PLC_LASER_PULSEFREQ_FIRST:
    case PLC_LASER_PULSEFREQ_SECOND:
    case PLC_LASER_PULSEFREQ_THIRD:
      APPEND_COMMAND_LEVEL(LASER_PULSE_FREQ_AUTO, plc_cmd_id,
          PLC_LASER_PULSEFREQ_CUTTING)

      break;
    case PLC_LASER_TYPE_CUTTING:
    case PLC_LASER_TYPE_FIRST:
    case PLC_LASER_TYPE_SECOND:
    case PLC_LASER_TYPE_THIRD:
      APPEND_COMMAND_LEVEL(LASER_TYPE_AUTO, plc_cmd_id, PLC_LASER_TYPE_CUTTING)
      break;
    // Delay Command
    case PLC_DELAY_BLOW_CUTTING:
    case PLC_DELAY_BLOW_FIRST:
    case PLC_DELAY_BLOW_SECOND:
    case PLC_DELAY_BLOW_THIRD:
      APPEND_COMMAND_LEVEL(DELAY_BLOW_AUTO, plc_cmd_id, PLC_DELAY_BLOW_CUTTING)
      break;
    case PLC_DELAY_STAY_CUTTING:
    case PLC_DELAY_STAY_FIRST:
    case PLC_DELAY_STAY_SECOND:
    case PLC_DELAY_STAY_THIRD:
      APPEND_COMMAND_LEVEL(DELAY_STAY_AUTO, plc_cmd_id, PLC_DELAY_STAY_CUTTING)
      break;
    // Focus Command
    case PLC_FOCUS_POSITION_CUTTING:
    case PLC_FOCUS_POSITION_FIRST:
    case PLC_FOCUS_POSITION_SECOND:
    case PLC_FOCUS_POSITION_THIRD:
      APPEND_COMMAND_LEVEL(FOCUS_POSITION_AUTO, plc_cmd_id,
          PLC_FOCUS_POSITION_CUTTING)

      break;
  }
  return 0;
}
