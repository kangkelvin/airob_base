// Copyright (C) 2021 All rights reserved.
#pragma once

#include <memory>
#include <string>

#include "lifecycle_msgs/msg/transition.hpp"
#include "rclcpp/publisher.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"
#include "rclcpp_lifecycle/lifecycle_publisher.hpp"
#include "std_msgs/msg/string.hpp"

namespace intel::examples {

typedef rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
    CallbackReturn;

class SimplePubSub final : public rclcpp_lifecycle::LifecycleNode {
 public:
  explicit SimplePubSub(const std::string &node_name, bool intra_process_comms = false);

  void publishOnTimer();

  /// Transition callback for state configuring
  /**
   * on_configure callback is being called when the lifecycle node
   * enters the "configuring" state.
   * Depending on the return value of this function, the state machine
   * either invokes a transition to the "inactive" state or stays
   * in "unconfigured".
   * TRANSITION_CALLBACK_SUCCESS transitions to "inactive"
   * TRANSITION_CALLBACK_FAILURE transitions to "unconfigured"
   * TRANSITION_CALLBACK_ERROR or any uncaught exceptions to "errorprocessing"
   */
  CallbackReturn on_configure(const rclcpp_lifecycle::State &state);

  /// Transition callback for state activating
  /**
   * on_activate callback is being called when the lifecycle node
   * enters the "activating" state.
   * Depending on the return value of this function, the state machine
   * either invokes a transition to the "active" state or stays
   * in "inactive".
   * TRANSITION_CALLBACK_SUCCESS transitions to "active"
   * TRANSITION_CALLBACK_FAILURE transitions to "inactive"
   * TRANSITION_CALLBACK_ERROR or any uncaught exceptions to "errorprocessing"
   */
  CallbackReturn on_activate(const rclcpp_lifecycle::State &state);

  /// Transition callback for state deactivating
  /**
   * on_deactivate callback is being called when the lifecycle node
   * enters the "deactivating" state.
   * Depending on the return value of this function, the state machine
   * either invokes a transition to the "inactive" state or stays
   * in "active".
   * TRANSITION_CALLBACK_SUCCESS transitions to "inactive"
   * TRANSITION_CALLBACK_FAILURE transitions to "active"
   * TRANSITION_CALLBACK_ERROR or any uncaught exceptions to "errorprocessing"
   */
  CallbackReturn on_deactivate(const rclcpp_lifecycle::State &state);

  /// Transition callback for state cleaning up
  /**
   * on_cleanup callback is being called when the lifecycle node
   * enters the "cleaningup" state.
   * Depending on the return value of this function, the state machine
   * either invokes a transition to the "unconfigured" state or stays
   * in "inactive".
   * TRANSITION_CALLBACK_SUCCESS transitions to "unconfigured"
   * TRANSITION_CALLBACK_FAILURE transitions to "inactive"
   * TRANSITION_CALLBACK_ERROR or any uncaught exceptions to "errorprocessing"
   */
  CallbackReturn on_cleanup(const rclcpp_lifecycle::State &state);

  /// Transition callback for state shutting down
  /**
   * on_shutdown callback is being called when the lifecycle node
   * enters the "shuttingdown" state.
   * Depending on the return value of this function, the state machine
   * either invokes a transition to the "finalized" state or stays
   * in its current state.
   * TRANSITION_CALLBACK_SUCCESS transitions to "finalized"
   * TRANSITION_CALLBACK_FAILURE transitions to current state
   * TRANSITION_CALLBACK_ERROR or any uncaught exceptions to "errorprocessing"
   */
  CallbackReturn on_shutdown(const rclcpp_lifecycle::State &state);

 private:
  // We hold an instance of a lifecycle publisher. This lifecycle publisher
  // can be activated or deactivated regarding on which state the lifecycle node
  // is in.
  // By default, a lifecycle publisher is inactive by creation and has to be
  // activated to publish messages into the ROS world.
  std::shared_ptr<rclcpp_lifecycle::LifecyclePublisher<std_msgs::msg::String>> pub_;

  // We hold an instance of a timer which periodically triggers the publish function.
  // As for the beta version, this is a regular timer. In a future version, a
  // lifecycle timer will be created which obeys the same lifecycle management as the
  // lifecycle publisher.
  std::shared_ptr<rclcpp::TimerBase> timer_;
};

}  // namespace intel::examples