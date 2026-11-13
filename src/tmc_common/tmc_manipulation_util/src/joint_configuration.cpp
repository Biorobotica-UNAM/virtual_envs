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
/// @file     joint_configuration.cpp
/// @brief Functions for handling joint postures and Trajectory (joint_configuration)
/// @version  1.0.0
/// @author   Kiyohiro Sogen
/// @date     2011.10.25
#include "tmc_manipulation_util/joint_configuration.hpp"
#include <string>
#include <vector>
#include <tmc_utils/param_loader.hpp>

namespace {

/// @brief Load an array of time from time_name
bool LoadTime(const ros::NodeHandle& node, const std::string& time_name, std::vector<double>& dst_time) {
  // Loading time_name
  XmlRpc::XmlRpcValue time_rpc;
  std::vector<double> time_vec;
  dst_time = time_vec;
  if (!node.getParam(time_name, time_rpc)) {
    return false;
  }
  if (time_rpc.getType() != XmlRpc::XmlRpcValue::TypeArray) {
    return false;
  }
  for (int32_t i = 0; i < time_rpc.size(); ++i) {
    // [0.2, 0.3, 0.4] type
    if (time_rpc[i].getType() == XmlRpc::XmlRpcValue::TypeDouble) {
      time_vec.push_back(static_cast<double>(time_rpc[i]));
    } else if (time_rpc[i].getType() == XmlRpc::XmlRpcValue::TypeString) {
      // When time is specified by name [0.2, time1, time2] type
      // Load parameters using time_rpc[i] as the key
      XmlRpc::XmlRpcValue sub_time_rpc;
      if (!node.getParam(static_cast<std::string>(time_rpc[i]), sub_time_rpc)) {
        return false;
      }
      for (int32_t j = 0; j < sub_time_rpc.size(); ++j) {
        // The contents of the Array must be of type double
        if (sub_time_rpc[j].getType() == XmlRpc::XmlRpcValue::TypeDouble) {
          time_vec.push_back(static_cast<double>(sub_time_rpc[j]));
        } else {
          return false;
        }
      }
    } else {
      return false;
    }
  }
  dst_time = time_vec;
  return true;
}

/// @brief Check if the contents of xml_rpc_value's Array are double and store them in dst_point.positions
/// @note I want to make xml_rpc_value const, but,
///       static_cast<double>(xml_rpc_value[i]) does not pass if it is const
bool ToTrajectoryPointPositions(XmlRpc::XmlRpcValue& xml_rpc_value,
                                trajectory_msgs::JointTrajectoryPoint& dst_point) {
  if (xml_rpc_value.getType() != XmlRpc::XmlRpcValue::TypeArray) {
    return false;
  }
  std::vector<double> positions;
  for (uint32_t i = 0; i < xml_rpc_value.size(); ++i) {
    // The contents of the Array must be of type double
    if (xml_rpc_value[i].getType() == XmlRpc::XmlRpcValue::TypeDouble) {
      positions.push_back(static_cast<double>(xml_rpc_value[i]));
    } else {
      return false;
    }
  }
  dst_point.positions = positions;
  return true;
}

/// @brief Load only the joint angle sequence from param_name
bool LoadTrajectory(const ros::NodeHandle& node, const std::string& param_name,
                    trajectory_msgs::JointTrajectory& dst_joint_trajectory) {
  trajectory_msgs::JointTrajectory joint_trajectory;
  dst_joint_trajectory = joint_trajectory;

  // Loading param_name
  XmlRpc::XmlRpcValue param_rpc;
  if (!node.getParam(param_name, param_rpc)) {
    return false;
  }
  if (param_rpc.getType() != XmlRpc::XmlRpcValue::TypeArray) {
    return false;
  }
  // Process based on the type of param_rpc[i]
  // TypeString and TypeArray can coexist,
  // TypeDouble will cause issues if param_rpc[i] is not entirely TypeDouble,
  // It is filtered by the user of LoadTrajectory, so it is not a concern here
  trajectory_msgs::JointTrajectoryPoint single_point;
  for (int32_t i = 0; i < param_rpc.size(); ++i) {
    switch (param_rpc[i].getType()) {
      case XmlRpc::XmlRpcValue::TypeString: {
        // Load parameters using param_rpc[i] as the key
        XmlRpc::XmlRpcValue sub_param_rpc;
        if (!node.getParam(static_cast<std::string>(param_rpc[i]), sub_param_rpc)) {
          return false;
        }
        trajectory_msgs::JointTrajectoryPoint point;
        if (ToTrajectoryPointPositions(sub_param_rpc, point)) {
          joint_trajectory.points.push_back(point);
        } else {
          return false;
        }
        break;
      }
      case XmlRpc::XmlRpcValue::TypeArray: {
        // Directly load the Array of param_rpc[i]
        trajectory_msgs::JointTrajectoryPoint point;
        if (ToTrajectoryPointPositions(param_rpc[i], point)) {
          joint_trajectory.points.push_back(point);
        } else {
          return false;
        }
        break;
      }
      case XmlRpc::XmlRpcValue::TypeDouble:
        // In the case of double, load a single position, trajectory_points will be 1
        single_point.positions.push_back(static_cast<double>(param_rpc[i]));
        break;
      default:
        return false;
    }
  }
  // If a value is in single_point, add it to joint_trajectory
  if (!single_point.positions.empty()) {
    joint_trajectory.points.push_back(single_point);
  }

  dst_joint_trajectory = joint_trajectory;

  return true;
}
}  // unnamed namespace

