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
/// @file     joint_trajectory_configuration.cpp
/// @brief    General function for handling joint trajectories
#include <tmc_manipulation_util/joint_trajectory_configuration.hpp>

#include <algorithm>
#include <string>
#include <vector>

namespace {
/// Retrieve joint angles from joint_states
/// Return false if not found
bool GetJointPosition(const sensor_msgs::msg::JointState& joint_states,
                      const std::string& joint,
                      double& angle_out) {
  for (unsigned int i = 0; i < joint_states.name.size(); ++i) {
    if ((joint_states.name[i] == joint) && i < joint_states.position.size()) {
      angle_out = joint_states.position[i];
      return true;
    }
  }
  return false;
}
}  // namespace

namespace tmc_manipulation_util {

/// From joint_trajectory with joint_names
/// Extract only the specified joints to create joint_trajectory
/// Retrieve joints not present in joint_trajectory from joint_state.
/// If not found there either, consider it a failure
/// @param[in] joint_trajectory Input trajectory
/// @param[in] joint_names Joint names
/// @param[in] joint_state Joint state
/// @param[out] partial_joint_trajectory_out Output trajectory
/// @return Success or failure
bool ExtractTrajectory(
    const trajectory_msgs::msg::JointTrajectory& joint_trajectory,
    const std::vector<std::string>& joint_names,
    const sensor_msgs::msg::JointState& joint_state,
    trajectory_msgs::msg::JointTrajectory& partial_joint_trajectory_out) {
  uint32_t num_points = joint_trajectory.points.size();
  uint32_t num_joints = joint_names.size();
  // Create corresponding numbers for joint_trajectory, -1 indicates no correspondence
  std::vector<int32_t> index_map(num_joints);
  std::vector<double> position_map(num_joints);
  for (unsigned int joint_index = 0; joint_index < num_joints; ++joint_index) {
    index_map[joint_index] = -1;
    for (unsigned int input_index = 0; input_index < joint_trajectory.joint_names.size(); ++input_index) {
      if (joint_trajectory.joint_names[input_index] == joint_names[joint_index]) {
        index_map[joint_index] = input_index;
      }
    }
  }

  partial_joint_trajectory_out = trajectory_msgs::msg::JointTrajectory();
  partial_joint_trajectory_out.header = joint_trajectory.header;
  partial_joint_trajectory_out.joint_names = joint_names;

  partial_joint_trajectory_out.points.resize(num_points);
  for (unsigned int point_index = 0; point_index < num_points; ++point_index) {
    partial_joint_trajectory_out.points[point_index].positions.resize(num_joints);
    partial_joint_trajectory_out.points[point_index].time_from_start =
        joint_trajectory.points[point_index].time_from_start;

    bool has_velocities = !(joint_trajectory.points[point_index].velocities.empty());
    bool has_accelerations = !(joint_trajectory.points[point_index].accelerations.empty());
    bool has_effort = !(joint_trajectory.points[point_index].effort.empty());
    if (has_velocities) partial_joint_trajectory_out.points[point_index].velocities.resize(num_joints);
    if (has_accelerations) partial_joint_trajectory_out.points[point_index].accelerations.resize(num_joints);
    if (has_effort) partial_joint_trajectory_out.points[point_index].effort.resize(num_joints);

    for (unsigned int joint_index = 0; joint_index < num_joints; ++joint_index) {
      if (index_map[joint_index] != -1) {
        partial_joint_trajectory_out.points[point_index].positions[joint_index] =
            joint_trajectory.points[point_index].positions[index_map[joint_index]];
        if (has_velocities) {
          partial_joint_trajectory_out.points[point_index].velocities[joint_index] =
              joint_trajectory.points[point_index].velocities[index_map[joint_index]];
        }
        if (has_accelerations) {
          partial_joint_trajectory_out.points[point_index].accelerations[joint_index] =
              joint_trajectory.points[point_index].accelerations[index_map[joint_index]];
        }
        if (has_effort) {
          partial_joint_trajectory_out.points[point_index].effort[joint_index] =
              joint_trajectory.points[point_index].effort[index_map[joint_index]];
        }
      } else {
        double angle = 0.0;
        if (!GetJointPosition(joint_state, joint_names[joint_index], angle)) {
          return false;
        }
        partial_joint_trajectory_out.points[point_index].positions[joint_index] = angle;
        if (has_velocities) partial_joint_trajectory_out.points[point_index].velocities[joint_index] = 0.0;
        if (has_accelerations) partial_joint_trajectory_out.points[point_index].accelerations[joint_index] = 0.0;
        if (has_effort) partial_joint_trajectory_out.points[point_index].effort[joint_index] = 0.0;
      }
    }
  }
  return true;
}

/// Merge original_trajectory and additional_trajectory, which have the same number of points for different joints, into a single trajectory
/// Merge original_trajectory and additional_trajectory into a single trajectory
/// Failure occurs if the size of points differs
/// time_from_start is aligned with the original
/// @param[in] original_trajectory Original input trajectory
/// @param[in] additional_trajectory Additional input trajectory
/// @param[out] merged_trajectory_out Output trajectory
/// @return Success or failure
bool MergeJointTrajectory(
    const trajectory_msgs::msg::JointTrajectory& original_trajectory,
    const trajectory_msgs::msg::JointTrajectory& additional_trajectory,
    trajectory_msgs::msg::JointTrajectory& merged_trajectory_out) {
  if (original_trajectory.points.size() != additional_trajectory.points.size()) {
    return false;
  }
  // Prepare a check vector to maintain the order of joints in merged_trajectory_out
  auto joint_names = original_trajectory.joint_names;
  joint_names.insert(joint_names.begin(),
                     additional_trajectory.joint_names.begin(),
                     additional_trajectory.joint_names.end());
  std::sort(joint_names.begin(), joint_names.end());
  if (std::unique(joint_names.begin(), joint_names.end()) != joint_names.end()) {
    return false;
  }

  merged_trajectory_out = original_trajectory;
  merged_trajectory_out.joint_names.insert(
      merged_trajectory_out.joint_names.begin(),
      additional_trajectory.joint_names.begin(),
      additional_trajectory.joint_names.end());
  auto num_points = merged_trajectory_out.points.size();
  for (unsigned int point_index = 0; point_index < num_points; ++point_index) {
    merged_trajectory_out.points[point_index].positions.insert(
        merged_trajectory_out.points[point_index].positions.begin(),
        additional_trajectory.points[point_index].positions.begin(),
        additional_trajectory.points[point_index].positions.end());
    merged_trajectory_out.points[point_index].velocities.insert(
        merged_trajectory_out.points[point_index].velocities.begin(),
        additional_trajectory.points[point_index].velocities.begin(),
        additional_trajectory.points[point_index].velocities.end());
    merged_trajectory_out.points[point_index].accelerations.insert(
        merged_trajectory_out.points[point_index].accelerations.begin(),
        additional_trajectory.points[point_index].accelerations.begin(),
        additional_trajectory.points[point_index].accelerations.end());
    merged_trajectory_out.points[point_index].effort.insert(
        merged_trajectory_out.points[point_index].effort.begin(),
        additional_trajectory.points[point_index].effort.begin(),
        additional_trajectory.points[point_index].effort.end());
  }
  return true;
}
}  // namespace tmc_manipulation_util
