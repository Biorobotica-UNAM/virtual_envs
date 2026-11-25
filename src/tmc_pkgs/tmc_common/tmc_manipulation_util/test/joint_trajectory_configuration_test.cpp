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
/// @file     joint_trajectory_configuration_test.cpp
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include <tmc_manipulation_util/joint_trajectory_configuration.hpp>

namespace {
sensor_msgs::msg::JointState MakeJointState() {
  sensor_msgs::msg::JointState joint_state;
  joint_state.name = {"head", "arm", "wrist", "hand", "odom"};
  joint_state.position = {99.0, 99.0, 99.0, 99.0, 99.0};
  joint_state.velocity = {99.0, 99.0, 99.0, 99.0, 99.0};
  joint_state.effort = {99.0, 99.0, 99.0, 99.0, 99.0};
  return joint_state;
}

// Create a trajectory for validation
// 3 points
// Position is 0.1, 1.1, 2.1 for all axes
// Velocity is 0.2, 1.2, 2.2 for all axes
// Acceleration is 0.3, 1.3, 2.3 for all axes
// Effort is 0.4, 1.4, 2.4 for all axes
// Time is 0.0, 1.0, 2.0
trajectory_msgs::msg::JointTrajectory MakeTrajectory() {
  trajectory_msgs::msg::JointTrajectory joint_trajectory;
  joint_trajectory.header.stamp.sec = 1;
  joint_trajectory.header.frame_id = "ref_frame";
  joint_trajectory.joint_names = {"head", "arm", "wrist", "gripper"};

  for (auto i = 0; i < 3; ++i) {
    trajectory_msgs::msg::JointTrajectoryPoint point;
    point.positions = std::vector<double>(4, static_cast<double>(i) + 0.1);
    point.velocities = std::vector<double>(4, static_cast<double>(i) + 0.2);
    point.accelerations = std::vector<double>(4, static_cast<double>(i) + 0.3);
    point.effort = std::vector<double>(4, static_cast<double>(i) + 0.4);
    point.time_from_start.sec = i;
    joint_trajectory.points.push_back(point);
  }
  return joint_trajectory;
}

// Create a trajectory for validation head/odom
// Specified number of points
// Position is 0.1, 1.1, 2.1 for all axes...
// Velocity is 0.2, 1.2, 2.2 for all axes...
// Acceleration is 0.3, 1.3, 2.3 for all axes...
// Effort is 0.4, 1.4, 2.4 for all axes...
// Time is 0.0, 1.0, 2.0...
trajectory_msgs::msg::JointTrajectory MakeHeadTrajectory(const uint32_t num_of_points) {
  trajectory_msgs::msg::JointTrajectory joint_trajectory;
  joint_trajectory.joint_names = {"head", "odom"};

  for (unsigned int i = 0; i < num_of_points; ++i) {
    trajectory_msgs::msg::JointTrajectoryPoint point;
    point.positions = std::vector<double>(2, static_cast<double>(i) + 0.1);
    point.velocities = std::vector<double>(2, static_cast<double>(i) + 0.2);
    point.accelerations = std::vector<double>(2, static_cast<double>(i) + 0.3);
    point.effort = std::vector<double>(2, static_cast<double>(i) + 0.4);
    point.time_from_start.sec = i;
    joint_trajectory.points.push_back(point);
  }
  return joint_trajectory;
}

// Create a trajectory for validation arm/wrist/hand
// Specified number of points
// Position is 0.1, 1.1, 2.1 for all axes...
// Velocity is 0.2, 1.2, 2.2 for all axes...
// Acceleration is 0.3, 1.3, 2.3 for all axes...
// Effort is 0.4, 1.4, 2.4 for all axes...
// Time is 0.5, 1.5, 2.5...
trajectory_msgs::msg::JointTrajectory MakeArmTrajectory(const uint32_t num_of_points) {
  trajectory_msgs::msg::JointTrajectory joint_trajectory;
  joint_trajectory.joint_names = {"arm", "wrist", "hand"};

  for (unsigned int i = 0; i < num_of_points; ++i) {
    trajectory_msgs::msg::JointTrajectoryPoint point;
    point.positions = std::vector<double>(3, static_cast<double>(i) + 0.1);
    point.velocities = std::vector<double>(3, static_cast<double>(i) + 0.2);
    point.accelerations = std::vector<double>(3, static_cast<double>(i) + 0.3);
    point.effort = std::vector<double>(3, static_cast<double>(i) + 0.4);
    point.time_from_start.sec = i;
    point.time_from_start.nanosec = 500'000'000;
    joint_trajectory.points.push_back(point);
  }
  return joint_trajectory;
}
}  // namespace

