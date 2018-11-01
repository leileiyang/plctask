#ifndef PLCCFG_FOCUSCFG_H_
#define PLCCFG_FOCUSCFG_H_

#include "plccfg.h"
#include <vector>
#include <iostream>

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

#endif // DEVICES_DEVCFG_FOCUSCFG_H_
