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
/// @file     uint.cpp
#include <string>
#include <vector>
#include <gtest/gtest.h>
#include <tmc_manipulation_util/tmc_manipulation_util.hpp>

namespace {
const double kThreshEqual = 0.000001;
}

TEST(GetIndexedString, aa) {
  using tmc_manipulation_util::GetIndexedString;

  EXPECT_EQ("joint1/name", GetIndexedString("joint", 1, "name"));
  EXPECT_EQ("pre100/val", GetIndexedString("pre", 100, "val"));
  EXPECT_EQ("pre0/hoge", GetIndexedString("pre", -0, "hoge"));
  EXPECT_EQ("pre0/hoge", GetIndexedString("pre", 0, "hoge"));
  EXPECT_EQ("pre0/hoge", GetIndexedString("pre", 0.1, "hoge"));
  EXPECT_EQ("pre-1/hoge", GetIndexedString("pre", -1, "hoge"));
}


TEST(GetParamAtIndex, Exist) {
  using tmc_manipulation_util::GetParamAtIndex;
  using tmc_manipulation_util::SetParamAtIndex;

  ros::NodeHandle node;
  SetParamAtIndex(node, "test", 0, "name", "test0");
  node.setParam("test1/name", "test1");
  node.setParam("test1/type", "linear");
  SetParamAtIndex(node, "test", 1, "mode", 1);
  node.setParam("test2/max_velocity", 100.0);

  std::string value0;
  bool exist_name0 = GetParamAtIndex(node, "test", 0, "name", value0);
  EXPECT_TRUE(exist_name0);
  EXPECT_EQ("test0", value0);

  std::string value1;
  bool exist_name1 = GetParamAtIndex(node, "test", 1, "name", value1);
  EXPECT_TRUE(exist_name1);
  EXPECT_EQ("test1", value1);

  std::string type;
  bool exist_type = GetParamAtIndex(node, "test", 1, "type", type);
  EXPECT_TRUE(exist_type);
  EXPECT_EQ("linear", type);

  int mode;
  bool exist_mode = GetParamAtIndex(node, "test", 1, "mode", mode);
  EXPECT_TRUE(exist_mode);
  EXPECT_EQ(1, mode);

  double vel;
  bool exist_vel = GetParamAtIndex(node, "test", 2, "max_velocity", vel);
  EXPECT_TRUE(exist_vel);
  EXPECT_DOUBLE_EQ(100.0, vel);
}

TEST(GetParamAtIndex, NoExist) {
  using tmc_manipulation_util::GetParamAtIndex;

  ros::NodeHandle node("~");

  std::string value0;
  bool exist_name0 = GetParamAtIndex(node, "test", 0, "name", value0);
  EXPECT_FALSE(exist_name0);

  std::string value1;
  bool exist_name1 = GetParamAtIndex(node, "test", 1, "name", value1);
  EXPECT_FALSE(exist_name1);

  std::string type;
  bool exist_type = GetParamAtIndex(node, "test", 1, "type", type);
  EXPECT_FALSE(exist_type);

  int mode;
  bool exist_mode = GetParamAtIndex(node, "test", 1, "mode", mode);
  EXPECT_FALSE(exist_mode);

  double vel;
  bool exist_vel = GetParamAtIndex(node, "test", 2, "max_velocity", vel);
  EXPECT_FALSE(exist_vel);
}


TEST(GetSetJointParamAtIndex, Exist) {
  using tmc_manipulation_util::GetJointParamAtIndex;
  using tmc_manipulation_util::SetJointParamAtIndex;

  ros::NodeHandle node;

  node.setParam("joint0/name", "joint0");
  SetJointParamAtIndex<std::string>(node, 1, "name", "joint1");
  node.setParam("joint1/type", "linear");
  node.setParam("joint1/mode", 1);
  SetJointParamAtIndex(node, 2, "max_velocity", 100.0);

  std::string value0;
  bool exist_name0 = GetJointParamAtIndex(node, 0, "name", value0);
  EXPECT_TRUE(exist_name0);
  EXPECT_EQ("joint0", value0);

  std::string value1;
  bool exist_name1 = GetJointParamAtIndex(node, 1, "name", value1);
  EXPECT_TRUE(exist_name1);
  EXPECT_EQ("joint1", value1);

  std::string type;
  bool exist_type = GetJointParamAtIndex(node, 1, "type", type);
  EXPECT_TRUE(exist_type);
  EXPECT_EQ("linear", type);

  int mode;
  bool exist_mode = GetJointParamAtIndex(node, 1, "mode", mode);
  EXPECT_TRUE(exist_mode);
  EXPECT_EQ(1, mode);

  double vel;
  bool exist_vel = GetJointParamAtIndex(node, 2, "max_velocity", vel);
  EXPECT_TRUE(exist_vel);
  EXPECT_DOUBLE_EQ(100.0, vel);
}

