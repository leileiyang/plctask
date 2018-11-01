#ifndef PLCCFG_DELAY_CFG_H_
#define PLCCFG_DELAY_CFG_H_

#include "plccfg.h"
#include <vector>
#include <iostream>

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
