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

#include <tmc_utils/robot_description.hpp>

#include <tmc_utils/parameters.hpp>

namespace {
// Default name of the URDF robot model to be loaded
const char* const kDefaultRobotModelName = "robot_description";
// Default name of the node to load the URDF robot model
const char* const kDefaultRobotModelNode = "robot_state_publisher";
}  // namespace

namespace tmc_utils {
std::string ResolveRobotDescription(const rclcpp_lifecycle::LifecycleNode::SharedPtr& node) {
  const auto model_name = GetParameter<std::string>(node, "model_name", kDefaultRobotModelName);
  const auto robot_description = GetParameter<std::string>(node, model_name, "");
  if (!robot_description.empty()) {
    return robot_description;
  }

  const auto model_node_name = GetParameter<std::string>(node, "model_node_name", kDefaultRobotModelNode);
  const auto timeout = GetParameter<int32_t>(node, "parameter_connection_timeout", 60);
  // Using the argument node causes an error when retrieving parameters with get_parameter, so
  // Temporarily create a node object.
  auto client_node = rclcpp_lifecycle::LifecycleNode::make_shared(std::string(node->get_name()));
  auto parameters_client = std::make_shared<rclcpp::SyncParametersClient>(client_node, model_node_name);
  int32_t wait_for_service_count = 0;
  while (!parameters_client->wait_for_service(std::chrono::seconds(1))) {
    ++wait_for_service_count;
    if (!rclcpp::ok()) {
      return "";
    } else if (wait_for_service_count >= timeout) {
      RCLCPP_ERROR_STREAM(node->get_logger(), "Could not connect parameter server of " << model_node_name);
      return "";
    }
  }
  return parameters_client->get_parameter<std::string>(model_name, "");
}
}  // namespace tmc_utils