TEST(GetSetJointParams, Exist) {
  using tmc_manipulation_util::GetJointParams;
  using tmc_manipulation_util::SetJointParams;
  using tmc_manipulation_util::SetJointParamAtIndex;
  ros::NodeHandle node;
  std::vector<int> values;
  values.push_back(1);
  values.push_back(-1);
  values.push_back(5);

  SetJointParams(node, "hoge", values);
  SetJointParamAtIndex(node, 3, "hoge", 0);

  std::vector<int> output;
  EXPECT_EQ(4, GetJointParams(node, "hoge", output));
  EXPECT_EQ(1, output.at(0));
  EXPECT_EQ(-1, output.at(1));
  EXPECT_EQ(5, output.at(2));
  EXPECT_EQ(0, output.at(3));
}


TEST(GetJointStateIndexByName, Found) {
  using tmc_manipulation_util::GetJointStateIndexByName;
  sensor_msgs::JointState joint_state;
  joint_state.name.push_back("joint0");
  joint_state.name.push_back("joint1");
  std::string name0("joint0");
  std::string name1("joint1");

  try {
    EXPECT_EQ(0, GetJointStateIndexByName(joint_state, name0));
    EXPECT_EQ(1, GetJointStateIndexByName(joint_state, name1));
  } catch (std::invalid_argument& ex) {
    EXPECT_TRUE(false);
  }
}


TEST(GetJointStateIndexByName, NotFound) {
  using tmc_manipulation_util::GetJointStateIndexByName;
  sensor_msgs::JointState joint_state;
  joint_state.name.push_back("joint0");
  joint_state.name.push_back("joint1");
  std::string name2("joint2");

  EXPECT_EQ(-1, GetJointStateIndexByName(joint_state, name2));
}

TEST(ConvertJointStateMsgToJointVec, FoundAll) {
  using tmc_manipulation_util::ConvertJointStateMsgToJointVec;
  sensor_msgs::JointState joint_state;
  joint_state.name.push_back("joint0");
  joint_state.name.push_back("joint1");
  joint_state.position.push_back(0.3);
  joint_state.position.push_back(0.4);
  std::vector<double> joint_vec;
  std::vector<std::string> joint_name_list;
  joint_name_list.push_back("joint1");
  joint_name_list.push_back("joint0");
  EXPECT_TRUE(ConvertJointStateMsgToJointVec(joint_state, joint_name_list, joint_vec));
  EXPECT_EQ(2, static_cast<int32_t>(joint_vec.size()));
  EXPECT_EQ(0.4, joint_vec[0]);
  EXPECT_EQ(0.3, joint_vec[1]);
}

