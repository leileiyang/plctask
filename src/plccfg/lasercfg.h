#ifndef PLCCFG_LASER_CFG_H_
#define PLCCFG_LASER_CFG_H_

#include "plccfg.h"
#include <vector>
#include <iostream>

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

#endif
