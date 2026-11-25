/*
Copyright (c) 2024 TOYOTA MOTOR CORPORATION
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
#ifndef TMC_UTILS_PARAMETERS_HPP_
#define TMC_UTILS_PARAMETERS_HPP_

#include <memory>
#include <string>
#include <vector>

#include <rclcpp/rclcpp.hpp>
#include <rclcpp_lifecycle/lifecycle_node.hpp>

namespace tmc_utils {

template <typename NodeType, typename ParameterType>
auto GetParameter(NodeType* node, const std::string& name, const ParameterType& default_value) {
  if (node->has_parameter(name)) {
    return node->get_parameter(name).template get_value<ParameterType>();
  } else {
    return node->template declare_parameter<ParameterType>(name, default_value);
  }
}

template <typename ParameterType>
auto GetParameter(rclcpp::Node* node, const std::string& name, const ParameterType& default_value) {
  return GetParameter<rclcpp::Node, ParameterType>(node, name, default_value);
}

template <typename ParameterType>
auto GetParameter(rclcpp::Node::SharedPtr node, const std::string& name, const ParameterType& default_value) {
  return GetParameter<rclcpp::Node, ParameterType>(node.get(), name, default_value);
}

template <typename ParameterType>
auto GetParameter(rclcpp_lifecycle::LifecycleNode* node, const std::string& name, const ParameterType& default_value) {
  return GetParameter<rclcpp_lifecycle::LifecycleNode, ParameterType>(node, name, default_value);
}

template <typename ParameterType>
auto GetParameter(rclcpp_lifecycle::LifecycleNode::SharedPtr node,
                  const std::string& name,
                  const ParameterType& default_value) {
  return GetParameter<rclcpp_lifecycle::LifecycleNode, ParameterType>(node.get(), name, default_value);
}


template<typename ParameterType>
class DynamicParameter {
 public:
  using Ptr = std::shared_ptr<DynamicParameter>;

  DynamicParameter(rclcpp::Node* node,
                   const std::string& parameter_name,
                   const ParameterType& default_value) : name_(parameter_name) {
    value_ = GetParameter<ParameterType>(node, parameter_name, default_value);
    handle_ = node->add_on_set_parameters_callback(
        std::bind(&DynamicParameter<ParameterType>::SetParameterCallback, this, std::placeholders::_1));
  }

  DynamicParameter(const rclcpp::Node::SharedPtr& node,
                   const std::string& parameter_name,
                   const ParameterType& default_value) : DynamicParameter(node.get(), parameter_name, default_value) {}

  DynamicParameter(rclcpp_lifecycle::LifecycleNode* node,
                   const std::string& parameter_name,
                   const ParameterType& default_value) : name_(parameter_name) {
    value_ = GetParameter<ParameterType>(node, parameter_name, default_value);
    handle_ = node->add_on_set_parameters_callback(
        std::bind(&DynamicParameter<ParameterType>::SetParameterCallback, this, std::placeholders::_1));
  }

  DynamicParameter(const rclcpp_lifecycle::LifecycleNode::SharedPtr& node,
                   const std::string& parameter_name,
                   const ParameterType& default_value) : DynamicParameter(node.get(), parameter_name, default_value) {}

  ParameterType value() const { std::lock_guard<std::mutex> lock(mutex_); return value_; }

  rcl_interfaces::msg::SetParametersResult SetParameterCallback(const std::vector<rclcpp::Parameter>& params) {
    for (const auto& param : params) {
      if (param.get_name() == name_) {
        std::lock_guard<std::mutex> lock(mutex_);
        value_ = param.get_value<ParameterType>();
        break;
      }
    }
    // The parameter update callback is called in the chain, but if it returns false, it will be terminated, so it must always return true
    auto result = rcl_interfaces::msg::SetParametersResult();
    result.successful = true;
    return result;
  }

 private:
  ParameterType value_;
  std::string name_;
  rclcpp::node_interfaces::OnSetParametersCallbackHandle::SharedPtr handle_;
  mutable std::mutex mutex_;
};

}  // namespace tmc_utils
#endif  // TMC_UTILS_PARAMETERS_HPP_