TEST(GetJointConfigurationTrajectory, Normal) {
  using tmc_manipulation_util::GetJointConfiguration;
  ros::NodeHandle node;
  trajectory_msgs::JointTrajectory joint_trajectory;
  // Called without specifying joint_name
  EXPECT_TRUE(GetJointConfiguration(node, "initial", "initial_time", joint_trajectory));
  ASSERT_EQ(joint_trajectory.joint_names.size(), 3U);
  EXPECT_EQ(joint_trajectory.joint_names[0], "CARM/LINEAR");
  EXPECT_EQ(joint_trajectory.joint_names[1], "CARM/SHOULDER_Y");
  EXPECT_EQ(joint_trajectory.joint_names[2], "CARM/SHOULDER_R");

  ASSERT_EQ(joint_trajectory.points.size(), 1U);
  ASSERT_EQ(joint_trajectory.points[0].positions.size(), 3U);
  EXPECT_EQ(joint_trajectory.points[0].positions[0], 0.1);
  EXPECT_EQ(joint_trajectory.points[0].positions[1], -0.1);
  EXPECT_EQ(joint_trajectory.points[0].positions[2], -0.2);
  EXPECT_NEAR(joint_trajectory.points[0].time_from_start.toSec(), 0.5, kThreshEqual);


  // Called with correctly specified joint_name
  EXPECT_TRUE(GetJointConfiguration(node, "joint_names", "reach", "reach_time", joint_trajectory));
  ASSERT_EQ(joint_trajectory.joint_names.size(), 3U);
  EXPECT_EQ(joint_trajectory.joint_names[0], "CARM/LINEAR");
  EXPECT_EQ(joint_trajectory.joint_names[1], "CARM/SHOULDER_Y");
  EXPECT_EQ(joint_trajectory.joint_names[2], "CARM/SHOULDER_R");

  ASSERT_EQ(joint_trajectory.points.size(), 1U);
  ASSERT_EQ(joint_trajectory.points[0].positions.size(), 3U);
  EXPECT_EQ(joint_trajectory.points[0].positions[0], 0.0);
  EXPECT_EQ(joint_trajectory.points[0].positions[1], 0.9);
  EXPECT_EQ(joint_trajectory.points[0].positions[2], 0.8);
  EXPECT_NEAR(joint_trajectory.points[0].time_from_start.toSec(), 0.3, kThreshEqual);

  // Called with correctly specified joint_name to obtain trajectory
  EXPECT_TRUE(GetJointConfiguration(node, "joint_names", "take", "take_time", joint_trajectory));
  ASSERT_EQ(joint_trajectory.joint_names.size(), 3U);
  EXPECT_EQ(joint_trajectory.joint_names[0], "CARM/LINEAR");
  EXPECT_EQ(joint_trajectory.joint_names[1], "CARM/SHOULDER_Y");
  EXPECT_EQ(joint_trajectory.joint_names[2], "CARM/SHOULDER_R");

  ASSERT_EQ(joint_trajectory.points.size(), 2U);
  ASSERT_EQ(joint_trajectory.points[0].positions.size(), 3U);
  EXPECT_EQ(joint_trajectory.points[0].positions[0], 0.1);
  EXPECT_EQ(joint_trajectory.points[0].positions[1], -0.1);
  EXPECT_EQ(joint_trajectory.points[0].positions[2], -0.2);
  EXPECT_NEAR(joint_trajectory.points[0].time_from_start.toSec(), 0.3, kThreshEqual);
  ASSERT_EQ(joint_trajectory.points[1].positions.size(), 3U);
  EXPECT_EQ(joint_trajectory.points[1].positions[0], 0.0);
  EXPECT_EQ(joint_trajectory.points[1].positions[1], 0.9);
  EXPECT_EQ(joint_trajectory.points[1].positions[2], 0.8);
  EXPECT_NEAR(joint_trajectory.points[1].time_from_start.toSec(), 0.2, kThreshEqual);

  // Called with correctly specified joint_name and string parameter for time
  EXPECT_TRUE(GetJointConfiguration(node, "joint_names", "take", "take_time2", joint_trajectory));
  ASSERT_EQ(joint_trajectory.joint_names.size(), 3U);
  EXPECT_EQ(joint_trajectory.joint_names[0], "CARM/LINEAR");
  EXPECT_EQ(joint_trajectory.joint_names[1], "CARM/SHOULDER_Y");
  EXPECT_EQ(joint_trajectory.joint_names[2], "CARM/SHOULDER_R");

  ASSERT_EQ(joint_trajectory.points.size(), 2U);
  ASSERT_EQ(joint_trajectory.points[0].positions.size(), 3U);
  EXPECT_EQ(joint_trajectory.points[0].positions[0], 0.1);
  EXPECT_EQ(joint_trajectory.points[0].positions[1], -0.1);
  EXPECT_EQ(joint_trajectory.points[0].positions[2], -0.2);
  EXPECT_NEAR(joint_trajectory.points[0].time_from_start.toSec(), 1.0, kThreshEqual);
  ASSERT_EQ(joint_trajectory.points[1].positions.size(), 3U);
  EXPECT_EQ(joint_trajectory.points[1].positions[0], 0.0);
  EXPECT_EQ(joint_trajectory.points[1].positions[1], 0.9);
  EXPECT_EQ(joint_trajectory.points[1].positions[2], 0.8);
  EXPECT_NEAR(joint_trajectory.points[1].time_from_start.toSec(), 0.2, kThreshEqual);
}

