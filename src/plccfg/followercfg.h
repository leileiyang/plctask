#ifndef PLCCFG_FOLLOWERCFG_H_
#define PLCCFG_FOLLOWERCFG_H_

#include "plccfg.h"
#include <vector>
#include <iostream>

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

#endif
