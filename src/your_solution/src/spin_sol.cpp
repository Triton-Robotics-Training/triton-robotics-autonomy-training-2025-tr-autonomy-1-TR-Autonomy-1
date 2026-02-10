#include "spin_sol.h"

SpinSolution::SpinSolution() : Node("spinsolution") {
  measured_pos_subscriber_ = this->create_subscription<ArrayMsg>(
      "/measuredpos", 10,
      std::bind(&SpinSolution::measuredPosCallback, this, std::placeholders::_1));

  measured_vel_subscriber_ = this->create_subscription<ArrayMsg>(
      "/measuredvel", 10,
      std::bind(&SpinSolution::measuredVelCallback, this, std::placeholders::_1));

  predicted_pos_publisher_ = this->create_publisher<ArrayMsg>("/predictedpos", 10);

  timer_ = this->create_wall_timer(
      std::chrono::milliseconds(50),
      std::bind(&SpinSolution::timerCallback, this));
  
  latest_position_ = std::make_shared<ArrayMsg>();
  latest_velocity_ = std::make_shared<ArrayMsg>();
  last_measurement_time_ = this->now();
}

void SpinSolution::measuredPosCallback(const ArrayMsg::SharedPtr msg) {
  latest_position_ = msg;
  last_measurement_time_ = this->now();
}

void SpinSolution::measuredVelCallback(const ArrayMsg::SharedPtr msg) {
  latest_velocity_ = msg;
}

void SpinSolution::timerCallback() {
  if (latest_position_->data.size() < 2 || latest_velocity_->data.size() < 2) {
    RCLCPP_WARN_THROTTLE(this->get_logger(), *this->get_clock(), 5000,
                         "Waiting for initial measurements...");
    return;
  }

  rclcpp::Time current_time = this->now();
  double delta_t = (current_time - last_measurement_time_).seconds();

  ArrayMsg predicted_msg;
  predicted_msg.data.resize(2);
  predicted_msg.data[0] = latest_position_->data[0] + delta_t * latest_velocity_->data[0];
  predicted_msg.data[1] = latest_position_->data[1] + delta_t * latest_velocity_->data[1];

  predicted_pos_publisher_->publish(predicted_msg);
}

int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<SpinSolution>());
  rclcpp::shutdown();
  return 0;
}