TEST(GetJointConfigurationTrajectory, Abnormal) {
  using tmc_manipulation_util::GetJointConfiguration;
  ros::NodeHandle node;
  trajectory_msgs::JointTrajectory joint_trajectory;
  // Mismatch between the number of joints and joint names
  EXPECT_FALSE(GetJointConfiguration(node, "error", "initial_time", joint_trajectory));

  // Invalid joint_name
  EXPECT_FALSE(GetJointConfiguration(node, "error_joint_names", "initial", "initial_time", joint_trajectory));

  // Invalid param_name
  EXPECT_FALSE(GetJointConfiguration(node, "joint_names", "error_take", "initial_time", joint_trajectory));

  // Invalid time_name
  EXPECT_FALSE(GetJointConfiguration(node, "joint_names", "take", "invalid_time_name", joint_trajectory));
  // Invalid size of time
  EXPECT_FALSE(GetJointConfiguration(node, "joint_names", "initial", "error_time", joint_trajectory));

  // Invalid size of time
  EXPECT_FALSE(GetJointConfiguration(node, "joint_names", "take", "error_take_time", joint_trajectory));

  // Invalid size of time
  EXPECT_FALSE(GetJointConfiguration(node, "joint_names", "take", "error_take_time2", joint_trajectory));

  ros::NodeHandle error_node("error_namespace");
  // Invalid node
  EXPECT_FALSE(GetJointConfiguration(error_node, "joint_names", "take", "take_time", joint_trajectory));
}

