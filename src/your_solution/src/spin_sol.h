#ifndef YOUR_SOLUTION_SRC_SPIN_SOL_H_
#define YOUR_SOLUTION_SRC_SPIN_SOL_H_

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64_multi_array.hpp"
using ArrayMsg = std_msgs::msg::Float64MultiArray;

class SpinSolution : public rclcpp::Node {
 public:
  SpinSolution();
 private:
  void measuredPosCallback(const ArrayMsg::SharedPtr msg);
  void measuredVelCallback(const ArrayMsg::SharedPtr msg);
  void timerCallback();
  rclcpp::Subscription<ArrayMsg>::SharedPtr measured_pos_subscriber_;
  rclcpp::Subscription<ArrayMsg>::SharedPtr measured_vel_subscriber_;
  rclcpp::Publisher<ArrayMsg>::SharedPtr predicted_pos_publisher_;

  rclcpp::TimerBase::SharedPtr timer_;

  ArrayMsg::SharedPtr latest_position_;
  ArrayMsg::SharedPtr latest_velocity_;
  rclcpp::Time last_measurement_time_;
};

#endif //YOUR_SOLUTION_SRC_SPIN_SOL_H_
