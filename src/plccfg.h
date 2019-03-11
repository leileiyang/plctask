#ifndef PLCCFG_H_
#define PLCCFG_H_

#include <iostream>
#include <vector>

// Auto command macro format: device_operate_level
// e.g. GAS_OPEN_CUTTING
//      GAS_OPEN_FIRST
//      LHC_FOLLOW_CUTTING
//      LHC_FOLLOW_FIRST
//      LHC_PROGRESSIVE_FIRST

#define CRAFT_LAYERS 20 
#define STRIPING_LAYER_INDEX 18
#define COOLING_LAYER_INDEX 19

#define CRAFT_LEVELS 4

enum CRAFT_LEVEL {
  CRAFT_CUTTING = 0,
  CRAFT_FIRST = 1,
  CRAFT_SECOND = 2,
  CRAFT_THIRD = 3,

  CRAFT_STRIPING = 10,
  CRAFT_COOLING = 11,
};

enum PLC_CMD_ENUM {
  // 
  PLC_CMD_NONE = 0,
  // Gas Command
  PLC_GAS_OPEN_CUTTING = 100,
  PLC_GAS_OPEN_FIRST = 101,
  PLC_GAS_OPEN_SECOND = 102,
  PLC_GAS_OPEN_THIRD = 103,

  PLC_GAS_CLOSE_CUTTING = 104,
  PLC_GAS_CLOSE_FIRST = 105,
  PLC_GAS_CLOSE_SECOND = 106,
  PLC_GAS_CLOSE_THIRD = 107,

  PLC_GAS_PRESSURE_CUTTING = 108,
  PLC_GAS_PRESSURE_FIRST = 109,
  PLC_GAS_PRESSURE_SECOND = 110,
  PLC_GAS_PRESSURE_THIRD = 111,

  // Follower Command
  PLC_LHC_FOLLOW_CUTTING = 120,
  PLC_LHC_FOLLOW_FIRST = 121,
  PLC_LHC_FOLLOW_SECOND = 122,
  PLC_LHC_FOLLOW_THIRD = 123,

  PLC_LHC_PROGRESSIVE_CUTTING = 124,
  PLC_LHC_PROGRESSIVE_FIRST = 125,
  PLC_LHC_PROGRESSIVE_SECOND = 126,
  PLC_LHC_PROGRESSIVE_THIRD = 127,

  PLC_LHC_LIFT = 128,
  PLC_LHC_Z_AXIS_HOLD = 129,

  // Laser Command
  PLC_LASER_ON = 140,
  PLC_LASER_OFF = 141,
  PLC_LASER_SHUTTER_ON = 142,
  PLC_LASER_SHUTTER_OFF = 143,

  PLC_LASER_POWER_CUTTING = 144,
  PLC_LASER_POWER_FIRST = 145,
  PLC_LASER_POWER_SECOND = 146,
  PLC_LASER_POWER_THIRD = 147,

  PLC_LASER_DUTYRATIO_CUTTING = 148,
  PLC_LASER_DUTYRATIO_FIRST = 149,
  PLC_LASER_DUTYRATIO_SECOND = 150,
  PLC_LASER_DUTYRATIO_THIRD = 151,

  PLC_LASER_PULSEFREQ_CUTTING = 152, 
  PLC_LASER_PULSEFREQ_FIRST = 153, 
  PLC_LASER_PULSEFREQ_SECOND = 154, 
  PLC_LASER_PULSEFREQ_THIRD = 155, 

  PLC_LASER_TYPE_CUTTING = 156, 
  PLC_LASER_TYPE_FIRST = 157, 
  PLC_LASER_TYPE_SECOND = 158, 
  PLC_LASER_TYPE_THIRD = 159, 

  // Delay Command
  PLC_DELAY_BLOW_CUTTING = 184,
  PLC_DELAY_BLOW_FIRST = 185,
  PLC_DELAY_BLOW_SECOND = 186,
  PLC_DELAY_BLOW_THIRD = 187,

  PLC_DELAY_STAY_CUTTING = 188,
  PLC_DELAY_STAY_FIRST = 189,
  PLC_DELAY_STAY_SECOND = 190,
  PLC_DELAY_STAY_THIRD = 191,

  // Focus Command
  PLC_FOCUS_POSITION_CUTTING = 210,
  PLC_FOCUS_POSITION_FIRST = 211,
  PLC_FOCUS_POSITION_SECOND = 212,
  PLC_FOCUS_POSITION_THIRD = 213,