TEST(GetJointConfigurationState, Normal) {
  using tmc_manipulation_util::GetJointConfiguration;
  ros::NodeHandle node;
  sensor_msgs::JointState joint_state;
  // Called without specifying joint_name
  EXPECT_TRUE(GetJointConfiguration(node, "initial", joint_state));
  ASSERT_EQ(joint_state.name.size(), 3U);
  EXPECT_EQ(joint_state.name[0], "CARM/LINEAR");
  EXPECT_EQ(joint_state.name[1], "CARM/SHOULDER_Y");
  EXPECT_EQ(joint_state.name[2], "CARM/SHOULDER_R");
  ASSERT_EQ(joint_state.position.size(), 3U);
  EXPECT_EQ(joint_state.position[0], 0.1);
  EXPECT_EQ(joint_state.position[1], -0.1);
  EXPECT_EQ(joint_state.position[2], -0.2);

  // Called with specified joint_name
  EXPECT_TRUE(GetJointConfiguration(node, "joint_names", "reach", joint_state));
  ASSERT_EQ(joint_state.name.size(), 3U);
  EXPECT_EQ(joint_state.name[0], "CARM/LINEAR");
  EXPECT_EQ(joint_state.name[1], "CARM/SHOULDER_Y");
  EXPECT_EQ(joint_state.name[2], "CARM/SHOULDER_R");
  ASSERT_EQ(joint_state.position.size(), 3U);
  EXPECT_EQ(joint_state.position[0], 0.0);
  EXPECT_EQ(joint_state.position[1], 0.9);
  EXPECT_EQ(joint_state.position[2], 0.8);

  std::vector<sensor_msgs::JointState> joint_states;
  // Called without specifying joint_name
  EXPECT_TRUE(GetJointConfiguration(node, "initial_states", joint_states));
  ASSERT_EQ(joint_states.size(), 2U);
  ASSERT_EQ(joint_states.at(0).name.size(), 3U);
  EXPECT_EQ(joint_states.at(0).name[0], "CARM/LINEAR");
  EXPECT_EQ(joint_states.at(0).name[1], "CARM/SHOULDER_Y");
  EXPECT_EQ(joint_states.at(0).name[2], "CARM/SHOULDER_R");
  ASSERT_EQ(joint_states.at(0).position.size(), 3U);
  EXPECT_EQ(joint_states.at(0).position[0], 0.1);
  EXPECT_EQ(joint_states.at(0).position[1], -0.1);
  EXPECT_EQ(joint_states.at(0).position[2], -0.2);
  ASSERT_EQ(joint_states.at(1).name.size(), 3U);
  EXPECT_EQ(joint_states.at(1).name[0], "CARM/LINEAR");
  EXPECT_EQ(joint_states.at(1).name[1], "CARM/SHOULDER_Y");
  EXPECT_EQ(joint_states.at(1).name[2], "CARM/SHOULDER_R");
  ASSERT_EQ(joint_states.at(1).position.size(), 3U);
  EXPECT_EQ(joint_states.at(1).position[0], 0.0);
  EXPECT_EQ(joint_states.at(1).position[1], 0.9);
  EXPECT_EQ(joint_states.at(1).position[2], 0.8);

  // Called with specified joint_name
  EXPECT_TRUE(GetJointConfiguration(node, "joint_names", "reach_states", joint_states));
  ASSERT_EQ(joint_state.name.size(), 3U);
  ASSERT_EQ(joint_states.at(0).name.size(), 3U);
  EXPECT_EQ(joint_states.at(0).name[0], "CARM/LINEAR");
  EXPECT_EQ(joint_states.at(0).name[1], "CARM/SHOULDER_Y");
  EXPECT_EQ(joint_states.at(0).name[2], "CARM/SHOULDER_R");
  ASSERT_EQ(joint_states.at(0).position.size(), 3U);
  EXPECT_EQ(joint_states.at(0).position[0], 0.0);
  EXPECT_EQ(joint_states.at(0).position[1], 0.9);
  EXPECT_EQ(joint_states.at(0).position[2], 0.8);
  ASSERT_EQ(joint_states.at(1).name.size(), 3U);
  EXPECT_EQ(joint_states.at(1).name[0], "CARM/LINEAR");
  EXPECT_EQ(joint_states.at(1).name[1], "CARM/SHOULDER_Y");
  EXPECT_EQ(joint_states.at(1).name[2], "CARM/SHOULDER_R");
  ASSERT_EQ(joint_states.at(1).position.size(), 3U);
  EXPECT_EQ(joint_states.at(1).position[0], 0.1);
  EXPECT_EQ(joint_states.at(1).position[1], -0.1);
  EXPECT_EQ(joint_states.at(1).position[2], -0.2);
}

TEST(GetJointConfigurationState, Abnormal) {
  using tmc_manipulation_util::GetJointConfiguration;
  ros::NodeHandle node;
  sensor_msgs::JointState joint_state;
  // Mismatch between the number of joints and joint names
  EXPECT_FALSE(GetJointConfiguration(node, "error", joint_state));

  // Invalid joint_name
  EXPECT_FALSE(GetJointConfiguration(node, "error_joint_names", "initial", joint_state));

  // Invalid param_name
  EXPECT_FALSE(GetJointConfiguration(node, "joint_names", "error_take", joint_state));

  // Cannot receive trajectory (multiple single positions) with joint_state.
  EXPECT_FALSE(GetJointConfiguration(node, "joint_names", "take", joint_state));

  ros::NodeHandle error_node("error_namespace");
  // Invalid node
  EXPECT_FALSE(GetJointConfiguration(error_node, "joint_names", "initial", joint_state));

  std::vector<sensor_msgs::JointState> joint_states;
  // Mismatch between the number of joints and joint names
  EXPECT_FALSE(GetJointConfiguration(node, "error_states", joint_states));

  // Invalid joint_name
  EXPECT_FALSE(GetJointConfiguration(node, "error_joint_names", "initial_states", joint_states));

  // Invalid param_name
  EXPECT_FALSE(GetJointConfiguration(node, "joint_names", "error_take", joint_state));

  // Invalid node
  EXPECT_FALSE(GetJointConfiguration(error_node, "joint_names", "initial_states", joint_states));
}

