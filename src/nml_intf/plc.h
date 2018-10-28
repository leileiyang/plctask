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

#define OPEN_GAS_CMD_TYPE                ((NMLTYPE) 220)
#define OPEN_CUTTING_GAS_TYPE            ((NMLTYPE) 221)
#define SET_CUTTING_PRESSURE_TYPE        ((NMLTYPE) 222)

#define IO_CFG_MSG_TYPE                  ((NMLTYPE) 250)

#endif
