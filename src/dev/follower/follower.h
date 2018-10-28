#ifndef DEV_FOLLOWER_FOLLOWER_H_
#define DEV_FOLLOWER_FOLLOWER_H_

#include "lhc_interface.h"

class Follower {
 public:
  Follower();
  ~Follower();
  int ConnectInterface(LhcInterface *lhc_intf);
  int FollowTo(double height);
  int IncrFollowTo(double height, double time);
  int LiftTo(double height);
  void Update();

 private:
  LhcInterface *lhc_intf_;
  LHC_STATUS status_;
};

#endif