TEST(JointConfigurationTest, OverwriteJointState) {
  // Normal case, overwritten
  sensor_msgs::JointState joint_state;
  joint_state.name.push_back("shoulder");
  joint_state.name.push_back("elbow");
  joint_state.position.push_back(1.0);
  joint_state.position.push_back(2.0);
  sensor_msgs::JointState overwriting_state;
  overwriting_state.name.push_back("elbow");
  overwriting_state.position.push_back(-1.0);
  sensor_msgs::JointState overwritten_state;
  EXPECT_TRUE(tmc_manipulation_util::OverwriteJointState(joint_state, overwriting_state, overwritten_state));
  ASSERT_EQ(overwritten_state.name.size(), 2);
  ASSERT_EQ(overwritten_state.position.size(), 2);
  EXPECT_EQ(joint_state.name[0], overwritten_state.name[0]);
  EXPECT_EQ(joint_state.name[1], overwritten_state.name[1]);
  EXPECT_DOUBLE_EQ(joint_state.position[0], overwritten_state.position[0]);
  EXPECT_DOUBLE_EQ(overwriting_state.position[0], overwritten_state.position[1]);

  // Normal case, not overwritten due to different name, ends
  overwritten_state.name[0].assign("wrist");
  EXPECT_TRUE(tmc_manipulation_util::OverwriteJointState(joint_state, overwritten_state, overwritten_state));
  EXPECT_EQ(joint_state.name[0], overwritten_state.name[0]);
  EXPECT_EQ(joint_state.name[1], overwritten_state.name[1]);
  EXPECT_DOUBLE_EQ(joint_state.position[0], overwritten_state.position[0]);
  EXPECT_DOUBLE_EQ(joint_state.position[1], overwritten_state.position[1]);

  // Abnormal case, invalid joint_state
  joint_state.position.resize(1);
  EXPECT_FALSE(tmc_manipulation_util::OverwriteJointState(joint_state, overwritten_state, overwritten_state));

  // Abnormal case, invalid overwritten_state
  joint_state.position.resize(2, 1.0);
  overwritten_state.name.push_back("wrist");
  EXPECT_FALSE(tmc_manipulation_util::OverwriteJointState(joint_state, overwritten_state, overwritten_state));
}

TEST(JointConfigurationTest, MergeJointState) {
  // Normal case, merged
  sensor_msgs::JointState joint_state;
  joint_state.name.push_back("shoulder");
  joint_state.name.push_back("elbow");
  joint_state.position.push_back(1.0);
  joint_state.position.push_back(2.0);
  sensor_msgs::JointState merge_state;
  merge_state.name.push_back("elbow");
  merge_state.name.push_back("wrist");
  merge_state.position.push_back(-1.0);
  merge_state.position.push_back(3.0);
  sensor_msgs::JointState merged_state;
  EXPECT_TRUE(tmc_manipulation_util::MergeJointState(joint_state, merge_state, merged_state));
  ASSERT_EQ(merged_state.name.size(), 3);
  ASSERT_EQ(merged_state.position.size(), 3);
  EXPECT_EQ(joint_state.name[0], merged_state.name[0]);
  EXPECT_EQ(joint_state.name[1], merged_state.name[1]);
  EXPECT_EQ(merge_state.name[1], merged_state.name[2]);
  EXPECT_DOUBLE_EQ(joint_state.position[0], merged_state.position[0]);
  EXPECT_DOUBLE_EQ(merge_state.position[0], merged_state.position[1]);
  EXPECT_DOUBLE_EQ(merge_state.position[1], merged_state.position[2]);

  // Abnormal case, invalid joint_state
  joint_state.position.resize(1);
  EXPECT_FALSE(tmc_manipulation_util::MergeJointState(joint_state, merge_state, merged_state));

  // Abnormal case, invalid merge_state
  joint_state.position.resize(2, 1.0);
  merge_state.name.push_back("hoge");
  EXPECT_FALSE(tmc_manipulation_util::MergeJointState(joint_state, merge_state, merged_state));
}

