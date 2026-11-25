/*
Copyright (c) 2025 TOYOTA MOTOR CORPORATION
All rights reserved.
Redistribution and use in source and binary forms, with or without
modification, are permitted (subject to the limitations in the disclaimer
below) provided that the following conditions are met:
* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.
* Neither the name of the copyright holder nor the names of its contributors may be used
  to endorse or promote products derived from this software without specific
  prior written permission.
NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS
LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
DAMAGE.
*/

#include <gtest/gtest.h>

#include <rclcpp/rclcpp.hpp>

#include <tmc_utils/robot_description.hpp>

namespace {
const char* const kRobotDescription = "robot_description_value";

void spin_some(const rclcpp::Node::SharedPtr& node) {
  while (node) {
    rclcpp::spin_some(node);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}
}

namespace tmc_utils {

TEST(ResolveRobotDescriptionTest, FromDefaultParameter) {
  rclcpp::NodeOptions options;
  options.parameter_overrides() = {rclcpp::Parameter("robot_description", kRobotDescription)};
  auto node = rclcpp_lifecycle::LifecycleNode::make_shared("test", options);

  EXPECT_EQ(tmc_utils::ResolveRobotDescription(node), kRobotDescription);
}

TEST(ResolveRobotDescriptionTest, FromCustomParameter) {
  const std::string custom_model_name = "custom_robot_description";
  rclcpp::NodeOptions options;
  options.parameter_overrides() = {rclcpp::Parameter("model_name", custom_model_name),
                                   rclcpp::Parameter(custom_model_name, kRobotDescription)};
  auto node = rclcpp_lifecycle::LifecycleNode::make_shared("test", options);

  EXPECT_EQ(tmc_utils::ResolveRobotDescription(node), kRobotDescription);
}

TEST(ResolveRobotDescriptionTest, FromRobotStatePublisherNode) {
  auto robot_state_publisher_node = rclcpp::Node::make_shared("robot_state_publisher");
  robot_state_publisher_node->declare_parameter("robot_description", kRobotDescription);
  auto spin_thread = std::thread(std::bind(spin_some, std::ref(robot_state_publisher_node)));

  auto node = rclcpp_lifecycle::LifecycleNode::make_shared("test");
  EXPECT_EQ(tmc_utils::ResolveRobotDescription(node), kRobotDescription);

  robot_state_publisher_node.reset();
  spin_thread.join();
}

TEST(ResolveRobotDescriptionTest, FromOtherNode) {
  auto another_node = rclcpp::Node::make_shared("another_node");
  another_node->declare_parameter("robot_description", kRobotDescription);
  auto spin_thread = std::thread(std::bind(spin_some, std::ref(another_node)));

  rclcpp::NodeOptions options;
  options.parameter_overrides() = {rclcpp::Parameter("model_node_name", "another_node")};
  auto node = rclcpp_lifecycle::LifecycleNode::make_shared("test", options);
  EXPECT_EQ(tmc_utils::ResolveRobotDescription(node), kRobotDescription);

  another_node.reset();
  spin_thread.join();
}

TEST(ResolveRobotDescriptionTest, FromCustomParameterOnOtherNode) {
  const std::string custom_model_name = "custom_robot_description";

  auto another_node = rclcpp::Node::make_shared("another_node");
  another_node->declare_parameter(custom_model_name, kRobotDescription);
  auto spin_thread = std::thread(std::bind(spin_some, std::ref(another_node)));

  rclcpp::NodeOptions options;
  options.parameter_overrides() = {
      rclcpp::Parameter("model_node_name", "another_node"),
      rclcpp::Parameter("model_name", custom_model_name)};
  auto node = rclcpp_lifecycle::LifecycleNode::make_shared("test", options);
  EXPECT_EQ(tmc_utils::ResolveRobotDescription(node), kRobotDescription);

  another_node.reset();
  spin_thread.join();
}

TEST(ResolveRobotDescriptionTest, NoOtherNode) {
  rclcpp::NodeOptions options;
  options.parameter_overrides() = {rclcpp::Parameter("parameter_connection_timeout", 2)};
  auto node = rclcpp_lifecycle::LifecycleNode::make_shared("test", options);
  EXPECT_EQ(tmc_utils::ResolveRobotDescription(node), "");
}

TEST(ResolveRobotDescriptionTest, NoRobotDescription) {
  auto robot_state_publisher_node = rclcpp::Node::make_shared("robot_state_publisher");
  auto spin_thread = std::thread(std::bind(spin_some, std::ref(robot_state_publisher_node)));

  rclcpp::NodeOptions options;
  options.parameter_overrides() = {rclcpp::Parameter("parameter_connection_timeout", 2)};
  auto node = rclcpp_lifecycle::LifecycleNode::make_shared("test", options);
  EXPECT_EQ(tmc_utils::ResolveRobotDescription(node), "");

  robot_state_publisher_node.reset();
  spin_thread.join();
}

}  // namespace tmc_utils

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