  // Alarm function id
  PLC_GAS_LPRESSURE_ALARM = 300,
  PLC_GAS_HPRESSURE_ALARM = 301,
  PLC_GAS_AIR_ALARM = 302,
  PLC_GAS_O2_ALARM = 303,
  PLC_GAS_N2_ALARM = 304,

  // Gas function id
  PLC_GAS_AIR = 400,
  PLC_GAS_O2 = 401,
  PLC_GAS_N2 = 402,
  PLC_GAS_HIGH_AIR = 403,
  PLC_GAS_HIGH_O2 = 404,
  PLC_GAS_HIGH_N2 = 405,

  // Laser function id
  PLC_LASER_SWITCH = 420,
  PLC_LASER_SHUTTER = 421,

};

struct ProcessArgs {
  bool no_lift;
  bool keep_air;
  bool no_follow;
  bool skip;
  bool striping;
  bool pre_pierce;
  int cutting;
  bool cooling;
  int craft_level;
};

class GasArgs {
 public:
  GasArgs(): gas_(CRAFT_LEVELS, 0), pressure_(CRAFT_LEVELS, 0.){}

  std::vector<int> gas_;
  std::vector<double> pressure_;
  void Show() {
    for (int i = 0; i < CRAFT_LEVELS; i++) {
      std::cout << gas_[i] << " pressure:" << pressure_[i] << std::endl;
    }
  }
};

class LaserArgs {
 public:
  LaserArgs(): peak_power_(CRAFT_LEVELS, 100), duty_ratio_(CRAFT_LEVELS, 100),
      pulse_frequency_(CRAFT_LEVELS, 5000), type_(CRAFT_LEVELS, 0) {}

  std::vector<double> peak_power_;
  std::vector<double> duty_ratio_;
  std::vector<int> pulse_frequency_;
  std::vector<int> type_;
  
  void Show() {
    for (int i = 0; i < CRAFT_LEVELS; i++) {
      std::cout << "peak power:" << peak_power_[i]
          << " duty ratio:" << duty_ratio_[i]
          << " pulse frequency:" << pulse_frequency_[i] 
          << " laser type:" << type_[i] << std::endl;

    }
  }
};

class FollowerArgs {
 public:
  FollowerArgs(): height_(CRAFT_LEVELS, 0.), incr_enable_(CRAFT_LEVELS, false),
      incr_time_(CRAFT_LEVELS, 0.), lift_height_(0.), no_follow_(false) {}

  std::vector<double> height_;
  std::vector<bool> incr_enable_;
  std::vector<double> incr_time_;
  double lift_height_;
  bool no_follow_;

  void Show() {
    for (int i = 0; i < CRAFT_LEVELS; i++) {
      std::cout << i << "->height:" << height_[i] << " incr_enable:" <<
        incr_enable_[i] << " incr_time:" << incr_time_[i] << std::endl;
    }
    std::cout << "lift height:" << lift_height_ << std::endl;
    std::cout << "no follow:" << no_follow_ << std::endl;
  }
};

class FocusArgs {
 public:
  FocusArgs(): position_(CRAFT_LEVELS, 0) {}

  std::vector<double> position_;
  void Show() {
    for (int i = 0; i < CRAFT_LEVELS; i++) {
      std::cout << "position:" << position_[i] << std::endl;
    }
  }
};

class DelayArgs {
 public:
  DelayArgs(): stay_(CRAFT_LEVELS, 3000),
      laser_off_blow_time_(CRAFT_LEVELS, 500),
      blow_enable_(CRAFT_LEVELS, false) {}

  std::vector<double> stay_;
  std::vector<double> laser_off_blow_time_;
  std::vector<bool> blow_enable_;
  
  void Show() {
    for (int i = 0; i < CRAFT_LEVELS; i++) {
      std::cout << "stay time:" << stay_[i]
          << " laser off blow:" << laser_off_blow_time_[i]
          << " blow enable:" << blow_enable_[i] << std::endl;

    }
  }
};

struct PlcGlobalArgs {
  double first_open_gas_delay;
  double open_gas_delay;
  double switch_gas_delay;
};

struct PlcArgs {
  ProcessArgs process_args;
  GasArgs gas_args;
  LaserArgs laser_args;
  DelayArgs delay_args;
};

#endif