TEST(JointConfigurationTest, ExtractJointState) {
  // Normal case, can be retrieved
  sensor_msgs::JointState joint_state;
  joint_state.name.push_back("shoulder");
  joint_state.name.push_back("elbow");
  joint_state.name.push_back("wrist");
  joint_state.position.push_back(1.0);
  joint_state.position.push_back(2.0);
  joint_state.position.push_back(3.0);
  std::vector<std::string> joint_names;
  joint_names.push_back("shoulder");
  joint_names.push_back("hoge");
  joint_names.push_back("wrist");
  sensor_msgs::JointState extracted_state;
  EXPECT_TRUE(tmc_manipulation_util::ExtractJointState(joint_state, joint_names, extracted_state));
  ASSERT_EQ(2, extracted_state.name.size());
  ASSERT_EQ(2, extracted_state.position.size());
  EXPECT_EQ(joint_state.name[0], extracted_state.name[0]);
  EXPECT_EQ(joint_state.name[2], extracted_state.name[1]);
  EXPECT_DOUBLE_EQ(joint_state.position[0], extracted_state.position[0]);
  EXPECT_DOUBLE_EQ(joint_state.position[2], extracted_state.position[1]);

  joint_names.clear();
  joint_names.push_back("elbow");
  EXPECT_TRUE(tmc_manipulation_util::ExtractJointState(joint_state, joint_names, extracted_state));
  ASSERT_EQ(1, extracted_state.name.size());
  ASSERT_EQ(1, extracted_state.position.size());
  EXPECT_EQ(joint_state.name[1], extracted_state.name[0]);
  EXPECT_DOUBLE_EQ(joint_state.position[1], extracted_state.position[0]);

  // Abnormal case, invalid joint_state
  joint_state.position.resize(2);
  EXPECT_FALSE(tmc_manipulation_util::ExtractJointState(joint_state, joint_names, extracted_state));
}

class ExtractSingleJointStateTest : public ::testing::Test {
 public:
  ExtractSingleJointStateTest() {}
  virtual ~ExtractSingleJointStateTest() {}

 protected:
  virtual void SetUp();
  virtual void TearDown() {}

  sensor_msgs::JointState joint_state_;
};

void ExtractSingleJointStateTest::SetUp() {
  joint_state_.name.push_back("shoulder");
  joint_state_.name.push_back("elbow");
  joint_state_.name.push_back("wrist");
  joint_state_.position.push_back(1.0);
  joint_state_.position.push_back(2.0);
  joint_state_.position.push_back(3.0);
  joint_state_.velocity.push_back(4.0);
  joint_state_.velocity.push_back(5.0);
  joint_state_.velocity.push_back(6.0);
  joint_state_.effort.push_back(7.0);
  joint_state_.effort.push_back(8.0);
  joint_state_.effort.push_back(9.0);
}


TEST_F(ExtractSingleJointStateTest, ExtractSingleJointStateNormal) {
  std::string joint_name("elbow");
  sensor_msgs::JointState extracted_state;

  EXPECT_TRUE(tmc_manipulation_util::ExtractJointState(joint_state_, joint_name, extracted_state));

  ASSERT_EQ(extracted_state.name.size(), 1);
  ASSERT_EQ(extracted_state.position.size(), 1);
  ASSERT_EQ(extracted_state.velocity.size(), 1);
  ASSERT_EQ(extracted_state.effort.size(), 1);

  EXPECT_EQ(joint_state_.name[1], extracted_state.name[0]);
  EXPECT_DOUBLE_EQ(joint_state_.position[1], extracted_state.position[0]);
  EXPECT_DOUBLE_EQ(joint_state_.velocity[1], extracted_state.velocity[0]);
  EXPECT_DOUBLE_EQ(joint_state_.effort[1], extracted_state.effort[0]);
}


TEST_F(ExtractSingleJointStateTest, ExtractSingleJointStateNotFound) {
  std::string joint_name("finger");
  sensor_msgs::JointState extracted_state;

  EXPECT_FALSE(tmc_manipulation_util::ExtractJointState(joint_state_, joint_name, extracted_state));
}


TEST_F(ExtractSingleJointStateTest, ExtractSingleJointStateInvalidSize) {
  std::string joint_name("elbow");
  sensor_msgs::JointState extracted_state;

  joint_state_.position.resize(2);
  EXPECT_FALSE(tmc_manipulation_util::ExtractJointState(joint_state_, joint_name, extracted_state));
}


int main(int argc, char** argv) {
  ros::init(argc, argv, "tmc_manipulation_uitl_unit");
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
