#include "follower.h"

Follower::Follower(): {}

Follower::~Follower() {}

int Follower::ConnectInterface(LhcInterface *lhc_intf) {
  lhc_intf_ = lhc_intf;
}

int Follower::FollowTo(double height) {
  if (lhc_intf_->FollowTo(height)) {
    return 0; 
  } else {
    return -1;
  }
}

int Follower::IncrFollowTo(double height, double time) {
  if (lhc_intf_->IncrFollowTo(height, time)) {
    return 0; 
  } else {
    return -1;
  }
}

int Follower::LiftTo(double height) {
  if (lhc_intf_->LiftTo(height)) {
    return 0; 
  } else {
    return -1;
  }
}

void Follower::Update() {
  lhc_intf_->Update(status_);
}