namespace tmc_manipulation_util {

/// @brief Load an array of joint names from joint_name
bool LoadJointName(const ros::NodeHandle& node, const std::string& joint_name,
                   std::vector<std::string>& dst_joint_name) {
  return tmc_utils::LoadStringArray(node, joint_name, dst_joint_name);
}


/// @brief Load joint names from joint_name, joint values from param_name, and
/// obtain time_from_start from time_name, and return as JointTrajectory
bool GetJointConfiguration(const ros::NodeHandle& node, const std::string& joint_name, const std::string& param_name,
                           const std::string& time_name, trajectory_msgs::JointTrajectory& dst_joint_trajectory) {
  trajectory_msgs::JointTrajectory joint_trajectory;
  dst_joint_trajectory = joint_trajectory;

  // Loading param_name
  if (!LoadTrajectory(node, param_name, joint_trajectory)) {
    return false;
  }

  // Loading joint_name
  std::vector<std::string> joint_names;
  if (!LoadJointName(node, joint_name, joint_names)) {
    return false;
  }

  // Loading time_name
  std::vector<double> time;
  if (!LoadTime(node, time_name, time)) {
    return false;
  }

  // Assign the array of joint names
  joint_trajectory.joint_names = joint_names;

  // The size of time and joint_trajectory.points must be the same.
  if (time.size() != joint_trajectory.points.size()) {
    return false;
  }

  // Check the size of joint_names and assign to time_from_start
  std::vector<trajectory_msgs::JointTrajectoryPoint>::iterator it_point = joint_trajectory.points.begin();
  std::vector<double>::const_iterator it_time = time.begin();
  for (; it_point != joint_trajectory.points.end(); ++it_point, ++it_time) {
    // Error if the size of positions does not match joint_names
    if (joint_trajectory.joint_names.size() != it_point->positions.size()) {
      return false;
    }
    it_point->time_from_start = ros::Duration(*it_time);
  }
  dst_joint_trajectory = joint_trajectory;
  dst_joint_trajectory.header.stamp = ros::Time::now();

  return true;
}

/// @brief Load joint values from param_name, and time_from_start from time_name,
/// and return as JointTrajectory
bool GetJointConfiguration(const ros::NodeHandle& node, const std::string& param_name, const std::string& time_name,
                           trajectory_msgs::JointTrajectory& dst_joint_trajectory) {
  return GetJointConfiguration(node, "joint_names", param_name, time_name, dst_joint_trajectory);
}

/// @brief Load joint names from joint_name, and joint values from param_name,
/// and return as JointState
bool GetJointConfiguration(const ros::NodeHandle& node, const std::string& joint_name, const std::string& param_name,
                           sensor_msgs::JointState& dst_joint_state) {
  sensor_msgs::JointState joint_state;
  dst_joint_state = joint_state;

  std::vector<sensor_msgs::JointState> joint_states;
  if (!GetJointConfiguration(node, joint_name, param_name, joint_states)) {
    return false;
  }

  if (joint_states.size() != 1) {
    return false;
  }

  dst_joint_state = joint_states.at(0);
  return true;
}

/// @brief Load joint values from param_name and return as JointState
/// Joint names are obtained from joint_names.
bool GetJointConfiguration(const ros::NodeHandle& node, const std::string& param_name,
                           sensor_msgs::JointState& dst_joint_state) {
  return GetJointConfiguration(node, "joint_names", param_name, dst_joint_state);
}

/// @brief Load joint names from joint_name, and joint values from param_name,
/// and return as an array of JointState
bool GetJointConfiguration(const ros::NodeHandle& node, const std::string& joint_name, const std::string& param_name,
                           std::vector<sensor_msgs::JointState>& dst_joint_states) {
  dst_joint_states.clear();

  // Loading param_name
  trajectory_msgs::JointTrajectory joint_trajectory;
  if (!LoadTrajectory(node, param_name, joint_trajectory)) {
    return false;
  }

  // Loading joint_name
  std::vector<std::string> joint_names;
  if (!LoadJointName(node, joint_name, joint_names)) {
    return false;
  }

  dst_joint_states.reserve(joint_trajectory.points.size());
  for (std::vector<trajectory_msgs::JointTrajectoryPoint>::const_iterator it_point = joint_trajectory.points.begin();
       it_point != joint_trajectory.points.end(); ++it_point) {
    // it_point->positions and joint_names must be the same size
    if (it_point->positions.size() != joint_names.size()) {
      return false;
    }
    // Convert to JointState
    sensor_msgs::JointState joint_state;
    joint_state.header.stamp = ros::Time::now();
    joint_state.name = joint_names;
    joint_state.position = it_point->positions;
    joint_state.velocity = it_point->velocities;
    dst_joint_states.push_back(joint_state);
  }

  return true;
}

/// @brief Load joint values from param_name and return as an array of JointState
/// Joint names are obtained from joint_names.
bool GetJointConfiguration(const ros::NodeHandle& node, const std::string& param_name,
                           std::vector<sensor_msgs::JointState>& dst_joint_states) {
  return GetJointConfiguration(node, "joint_names", param_name, dst_joint_states);
}

// Overwrite joint angles of joint_state with overwriting_state.
bool OverwriteJointState(const sensor_msgs::JointState& joint_state, const sensor_msgs::JointState& overwriting_state,
                         sensor_msgs::JointState& overwritten_state_out) {
  // Argument check
  if (joint_state.name.size() != joint_state.position.size()) {
    return false;
  }
  if (overwriting_state.name.size() != overwriting_state.position.size()) {
    return false;
  }
  // Find the corresponding joint
  overwritten_state_out = joint_state;
  for (std::vector<std::string>::const_iterator overwriting_name = overwriting_state.name.begin();
       overwriting_name != overwriting_state.name.end(); ++overwriting_name) {
    std::vector<std::string>::iterator overwritten_state_name =
        find(overwritten_state_out.name.begin(), overwritten_state_out.name.end(), *overwriting_name);
    if (overwritten_state_name != overwritten_state_out.name.end()) {
      overwritten_state_out.position.at(std::distance(overwritten_state_out.name.begin(), overwritten_state_name)) =
          overwriting_state.position.at(std::distance(overwriting_state.name.begin(), overwriting_name));
    }
  }
  return true;
}

// Merge joint angles of joint_state with merge_state.
bool MergeJointState(const sensor_msgs::JointState& joint_state, const sensor_msgs::JointState& merge_state,
                     sensor_msgs::JointState& merged_state_out) {
  // Argument check
  if (joint_state.name.size() != joint_state.position.size()) {
    return false;
  }
  if (merge_state.name.size() != merge_state.position.size()) {
    return false;
  }
  // Find the corresponding joint
  merged_state_out = joint_state;
  for (std::vector<std::string>::const_iterator merge_name = merge_state.name.begin();
       merge_name != merge_state.name.end(); ++merge_name) {
    std::vector<std::string>::iterator merged_state_name =
        find(merged_state_out.name.begin(), merged_state_out.name.end(), *merge_name);
    if (merged_state_name != merged_state_out.name.end()) {
      merged_state_out.position.at(std::distance(merged_state_out.name.begin(), merged_state_name)) =
          merge_state.position.at(std::distance(merge_state.name.begin(), merge_name));
    } else {
      merged_state_out.name.push_back(*merge_name);
      merged_state_out.position.push_back(merge_state.position.at(std::distance(merge_state.name.begin(), merge_name)));
    }
  }
  return true;
}

// Extract joint angles with names matching joint_names from joint_state
bool ExtractJointState(const sensor_msgs::JointState& joint_state, const std::vector<std::string>& joint_names,
                       sensor_msgs::JointState& extracted_state_out) {
  // Argument check
  if (joint_state.name.size() != joint_state.position.size()) {
    return false;
  }
  extracted_state_out = sensor_msgs::JointState();
  // Extract joint angles with matching names
  for (std::vector<std::string>::const_iterator joint_name = joint_names.begin(); joint_name != joint_names.end();
       ++joint_name) {
    std::vector<std::string>::const_iterator common_name =
        std::find(joint_state.name.begin(), joint_state.name.end(), *joint_name);
    if (common_name != joint_state.name.end()) {
      extracted_state_out.position.push_back(
          joint_state.position.at(std::distance(joint_state.name.begin(), common_name)));
      extracted_state_out.name.push_back(*common_name);
    }
  }
  return true;
}

// Extract only those matching joint_name from joint_state
bool ExtractJointState(const sensor_msgs::JointState& joint_state, const std::string& joint_name,
                       sensor_msgs::JointState& extracted_state_out) {
  if ((joint_state.name.size() != joint_state.position.size()) ||
      (joint_state.name.size() != joint_state.velocity.size()) ||
      (joint_state.name.size() != joint_state.effort.size())) {
    return false;
  }

  extracted_state_out.header = joint_state.header;
  extracted_state_out.position.clear();
  extracted_state_out.velocity.clear();
  extracted_state_out.effort.clear();

  // Extract joint information with matching names
  std::vector<std::string>::const_iterator name =
      std::find(joint_state.name.begin(), joint_state.name.end(), joint_name);

  bool success = false;
  if (name != joint_state.name.end()) {
    int32_t distance = std::distance(joint_state.name.begin(), name);

    extracted_state_out.position.push_back(joint_state.position.at(distance));

    extracted_state_out.velocity.push_back(joint_state.velocity.at(distance));

    extracted_state_out.effort.push_back(joint_state.effort.at(distance));

    extracted_state_out.name.push_back(*name);

    success = true;
  } else {
    success = false;
  }

  return success;
}
};  // namespace tmc_manipulation_util
