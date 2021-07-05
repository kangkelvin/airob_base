// Copyright (C) 2021 All rights reserved.

#include "lifecycle_talker.hpp"

#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <utility>

#include "lifecycle_msgs/msg/transition.hpp"
#include "rclcpp/publisher.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"
#include "rclcpp_lifecycle/lifecycle_publisher.hpp"
#include "rcutils/logging_macros.h"
#include "std_msgs/msg/string.hpp"

typedef rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
    CallbackReturn;

namespace intel::examples {

SimplePubSub::SimplePubSub(const std::string &node_name, bool intra_process_comms)
    : rclcpp_lifecycle::LifecycleNode(
          node_name, rclcpp::NodeOptions().use_intra_process_comms(intra_process_comms)) {
}

void SimplePubSub::publishOnTimer() {
  static size_t count = 0;
  auto msg = std::make_unique<std_msgs::msg::String>();
  msg->data = "Lifecycle HelloWorld #" + std::to_string(++count);

  // Print the current state for demo purposes
  if (!pub_->is_activated()) {
    RCLCPP_INFO(get_logger(),
                "Lifecycle publisher is currently inactive. Messages are not published.");
  } else {
    RCLCPP_INFO(get_logger(), "Lifecycle publisher is active. Publishing: [%s]",
                msg->data.c_str());
  }

  // We independently from the current state call publish on the lifecycle publisher.
  // Only if the publisher is in an active state, the message transfer is
  // enabled and the message actually published.
  pub_->publish(std::move(msg));
}

CallbackReturn SimplePubSub::on_configure(const rclcpp_lifecycle::State &state) {
  pub_ = this->create_publisher<std_msgs::msg::String>("lifecycle_chatter", 10);

  std::chrono::milliseconds timer_period(500);
  timer_ = this->create_wall_timer(timer_period,
                                   std::bind(&SimplePubSub::publishOnTimer, this));

  RCLCPP_INFO(get_logger(), "on_configure() is called from state %s.",
              state.label().c_str());

  return CallbackReturn::SUCCESS;
}

CallbackReturn SimplePubSub::on_activate(const rclcpp_lifecycle::State &state) {
  pub_->on_activate();
  RCUTILS_LOG_INFO_NAMED(get_name(), "on_activate() is called from state %s.",
                         state.label().c_str());

  // If node fails to activate you can throw an ERROR signal
  if (true) {
    return CallbackReturn::SUCCESS;

  } else {
    return CallbackReturn::ERROR;
  }
}

CallbackReturn SimplePubSub::on_deactivate(const rclcpp_lifecycle::State &state) {
  pub_->on_deactivate();
  RCUTILS_LOG_INFO_NAMED(get_name(), "on_deactivate() is called from state %s.",
                         state.label().c_str());

  return CallbackReturn::SUCCESS;
}

CallbackReturn SimplePubSub::on_cleanup(const rclcpp_lifecycle::State &state) {
  timer_.reset();
  pub_.reset();
  RCUTILS_LOG_INFO_NAMED(get_name(), "on cleanup is called from state %s.",
                         state.label().c_str());

  return CallbackReturn::SUCCESS;
}

CallbackReturn SimplePubSub::on_shutdown(const rclcpp_lifecycle::State &state) {
  timer_.reset();
  pub_.reset();
  RCUTILS_LOG_INFO_NAMED(get_name(), "on shutdown is called from state %s.",
                         state.label().c_str());

  return CallbackReturn::SUCCESS;
}

}  // namespace intel::examples

int main(int argc, char *argv[]) {
  setvbuf(stdout, NULL, _IONBF, BUFSIZ);
  rclcpp::init(argc, argv);
  rclcpp::executors::SingleThreadedExecutor exe;
  std::shared_ptr<intel::examples::SimplePubSub> node =
      std::make_shared<intel::examples::SimplePubSub>("example_simple_pub_sub_node");
  exe.add_node(node->get_node_base_interface());
  exe.spin();
  rclcpp::shutdown();
  return 0;
}
