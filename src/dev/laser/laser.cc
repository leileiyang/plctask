#include "laser.h"

#include <rcs_prnt.hh>

#include "io_laser.h"
#include "dev/gpio/io_device.h"

int Laser::ConnectIoDevice(IoDevice *io_dev) {
  laser_intf_ = new IoLaser(io_dev);
  return 0;
}

int Laser::LaserOn() {
  rcs_print("LaserOn\n");
  return laser_intf_->LaserOn();
}

int Laser::LaserOff() {
  rcs_print("LaserOff\n");
  return laser_intf_->LaserOff();
}

int Laser::ShutterOn() {
  rcs_print("ShutterOn\n");
  return laser_intf_->ShutterOn();
}

int Laser::ShutterOff() {
  rcs_print("ShutterOff\n");
  return laser_intf_->ShutterOff();
}

int Laser::SetPower(double power) {
  rcs_print("Laser Set Power %f\n", power);
  return laser_intf_->SetPower(power);
}

int Laser::SetDutyRatio(double duty_ratio) {
  rcs_print("Laser Set DutyRatio %f\n", duty_ratio);
  return laser_intf_->SetDutyRatio(duty_ratio);
}

int Laser::SetPulseFrequency(double pulse_frequency) {
  rcs_print("Laser Set Pulse Frequency %f\n", pulse_frequency);
  return laser_intf_->SetPulseFrequency(pulse_frequency);
}

int Laser::SetType(int type) {
  rcs_print("Laser Set Type %d\n", type);
  return laser_intf_->SetType(type);
}
