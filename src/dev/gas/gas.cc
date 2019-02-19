#include "gas.h"

#include "io_gas.h"
#include "../../nml_intf/plc_nml.h"

enum GAS_ID_ENUM {
  GAS_ID_AIR,
  GAS_ID_OXYGEN,
  GAS_ID_NITROGEN,
  GAS_ID_HIGH_AIR,
  GAS_ID_HIGH_OXYGEN,
  GAS_ID_HIGH_NITROGEN,
};

typedef std::pair<int, int> pair_t;

static const pair_t gas_array[] = {
  pair_t(GAS_ID_AIR, 0),
  pair_t(GAS_ID_OXYGEN, 1),
  pair_t(GAS_ID_NITROGEN, 2),
  pair_t(GAS_ID_HIGH_AIR, 3),
  pair_t(GAS_ID_HIGH_OXYGEN, 4),
  pair_t(GAS_ID_HIGH_NITROGEN, 5),
};

std::map<int, int> gas_id_pos_map(gas_array,
    gas_array + sizeof(gas_array) / sizeof(gas_array[0]));

int Gas::Open(int gas_id) {
  if (!enable_) {
    return GAS_OP_OK;
  }
  int ret = gas_intf_->Open(gas_id);
  if (ret) {
    ;
  } else {
    if (current_gas_ < 0) {
      ret = GAS_OPEN_FIRST_DELAY;
    } else if (current_gas_ != gas_id) {
      ret = GAS_OPEN_SWITCH_DELAY;
    }
    current_gas_ = gas_id;
    SetGasStatus(gas_id, 1);
  }
  return ret; 
}

int Gas::Close() {
  if (!enable_) {
    return 0;
  }
  gas_intf_->Close();
  SetGasStatus(current_gas_, 0);
  return 0;
}

int Gas::SetPressure(int gas_id, double pressure) {
  if (!enable_) {
    return 0;
  }
  int ret = gas_intf_->SetPressure(gas_id, pressure);
  if (ret) {
    ;
  } else {
    current_pressure_ = pressure;
    current_gas_ = gas_id;
  }
  return ret;
}

void Gas::UpdateCfg() {
  switch (intf_type_) {
    case GAS_INTF_IO:
      break;
    case GAS_INTF_MODBUS:
      break;
    default:
      break;
  }
}

void Gas::UpdateStatus(PLC_GAS_STAT &gas_stat) {
  gas_stat.current_gas_ = current_gas_;
  gas_stat.current_pressure_ = current_pressure_;
  gas_stat.gas_status_ = gas_status_;
  gas_stat.status_ = status_;
  gas_stat.alarm_status_ = alarm_status_;
}

void Gas::SetGasStatus(int gas_id, int on) {
  if (on) {
    gas_status_ |= 1UL << gas_id_pos_map[gas_id];
  } else {
    gas_status_ &= ~(1UL << gas_id_pos_map[gas_id]);
  }
}