namespace tmc_manipulation_util {
// When only joints existing in trajectory information are specified, results for all specified joints are returned
TEST(ExtractTrajectory, existAllJoints) {
  const auto joint_trajectory = MakeTrajectory();
  const auto joint_state = MakeJointState();
  const std::vector<std::string> joint_names = {"head", "wrist", "gripper"};

  // Confirm that processing returns normally
  trajectory_msgs::msg::JointTrajectory joint_trajectory_dst;
  EXPECT_TRUE(ExtractTrajectory(joint_trajectory,
                                joint_names,
                                joint_state,
                                joint_trajectory_dst));
  // Confirm that the header is copied
  EXPECT_EQ(1, joint_trajectory_dst.header.stamp.sec);
  EXPECT_EQ(0u, joint_trajectory_dst.header.stamp.nanosec);
  EXPECT_EQ("ref_frame", joint_trajectory_dst.header.frame_id);
  // Confirm that the extracted joints are correct
  EXPECT_EQ(joint_names, joint_trajectory_dst.joint_names);
  // Confirm that the number of points is correct
  ASSERT_EQ(3u, joint_trajectory_dst.points.size());

  for (auto point_index = 0; point_index < 3; ++point_index) {
    // Confirm that position information is correct
    ASSERT_EQ(joint_names.size(), joint_trajectory_dst.points[point_index].positions.size());
    for (unsigned int joint_index = 0; joint_index < joint_names.size(); ++joint_index) {
      EXPECT_EQ(static_cast<double>(point_index) + 0.1,
                joint_trajectory_dst.points[point_index].positions[joint_index]);
    }
    // Confirm that velocity information is correct
    ASSERT_EQ(joint_names.size(), joint_trajectory_dst.points[point_index].velocities.size());
    for (unsigned int joint_index = 0; joint_index < joint_names.size(); ++joint_index) {
      EXPECT_EQ(static_cast<double>(point_index) + 0.2,
                joint_trajectory_dst.points[point_index].velocities[joint_index]);
    }
    // Confirm that acceleration information is correct
    ASSERT_EQ(joint_names.size(), joint_trajectory_dst.points[point_index].accelerations.size());
    for (unsigned int joint_index = 0; joint_index < joint_names.size(); ++joint_index) {
      EXPECT_EQ(static_cast<double>(point_index) + 0.3,
                joint_trajectory_dst.points[point_index].accelerations[joint_index]);
    }
    // Confirm that effort information is correct
    ASSERT_EQ(joint_names.size(), joint_trajectory_dst.points[point_index].effort.size());
    for (unsigned int joint_index = 0; joint_index < joint_names.size(); ++joint_index) {
      EXPECT_EQ(static_cast<double>(point_index) + 0.4,
                joint_trajectory_dst.points[point_index].effort[joint_index]);
    }
    // Confirm that time values are correct
    EXPECT_EQ(joint_trajectory.points[point_index].time_from_start,
              joint_trajectory_dst.points[point_index].time_from_start);
  }
}

// Even when joints not existing in trajectory information are included, only results for joints existing in the JointState list are returned
TEST(ExtractTrajectory, includeNotExistJoint) {
  const auto joint_trajectory = MakeTrajectory();
  const auto joint_state = MakeJointState();
  const std::vector<std::string> joint_names = {"head", "odom"};

  // Confirm that processing returns normally
  trajectory_msgs::msg::JointTrajectory joint_trajectory_dst;
  EXPECT_TRUE(ExtractTrajectory(joint_trajectory,
                                joint_names,
                                joint_state,
                                joint_trajectory_dst));
  // Confirm that the extracted joints are correct
  EXPECT_EQ(joint_names, joint_trajectory_dst.joint_names);
  // Confirm that the number of points is correct
  ASSERT_EQ(3u, joint_trajectory_dst.points.size());

  for (auto point_index = 0; point_index < 3; ++point_index) {
    // Confirm that position information is correct
    ASSERT_EQ(2u, joint_trajectory_dst.points[point_index].positions.size());
    EXPECT_EQ(static_cast<double>(point_index) + 0.1, joint_trajectory_dst.points[point_index].positions[0]);
    EXPECT_EQ(99.0, joint_trajectory_dst.points[point_index].positions[1]);
    // Confirm that velocity information is correct
    ASSERT_EQ(2u, joint_trajectory_dst.points[point_index].velocities.size());
    EXPECT_EQ(static_cast<double>(point_index) + 0.2, joint_trajectory_dst.points[point_index].velocities[0]);
    EXPECT_EQ(0.0, joint_trajectory_dst.points[point_index].velocities[1]);
    // Confirm that acceleration information is correct
    ASSERT_EQ(2u, joint_trajectory_dst.points[point_index].accelerations.size());
    EXPECT_EQ(static_cast<double>(point_index) + 0.3, joint_trajectory_dst.points[point_index].accelerations[0]);
    EXPECT_EQ(0.0, joint_trajectory_dst.points[point_index].accelerations[1]);
    // Confirm that effort information is correct
    ASSERT_EQ(2u, joint_trajectory_dst.points[point_index].effort.size());
    EXPECT_EQ(static_cast<double>(point_index) + 0.4, joint_trajectory_dst.points[point_index].effort[0]);
    EXPECT_EQ(0.0, joint_trajectory_dst.points[point_index].effort[1]);
    // Confirm that time values are correct
    EXPECT_EQ(joint_trajectory.points[point_index].time_from_start,
              joint_trajectory_dst.points[point_index].time_from_start);
  }
}

// If vel, acc, eff are not included in the input trajectory, they are not included in the output
TEST(ExtractTrajectory, onlyPosition) {
  auto input_trajectory = MakeTrajectory();
  for (auto& point : input_trajectory.points) {
    point.velocities.clear();
    point.accelerations.clear();
    point.effort.clear();
  }
  const auto joint_state = MakeJointState();
  const std::vector<std::string> joint_names = {"head", "odom"};

  trajectory_msgs::msg::JointTrajectory dst_trajectory;
  EXPECT_TRUE(ExtractTrajectory(input_trajectory, joint_names,
                                joint_state, dst_trajectory));

  ASSERT_EQ(input_trajectory.points.size(), dst_trajectory.points.size());
  // Since detailed numbers are checked in a separate test, only confirm the size
  for (const auto& point : dst_trajectory.points) {
    EXPECT_EQ(joint_names.size(), point.positions.size());
    EXPECT_TRUE(point.velocities.empty());
    EXPECT_TRUE(point.accelerations.empty());
    EXPECT_TRUE(point.effort.empty());
  }
}

// If non-existing joints are specified, false is returned
TEST(ExtractTrajectory, notExistInJointState) {
  const auto joint_trajectory = MakeTrajectory();
  const auto joint_state = MakeJointState();
  const std::vector<std::string> joint_names = {"head", "shoulder"};

  // Confirm that processing returns abnormally
  trajectory_msgs::msg::JointTrajectory joint_trajectory_dst;
  EXPECT_FALSE(ExtractTrajectory(joint_trajectory,
                                 joint_names,
                                 joint_state,
                                 joint_trajectory_dst));
}

// Confirm that trajectory merge processing is handled correctly
TEST(MergeJointTrajectory, normalCase) {
  const auto base_joint_trajectory_1 = MakeHeadTrajectory(3);
  const auto base_joint_trajectory_2 = MakeArmTrajectory(3);
  std::vector<std::string> joint_names = {"arm", "wrist", "hand", "head", "odom"};

  // Confirm that processing returns normally
  trajectory_msgs::msg::JointTrajectory merged_joint_trajectory;
  EXPECT_TRUE(MergeJointTrajectory(base_joint_trajectory_1,
                                   base_joint_trajectory_2,
                                   merged_joint_trajectory));
  // Confirm that the joint list after merging is correct
  EXPECT_EQ(joint_names, merged_joint_trajectory.joint_names);
  // Confirm that the number of points is correct
  EXPECT_EQ(3u, merged_joint_trajectory.points.size());

  for (uint32_t point_index = 0; point_index < 3; ++point_index) {
    // Confirm that position information is correct
    ASSERT_EQ(joint_names.size(), merged_joint_trajectory.points[point_index].positions.size());
    for (uint32_t joint_index = 0; joint_index < joint_names.size(); ++joint_index) {
      EXPECT_EQ(static_cast<double>(point_index) + 0.1,
                merged_joint_trajectory.points[point_index].positions[joint_index]);
    }
    // Confirm that velocity information is correct
    ASSERT_EQ(joint_names.size(), merged_joint_trajectory.points[point_index].velocities.size());
    for (uint32_t joint_index = 0; joint_index < joint_names.size(); ++joint_index) {
      EXPECT_EQ(static_cast<double>(point_index) + 0.2,
                merged_joint_trajectory.points[point_index].velocities[joint_index]);
    }
    // Confirm that acceleration information is correct
    ASSERT_EQ(joint_names.size(), merged_joint_trajectory.points[point_index].accelerations.size());
    for (uint32_t joint_index = 0; joint_index < joint_names.size(); ++joint_index) {
      EXPECT_EQ(static_cast<double>(point_index) + 0.3,
                merged_joint_trajectory.points[point_index].accelerations[joint_index]);
    }
    // Confirm that effort information is correct
    ASSERT_EQ(joint_names.size(), merged_joint_trajectory.points[point_index].effort.size());
    for (uint32_t joint_index = 0; joint_index < joint_names.size(); ++joint_index) {
      EXPECT_EQ(static_cast<double>(point_index) + 0.4,
                merged_joint_trajectory.points[point_index].effort[joint_index]);
    }
    // Confirm that time values are correct
    EXPECT_EQ(static_cast<int>(point_index), merged_joint_trajectory.points[point_index].time_from_start.sec);
  }
}

// If trajectories with different numbers of points are specified, false is returned
TEST(MergeJointTrajectory, numOfPointsIsDiff) {
  const auto base_joint_trajectory_1 = MakeHeadTrajectory(3);
  const auto base_joint_trajectory_2 = MakeArmTrajectory(5);

  // Confirm that processing returns abnormally
  trajectory_msgs::msg::JointTrajectory merged_joint_trajectory;
  EXPECT_FALSE(MergeJointTrajectory(base_joint_trajectory_1,
                                    base_joint_trajectory_2,
                                    merged_joint_trajectory));
}

// If trajectories containing the same joint name are specified, false is returned
TEST(MergeJointTrajectory, includeSameJoint) {
  const auto joint_trajectory = MakeHeadTrajectory(3);
  trajectory_msgs::msg::JointTrajectory merged_joint_trajectory;
  EXPECT_FALSE(MergeJointTrajectory(joint_trajectory, joint_trajectory,
                                    merged_joint_trajectory));
}

}  // namespace tmc_manipulation_util

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
