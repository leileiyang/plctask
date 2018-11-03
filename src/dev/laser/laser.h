#ifndef DEV_LASER_LASER_H
#define DEV_LASER_LASER_H

class LaserInterface;
class IoDevice;

class Laser {
 public:
  int LaserOn();
  int LaserOff();
  int ShutterOn();
  int ShutterOff();
  int SetPower(double power);
  int SetDutyRatio(double duty_ratio);
  int SetPulseFrequency(double pulse_frequency);
  int SetType(int type);
  int ConnectInterface(LaserInterface *laser_intf) {
    laser_intf_ = laser_intf;
    return 0;
  }
  int ConnectIoDevice(IoDevice *io_dev);

 private:
  LaserInterface *laser_intf_;

};

#endif
