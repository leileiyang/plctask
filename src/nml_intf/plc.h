#ifndef NML_INTF_PLC_H_
#define NML_INTF_PLC_H_

#define PLC_STAT_TYPE                    ((NMLTYPE) 101)
#define FIRST_CMD_MSG_TYPE               ((NMLTYPE) 103)
#define SECOND_CMD_MSG_TYPE              ((NMLTYPE) 104)
#define MODBUS_CMD_MSG_TYPE              ((NMLTYPE) 107)
#define MODBUS_READ_MSG_TYPE             ((NMLTYPE) 108)
#define MODBUS_INIT_MSG_TYPE             ((NMLTYPE) 109)
#define MODBUS_WRITE_MSG_TYPE            ((NMLTYPE) 110)

#define PLC_CMD_MSG_TYPE                 ((NMLTYPE) 200)
#define JOB_ABORT_MSG_TYPE               ((NMLTYPE) 202)
#define PLC_EXEC_JOB_TYPE                ((NMLTYPE) 203)

#define PLC_JOB_MSG_TYPE                 ((NMLTYPE) 219)

#define GAS_OPEN_TYPE                    ((NMLTYPE) 220)
#define GAS_OPEN_AUTO_TYPE               ((NMLTYPE) 221)
#define GAS_PRESSURE_AUTO_TYPE           ((NMLTYPE) 222)
#define GAS_CLOSE_TYPE                   ((NMLTYPE) 223)

#define LHC_FOLLOW_AUTO_TYPE             ((NMLTYPE) 230)
#define LHC_INCR_FOLLOW_AUTO_TYPE        ((NMLTYPE) 231)
#define LHC_LIFT_AUTO_TYPE               ((NMLTYPE) 232)
#define LHC_AXIS_HOLD_TYPE               ((NMLTYPE) 233)
#define LHC_FOLLOW_TYPE                  ((NMLTYPE) 234)

#define LASER_ON_TYPE                    ((NMLTYPE) 240)
#define LASER_OFF_TYPE                   ((NMLTYPE) 241)
#define LASER_SHUTTER_ON_TYPE            ((NMLTYPE) 242)
#define LASER_SHUTTER_OFF_TYPE           ((NMLTYPE) 243)
#define LASER_POWER_AUTO_TYPE            ((NMLTYPE) 244)
#define LASER_DUTYRATIO_AUTO_TYPE        ((NMLTYPE) 245)
#define LASER_PULSE_FREQ_AUTO_TYPE       ((NMLTYPE) 246)
#define LASER_TYPE_AUTO_TYPE             ((NMLTYPE) 247)
#define LASER_ON_MANUAL_TYPE             ((NMLTYPE) 248)

#define DELAY_BLOW_AUTO_TYPE             ((NMLTYPE) 250)
#define DELAY_STAY_AUTO_TYPE             ((NMLTYPE) 251)

#define FOCUS_POSITION_AUTO_TYPE         ((NMLTYPE) 260)

#define IO_CFG_MSG_TYPE                  ((NMLTYPE) 280)

#endif
