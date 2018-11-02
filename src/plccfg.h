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
//

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
  GAS_OPEN_CUTTING = 100,
  GAS_OPEN_FIRST = 101,
  GAS_OPEN_SECOND = 102,
  GAS_OPEN_THIRD = 103,

  GAS_CLOSE_CUTTING = 104,
  GAS_CLOSE_FIRST = 105,
  GAS_CLOSE_SECOND = 106,
  GAS_CLOSE_THIRD = 107,

  GAS_PRESSURE_CUTTING = 108,
  GAS_PRESSURE_FIRST = 109,
  GAS_PRESSURE_SECOND = 110,
  GAS_PRESSURE_THIRD = 111,

  // Follower Command
  LHC_FOLLOW_CUTTING = 120,
  LHC_FOLLOW_FIRST = 121,
  LHC_FOLLOW_SECOND = 122,
  LHC_FOLLOW_THIRD = 123,

  LHC_PROGRESSIVE_CUTTING = 124,
  LHC_PROGRESSIVE_FIRST = 125,
  LHC_PROGRESSIVE_SECOND = 126,
  LHC_PROGRESSIVE_THIRD = 127,

  LHC_LIFT = 128,
  LHC_Z_AXIS_HOLD = 129,

  // Laser Command
  LASER_ON = 140,
  LASER_OFF = 141,
  LASER_SHUTTER_ON = 142,
  LASER_SHUTTER_OFF = 143,

  LASER_POWER_CUTTING = 144,
  LASER_POWER_FIRST = 145,
  LASER_POWER_SECOND = 146,
  LASER_POWER_THIRD = 147,

  LASER_DUTYRATIO_CUTTING = 148,
  LASER_DUTYRATIO_FIRST = 149,
  LASER_DUTYRATIO_SECOND = 150,
  LASER_DUTYRATIO_THIRD = 151,

  LASER_PULSEFREQ_CUTTING = 152, 
  LASER_PULSEFREQ_FIRST = 153, 
  LASER_PULSEFREQ_SECOND = 154, 
  LASER_PULSEFREQ_THIRD = 155, 

  LASER_TYPE_CUTTING = 156, 
  LASER_TYPE_FIRST = 157, 
  LASER_TYPE_SECOND = 158, 
  LASER_TYPE_THIRD = 159, 

  // Delay Command
  DELAY_BLOW_CUTTING = 184,
  DELAY_BLOW_FIRST = 185,
  DELAY_BLOW_SECOND = 186,
  DELAY_BLOW_THIRD = 187,

  DELAY_STAY_CUTTING = 188,
  DELAY_STAY_FIRST = 189,
  DELAY_STAY_SECOND = 190,
  DELAY_STAY_THIRD = 191,

  // Focus Command
  FOCUS_POSITION_CUTTING = 210,
  FOCUS_POSITION_FIRST = 211,
  FOCUS_POSITION_SECOND = 212,
  FOCUS_POSITION_THIRD = 213,

  // Alarm function id
  GAS_LPRESSURE_ALARM = 300,
  GAS_HPRESSURE_ALARM = 301,
  GAS_AIR_ALARM = 302,
  GAS_O2_ALARM = 303,
  GAS_N2_ALARM = 304,

  // Gas function id
  GAS_AIR = 400,
  GAS_O2 = 401,
  GAS_N2 = 402,
  GAS_HIGH_AIR = 403,
  GAS_HIGH_O2 = 404,
  GAS_HIGH_N2 = 405,

};

struct ProcessCfg {
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

class GasCfg {
 public:
  GasCfg(): gas_(CRAFT_LEVELS, 0), pressure_(CRAFT_LEVELS, 0.){}

  std::vector<int> gas_;
  std::vector<double> pressure_;
  void Show() {
    for (int i = 0; i < CRAFT_LEVELS; i++) {
      std::cout << gas_[i] << " pressure:" << pressure_[i] << std::endl;
    }
  }
};

class LaserCfg {
 public:
  LaserCfg(): peak_power_(CRAFT_LEVELS, 100), duty_ratio_(CRAFT_LEVELS, 100),
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

class FollowerCfg {
 public:
  FollowerCfg(): height_(CRAFT_LEVELS, 0.), incr_enable_(CRAFT_LEVELS, false),
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

class FocusCfg {
 public:
  FocusCfg(): position_(CRAFT_LEVELS, 0) {}

  std::vector<double> position_;
  void Show() {
    for (int i = 0; i < CRAFT_LEVELS; i++) {
      std::cout << "position:" << position_[i] << std::endl;
    }
  }
};

class DelayCfg {
 public:
  DelayCfg(): stay_(CRAFT_LEVELS, 3000),
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

#endif
