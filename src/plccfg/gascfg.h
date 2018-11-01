#ifndef PLCCFG_GASCFG_H_
#define PLCCFG_GASCFG_H_

#include "plccfg.h"
#include <vector>
#include <iostream>

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

#endif
