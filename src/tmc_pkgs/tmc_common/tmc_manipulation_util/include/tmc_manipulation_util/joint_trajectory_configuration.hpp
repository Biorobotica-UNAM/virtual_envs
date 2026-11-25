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
/// @file     joint_trajectory_configuration.hpp
/// @brief    General function for handling joint trajectories
#ifndef TMC_MANIPULATION_UTIL_JOINT_TRAJECTORY_CONFIGURATION_HPP_
#define TMC_MANIPULATION_UTIL_JOINT_TRAJECTORY_CONFIGURATION_HPP_
#include <string>
#include <vector>

#include <sensor_msgs/msg/joint_state.hpp>
#include <trajectory_msgs/msg/joint_trajectory.hpp>

namespace tmc_manipulation_util {
/// From joint_trajectory, using joint_names
/// Extract only the specified joints to create joint_trajectory
/// If a joint does not exist in joint_trajectory, obtain it from joint_state.
/// If it is not there either, it is considered a failure
/// @param[in] joint_trajectory Input trajectory
/// @param[in] joint_names Joint names
/// @param[in] joint_state Joint state
/// @param[out] partial_joint_trajectory_out Output trajectory
/// @return Success or failure
bool ExtractTrajectory(const trajectory_msgs::msg::JointTrajectory& joint_trajectory,
                       const std::vector<std::string>& joint_names,
                       const sensor_msgs::msg::JointState& joint_state,
                       trajectory_msgs::msg::JointTrajectory& partial_joint_trajectory_out);

/// Merge original_trajectory and additional_trajectory of different joints
/// with the same number of points into a single trajectory
/// If the size of points differs, it is considered a failure
/// time_from_start is aligned with the original
/// @param[in] original_trajectory Original input trajectory
/// @param[in] additional_trajectory Additional input trajectory
/// @param[out] merged_trajectory_out Output trajectory
/// @return Success or failure
bool MergeJointTrajectory(const trajectory_msgs::msg::JointTrajectory& original_trajectory,
                          const trajectory_msgs::msg::JointTrajectory& additional_trajectory,
                          trajectory_msgs::msg::JointTrajectory& merged_trajectory_out);
}  // namespace tmc_manipulation_util
#endif  // TMC_MANIPULATION_UTIL_JOINT_TRAJECTORY_CONFIGURATION_HPP_
