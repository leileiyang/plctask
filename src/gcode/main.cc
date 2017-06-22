#include <math.h>

#include "nml_intf/plc_nml.hh"

G_ORDER_MSG SplitG01(G_ORDER_MSG &gcmd, double length) {
  double lambda = 1;
  G_ORDER_MSG breed_cmd(gcmd);
  lambda = length / (gcmd.length_ - length);
  breed_cmd.x_ = (gcmd.x0_ + lambda * gcmd.x_) / (1 + lambda);
  breed_cmd.y_ = (gcmd.y0_ + lambda * gcmd.y_) / (1 + lambda);
  breed_cmd.length_ = (float)length;
  gcmd.x0_ = breed_cmd.x_;
  gcmd.y0_ = breed_cmd.y_;
  gcmd.length_ -= (float)length;
  return breed_cmd;
}


G_ORDER_MSG SplitG02G03(G_ORDER_MSG &gcmd, double length) {
  double theta = length / gcmd.r_;
  if (gcmd.name_ == 3) {
    theta = -theta;
  }
  double x1 = gcmd.x0_ - gcmd.i_;
  double y1 = gcmd.y0_ - gcmd.j_;

  G_ORDER_MSG breed_cmd(gcmd);
  breed_cmd.x_ = cos(theta) * x1 + sin(theta) * y1;
  breed_cmd.y_ = -sin(theta) * x1 + cos(theta) * y1;
  breed_cmd.x_ += gcmd.i_;
  breed_cmd.y_ += gcmd.j_;
  breed_cmd.length_ = (float)length;

  gcmd.x0_ = breed_cmd.x_;
  gcmd.y0_ = breed_cmd.y_;
  gcmd.length_ -= (float)length;

  return breed_cmd;
}

int main() {
  G_ORDER_MSG order;
  order.x0_ = 0;
  order.y0_ = 10;
  order.x_ = 10;
  order.y_ = 0;
  order.i_ = 0;
  order.j_ = 0;
  order.r_ = 10;
  order.length_ = 2 * M_PI * 10 / 4;
  order.name_ = 2;
  double length = 2 * M_PI * 10 / 4 / 3;
  G_ORDER_MSG breed_cmd = SplitG02G03(order, length);
  return 0;
}
