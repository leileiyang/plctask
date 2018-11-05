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
    case GAS_OPEN_CUTTING:
    case GAS_OPEN_FIRST:
    case GAS_OPEN_SECOND:
    case GAS_OPEN_THIRD:
      APPEND_COMMAND_LEVEL(OPEN_CUTTING_GAS, plc_cmd_id, GAS_OPEN_CUTTING)
      break;
    case GAS_CLOSE_CUTTING:
    case GAS_CLOSE_FIRST:
    case GAS_CLOSE_SECOND:
    case GAS_CLOSE_THIRD:
      break;
    case GAS_PRESSURE_CUTTING:
    case GAS_PRESSURE_FIRST:
    case GAS_PRESSURE_SECOND:
    case GAS_PRESSURE_THIRD:
      APPEND_COMMAND_LEVEL(SET_CUTTING_PRESSURE,
          plc_cmd_id, GAS_PRESSURE_CUTTING)

      break;
    // Follower Command
    case LHC_FOLLOW_CUTTING:
    case LHC_FOLLOW_FIRST:
    case LHC_FOLLOW_SECOND:
    case LHC_FOLLOW_THIRD:
      APPEND_COMMAND_LEVEL(LHC_FOLLOW, plc_cmd_id, LHC_FOLLOW_CUTTING)
      break;
    case LHC_PROGRESSIVE_CUTTING:
    case LHC_PROGRESSIVE_FIRST:
    case LHC_PROGRESSIVE_SECOND:
    case LHC_PROGRESSIVE_THIRD:
      APPEND_COMMAND_LEVEL(LHC_PROGRESSIVE_FOLLOW,
          plc_cmd_id, LHC_PROGRESSIVE_CUTTING)

      break;
    case LHC_LIFT:
      APPEND_COMMAND(LHC_LIFT_CMD, LHC_LIFT)
      break;
    case LHC_Z_AXIS_HOLD:
      APPEND_COMMAND(LHC_Z_AXIS_HOLD_CMD, LHC_Z_AXIS_HOLD)
      break;
    // Laser Command
    case LASER_ON:
      APPEND_COMMAND(LASER_ON_CMD, LASER_ON)
      break;
    case LASER_OFF:
      APPEND_COMMAND(LASER_OFF_CMD, LASER_OFF)
      break;
    case LASER_SHUTTER_ON:
      APPEND_COMMAND(LASER_SHUTTER_ON_CMD, LASER_SHUTTER_ON)
      break;
    case LASER_SHUTTER_OFF:
      APPEND_COMMAND(LASER_SHUTTER_OFF_CMD, LASER_SHUTTER_OFF)
      break;

    case LASER_POWER_CUTTING:
    case LASER_POWER_FIRST:
    case LASER_POWER_SECOND:
    case LASER_POWER_THIRD:
      APPEND_COMMAND_LEVEL(LASER_POWER, plc_cmd_id, LASER_POWER_CUTTING)
      break;
    case LASER_DUTYRATIO_CUTTING:
    case LASER_DUTYRATIO_FIRST:
    case LASER_DUTYRATIO_SECOND:
    case LASER_DUTYRATIO_THIRD:
      APPEND_COMMAND_LEVEL(LASER_DUTYRATIO, plc_cmd_id, LASER_DUTYRATIO_CUTTING)
      break;
    case LASER_PULSEFREQ_CUTTING:
    case LASER_PULSEFREQ_FIRST:
    case LASER_PULSEFREQ_SECOND:
    case LASER_PULSEFREQ_THIRD:
      APPEND_COMMAND_LEVEL(LASER_PULSE_FREQUENCY, plc_cmd_id, LASER_PULSEFREQ_CUTTING)
      break;
    case LASER_TYPE_CUTTING:
    case LASER_TYPE_FIRST:
    case LASER_TYPE_SECOND:
    case LASER_TYPE_THIRD:
      APPEND_COMMAND_LEVEL(LASER_TYPE, plc_cmd_id, LASER_TYPE_CUTTING)
      break;
    // Delay Command
    case DELAY_BLOW_CUTTING:
    case DELAY_BLOW_FIRST:
    case DELAY_BLOW_SECOND:
    case DELAY_BLOW_THIRD:
      APPEND_COMMAND_LEVEL(CUTTING_DELAY_BLOW, plc_cmd_id, DELAY_BLOW_CUTTING)
      break;
    case DELAY_STAY_CUTTING:
    case DELAY_STAY_FIRST:
    case DELAY_STAY_SECOND:
    case DELAY_STAY_THIRD:
      APPEND_COMMAND_LEVEL(CUTTING_DELAY_STAY, plc_cmd_id, DELAY_STAY_CUTTING)
      break;
    // Focus Command
    case FOCUS_POSITION_CUTTING:
    case FOCUS_POSITION_FIRST:
    case FOCUS_POSITION_SECOND:
    case FOCUS_POSITION_THIRD:
      APPEND_COMMAND_LEVEL(FOCUS_POSITION, plc_cmd_id, FOCUS_POSITION_CUTTING)
      break;
  }
  return 0;
}
