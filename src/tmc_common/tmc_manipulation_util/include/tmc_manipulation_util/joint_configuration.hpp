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
/// @file     joint_configuration.hpp
/// @brief Functions for handling joint postures and Trajectory (joint_configuration)
/// @version  1.0.0
/// @author   Kiyohiro Sogen
/// @date     2011.10.25
#ifndef JOINT_CONFIGURATION_HPP_
#define JOINT_CONFIGURATION_HPP_
#include <string>
#include <vector>
#include <ros/ros.h>
#include <sensor_msgs/JointState.h>
#include <trajectory_msgs/JointTrajectory.h>


namespace tmc_manipulation_util {

/// @brief Retrieve joint names from joint_name and joint values from param_name,
/// and return as JointTrajectory
/// @param[in] node Node handle for parameter server access
/// @param[in] joint_name Parameter name of the joint name array
/// @param[in] param_name Parameter name of the joint value array
/// @param[in] param_name Time parameter name
/// @param[out] dst_joint_trajecotry JointTrajectory containing joint names and joint values
/// @return true: Success false: Failure
bool GetJointConfiguration(const ros::NodeHandle& node, const std::string& joint_name, const std::string& param_name,
                           const std::string& time_name, trajectory_msgs::JointTrajectory& dst_joint_trajectory);

/// @brief Retrieve joint values from param_name and return as JointTrajectory
/// Joint names are retrieved from joint_names.
/// @param[in] node Node handle for parameter server access
/// @param[in] param_name Parameter name of the joint value array
/// @param[in] param_name Time parameter name
/// @param[out] dst_joint_trajecotry JointTrajectory containing joint names and joint values
/// @return true: Success false: Failure
bool GetJointConfiguration(const ros::NodeHandle& node, const std::string& param_name, const std::string& time_name,
                           trajectory_msgs::JointTrajectory& dst_joint_trajectory);

/// @brief Retrieve joint names from joint_name and joint values from param_name,
/// and return as JointState
/// @param[in] node Node handle for parameter server access
/// @param[in] joint_name Parameter name of the joint name array
/// @param[in] param_name Parameter name of the joint value array
/// @param[out] dst_joint_state JointState containing joint names and joint values
/// @return true: Success false: Failure
bool GetJointConfiguration(const ros::NodeHandle& node, const std::string& joint_name, const std::string& param_name,
                           sensor_msgs::JointState& dst_joint_state);

/// @brief Retrieve joint values from param_name and return as JointState
/// Joint names are retrieved from joint_names.
/// @param[in] node Node handle for parameter server access
/// @param[in] param_name Parameter name of the joint value array
/// @param[out] dst_joint_state JointState containing joint names and joint values
/// @return true: Success false: Failure
bool GetJointConfiguration(const ros::NodeHandle& node, const std::string& param_name,
                           sensor_msgs::JointState& dst_joint_state);

/// @brief Retrieve joint names from joint_name and joint values from param_name,
/// and return as JointState
/// @param[in] node Node handle for parameter server access
/// @param[in] joint_name Parameter name of the joint name array
/// @param[in] param_name Parameter name of the joint value array
/// @param[out] dst_joint_states Array of JointState
/// @return true: Success false: Failure
bool GetJointConfiguration(const ros::NodeHandle& node, const std::string& joint_name, const std::string& param_name,
                           std::vector<sensor_msgs::JointState>& dst_joint_states);

/// @brief Retrieve joint values from param_name and return as JointState
/// Joint names are retrieved from joint_names.
/// @param[in] node Node handle for parameter server access
/// @param[in] param_name Parameter name of the joint value array
/// @param[out] dst_joint_states Array of JointState
/// @return true: Success false: Failure
bool GetJointConfiguration(const ros::NodeHandle& node, const std::string& param_name,
                           std::vector<sensor_msgs::JointState>& dst_joint_states);

/// @brief Load an array of joint names from joint_name
bool LoadJointName(const ros::NodeHandle& node, const std::string& joint_name,
                   std::vector<std::string>& dst_joint_name);

/// @brief Overwrite joint angles in joint_state with overwriting_state.
///        Joint angles in overwriting_state that are not in joint_state
///        will not be reflected in overwritten_state
/// @param[in] joint_state The one to be overwritten
/// @param[in] overwriting_state The one to overwrite
/// @param[out] overwritten_state_out Overwritten JointState
/// @return bool True if the merge is successful
/// @note The order of joints in overwritten_state_out is the same as in joint_state
bool OverwriteJointState(const sensor_msgs::JointState& joint_state, const sensor_msgs::JointState& overwriting_state,
                         sensor_msgs::JointState& overwritten_state_out);

/// @brief Merge joint angles in joint_state with merge_state.
///        For joints existing in both joint_state and merge_state,
///        joint angles in merge_state will be reflected in merged_state
/// @param[in] joint_state The one to be merged
/// @param[in] merge_state The one to merge
/// @param[out] merged_state_out Merged JointState
/// @return bool True if the merge is successful
/// @note The order of joints in merged_state_out is the same as in joint_state,
///       and joints not existing in joint_state but existing in merge_state
///       will be added after the joints in joint_state in merged_state_out
bool MergeJointState(const sensor_msgs::JointState& joint_state, const sensor_msgs::JointState& merge_state,
                     sensor_msgs::JointState& merged_state_out);

/// @brief Extract joint angles from joint_state that match the names in joint_names
/// @param[in] joint_state Joint information
/// @param[in] joint_names Joint names
/// @param[out] extracted_state_out Joint information extracted only for joints in joint_names
/// @return bool True if extraction is successful
bool ExtractJointState(const sensor_msgs::JointState& joint_state, const std::vector<std::string>& joint_names,
                       sensor_msgs::JointState& extracted_state_out);

/// @brief Extract only those from joint_state that match the name in joint_name
/// @param[in] joint_state Joint information
/// @param[in] joint_name Joint name
/// @param[out] extracted_state_out Joint information extracted only for the joint in joint_name
/// @return bool True if extraction is successful
bool ExtractJointState(const sensor_msgs::JointState& joint_state, const std::string& joint_name,
                       sensor_msgs::JointState& extracted_state_out);


};  // namespace tmc_manipulation_util


#endif  // JOINT_CONFIGURATION_HPP_
