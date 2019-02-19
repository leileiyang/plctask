#ifndef DEV_FOLLOWER_FLHCINTERFACE_H_
#define DEV_FOLLOWER_FLHCINTERFACE_H_

#include <string>

enum LHC_STATUS {
  LHC_STILL,
  LHC_FOLLOWING,
  LHC_BACKING,
  LHC_HOMING,
};

class LhcInterface {
 public:
  virtual bool FollowTo(double height);
  virtual bool IncrFollowTo(double height, double time);
  virtual bool LiftTo(double height);

};

#endif
