#ifndef NML_INTF_PLC_H_
#define NML_INTF_PLC_H_

#define PLC_STAT_TYPE                    ((NMLTYPE) 101)
#define FIRST_CMD_MSG_TYPE               ((NMLTYPE) 103)
#define SECOND_CMD_MSG_TYPE              ((NMLTYPE) 104)
#define MODBUS_CMD_MSG_TYPE              ((NMLTYPE) 107)
#define MODBUS_READ_MSG_TYPE             ((NMLTYPE) 108)
#define MODBUS_INIT_MSG_TYPE             ((NMLTYPE) 109)
#define MODBUS_WRITE_MSG_TYPE            ((NMLTYPE) 110)

#define JOB_CMD_MSG_TYPE                 ((NMLTYPE) 200)
#define JOB_ABORT_MSG_TYPE               ((NMLTYPE) 202)

#define PLC_JOB_MSG_TYPE                 ((NMLTYPE) 219)

#define OPEN_GAS_TYPE                    ((NMLTYPE) 220)
#define OPEN_CUTTING_GAS_TYPE            ((NMLTYPE) 221)
#define SET_CUTTING_PRESSURE_TYPE        ((NMLTYPE) 222)

#define LHC_FOLLOW_TYPE                  ((NMLTYPE) 230)
#define LHC_PROGRESSIVE_FOLLOW_TYPE      ((NMLTYPE) 231)
#define LHC_LIFT_CMD_TYPE                ((NMLTYPE) 232)
#define LHC_Z_AXIS_HOLD_CMD_TYPE         ((NMLTYPE) 233)

#define LASER_ON_CMD_TYPE                ((NMLTYPE) 240)
#define LASER_OFF_CMD_TYPE               ((NMLTYPE) 241)
#define LASER_SHUTTER_ON_CMD_TYPE        ((NMLTYPE) 242)
#define LASER_SHUTTER_OFF_CMD_TYPE       ((NMLTYPE) 243)
#define LASER_POWER_TYPE                 ((NMLTYPE) 244)
#define LASER_DUTYRATIO_TYPE             ((NMLTYPE) 245)
#define LASER_PULSE_FREQUENCY_TYPE       ((NMLTYPE) 246)
#define LASER_TYPE_TYPE                  ((NMLTYPE) 247)

#define CUTTING_DELAY_BLOW_TYPE          ((NMLTYPE) 250)
#define CUTTING_DELAY_STAY_TYPE          ((NMLTYPE) 251)

#define FOCUS_POSITION_TYPE              ((NMLTYPE) 260)

#define IO_CFG_MSG_TYPE                  ((NMLTYPE) 280)

#endif
