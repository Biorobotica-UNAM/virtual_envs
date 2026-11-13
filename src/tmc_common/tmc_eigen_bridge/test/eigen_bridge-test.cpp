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
/// @file     test_eigen_bridge.cpp
/// @brief    Test of various type conversions. It may not be necessary to test...
/// @version  1.0.0
/// @author   Kiyohiro Sogen
/// @date     2012.01.16
/// @note     Applied for P-Robot coding rule(Ver:x.xx)
#include <gtest/gtest.h>
#include "tmc_eigen_bridge/eigen_bridge.hpp"

namespace {
const double kThreshEqual = 0.00001;
}

TEST(PointMsgToVector3d, normal) {
  geometry_msgs::msg::Point input;
  input.x = 0.7;
  input.y = 0.8;
  input.z = 0.9;
  Eigen::Vector3d output;
  tmc_eigen_bridge::PointMsgToVector3d(input, output);
  EXPECT_FLOAT_EQ(0.7, output[0]);
  EXPECT_FLOAT_EQ(0.8, output[1]);
  EXPECT_FLOAT_EQ(0.9, output[2]);
}

TEST(Vector3dToPointMsg, normal) {
  Eigen::Vector3d input;
  input[0] = 0.7;
  input[1] = 0.8;
  input[2] = 0.9;
  geometry_msgs::msg::Point output;
  tmc_eigen_bridge::Vector3dToPointMsg(input, output);
  EXPECT_FLOAT_EQ(0.7, output.x);
  EXPECT_FLOAT_EQ(0.8, output.y);
  EXPECT_FLOAT_EQ(0.9, output.z);
}

TEST(Vector3MsgToVector3d, normal) {
  geometry_msgs::msg::Vector3 input;
  input.x = 0.7;
  input.y = 0.8;
  input.z = 0.9;
  Eigen::Vector3d output;
  tmc_eigen_bridge::Vector3MsgToVector3d(input, output);
  EXPECT_FLOAT_EQ(0.7, output[0]);
  EXPECT_FLOAT_EQ(0.8, output[1]);
  EXPECT_FLOAT_EQ(0.9, output[2]);
}

TEST(Vector3dToVector3Msg, normal) {
  Eigen::Vector3d input;
  input[0] = 0.7;
  input[1] = 0.8;
  input[2] = 0.9;
  geometry_msgs::msg::Vector3 output;
  tmc_eigen_bridge::Vector3dToVector3Msg(input, output);
  EXPECT_FLOAT_EQ(0.7, output.x);
  EXPECT_FLOAT_EQ(0.8, output.y);
  EXPECT_FLOAT_EQ(0.9, output.z);
}

TEST(Affine3dToPoseMsg, normal) {
  Eigen::Affine3d input;
  input = Eigen::Translation3d(4.0, 5.0, 6.0) * Eigen::AngleAxisd(M_PI / 4.0, Eigen::Vector3d::UnitX()) *
          Eigen::AngleAxisd(M_PI / 3.0, Eigen::Vector3d::UnitY()) *
          Eigen::AngleAxisd(M_PI / 2.0, Eigen::Vector3d::UnitZ());
  geometry_msgs::msg::Pose output;
  tmc_eigen_bridge::Affine3dToPoseMsg(input, output);
  EXPECT_FLOAT_EQ(4.0, output.position.x);
  EXPECT_FLOAT_EQ(5.0, output.position.y);
  EXPECT_FLOAT_EQ(6.0, output.position.z);
  EXPECT_FLOAT_EQ(0.56098551, output.orientation.x);
  EXPECT_FLOAT_EQ(0.092295952, output.orientation.y);
  EXPECT_FLOAT_EQ(0.70105737, output.orientation.z);
  EXPECT_FLOAT_EQ(0.43045932, output.orientation.w);
}

TEST(Affine3dToTransformMsg, normal) {
  Eigen::Affine3d input;
  input = Eigen::Translation3d(4.0, 5.0, 6.0) * Eigen::AngleAxisd(M_PI / 4.0, Eigen::Vector3d::UnitX()) *
          Eigen::AngleAxisd(M_PI / 3.0, Eigen::Vector3d::UnitY()) *
          Eigen::AngleAxisd(M_PI / 2.0, Eigen::Vector3d::UnitZ());
  geometry_msgs::msg::Transform output;
  tmc_eigen_bridge::Affine3dToTransformMsg(input, output);
  EXPECT_FLOAT_EQ(4.0, output.translation.x);
  EXPECT_FLOAT_EQ(5.0, output.translation.y);
  EXPECT_FLOAT_EQ(6.0, output.translation.z);
  EXPECT_FLOAT_EQ(0.56098551, output.rotation.x);
  EXPECT_FLOAT_EQ(0.092295952, output.rotation.y);
  EXPECT_FLOAT_EQ(0.70105737, output.rotation.z);
  EXPECT_FLOAT_EQ(0.43045932, output.rotation.w);
}

TEST(PoseMsgToAffine3d, normal) {
  geometry_msgs::msg::Pose input;
  input.position.x = 6.0;
  input.position.y = 5.0;
  input.position.z = 4.0;
  input.orientation.x = 0.560986;
  input.orientation.y = 0.092296;
  input.orientation.z = 0.701057;
  input.orientation.w = 0.430459;

  Eigen::Affine3d output;
  tmc_eigen_bridge::PoseMsgToAffine3d(input, output);
  EXPECT_FLOAT_EQ(6.0, output.translation()[0]);
  EXPECT_FLOAT_EQ(5.0, output.translation()[1]);
  EXPECT_FLOAT_EQ(4.0, output.translation()[2]);
  Eigen::Matrix3d ref;
  ref = Eigen::Quaterniond(0.430459, 0.560986, 0.092296, 0.701057);
  EXPECT_FLOAT_EQ(ref(0, 0), output.linear()(0, 0));
  EXPECT_FLOAT_EQ(ref(0, 1), output.linear()(0, 1));
  EXPECT_FLOAT_EQ(ref(0, 2), output.linear()(0, 2));
  EXPECT_FLOAT_EQ(ref(1, 0), output.linear()(1, 0));
  EXPECT_FLOAT_EQ(ref(1, 1), output.linear()(1, 1));
  EXPECT_FLOAT_EQ(ref(1, 2), output.linear()(1, 2));
  EXPECT_FLOAT_EQ(ref(2, 0), output.linear()(2, 0));
  EXPECT_FLOAT_EQ(ref(2, 1), output.linear()(2, 1));
  EXPECT_FLOAT_EQ(ref(2, 2), output.linear()(2, 2));
}

TEST(Vector4fToVector3d, normal) {
  Eigen::Vector4f eigen_vec4;
  Eigen::Vector3d point;
  eigen_vec4[0] = 1.0;
  eigen_vec4[1] = 2.0;
  eigen_vec4[2] = 3.0;
  eigen_vec4[3] = 4.0;

  tmc_eigen_bridge::Vector4fToVector3d(eigen_vec4, point);
  EXPECT_FLOAT_EQ(1.0, point[0]);
  EXPECT_FLOAT_EQ(2.0, point[1]);
  EXPECT_FLOAT_EQ(3.0, point[2]);
}

TEST(Vector3dToVector4f, normal) {
  Eigen::Vector4f eigen_vec4;
  Eigen::Vector3d point;

  point[0] = 3.0;
  point[1] = 2.0;
  point[2] = 1.0;

  tmc_eigen_bridge::Vector3dToVector4f(point, eigen_vec4);
  EXPECT_FLOAT_EQ(3.0, eigen_vec4[0]);
  EXPECT_FLOAT_EQ(2.0, eigen_vec4[1]);
  EXPECT_FLOAT_EQ(1.0, eigen_vec4[2]);
  EXPECT_FLOAT_EQ(0.0, eigen_vec4[3]);
}

TEST(Affine3dToTF, normal) {
  Eigen::Affine3d input;
  tf2::Transform output;

  input.linear() << 1, 0, 0, 0, 1, 0, 0, 0, 1;
  input.translation() << 1, 2, 3;
  tmc_eigen_bridge::Affine3dToTF(input, output);
  EXPECT_FLOAT_EQ(1.0, output.getOrigin()[0]);
  EXPECT_FLOAT_EQ(2.0, output.getOrigin()[1]);
  EXPECT_FLOAT_EQ(3.0, output.getOrigin()[2]);

  EXPECT_FLOAT_EQ(1.0, output.getBasis()[0][0]);
  EXPECT_FLOAT_EQ(0.0, output.getBasis()[0][1]);
  EXPECT_FLOAT_EQ(0.0, output.getBasis()[0][2]);
  EXPECT_FLOAT_EQ(0.0, output.getBasis()[1][0]);
  EXPECT_FLOAT_EQ(1.0, output.getBasis()[1][1]);
  EXPECT_FLOAT_EQ(0.0, output.getBasis()[1][2]);
  EXPECT_FLOAT_EQ(0.0, output.getBasis()[2][0]);
  EXPECT_FLOAT_EQ(0.0, output.getBasis()[2][1]);
  EXPECT_FLOAT_EQ(1.0, output.getBasis()[2][2]);
}

TEST(TFToAffine3d, normal) {
  tf2::Transform input;
  Eigen::Affine3d output;

  input.setOrigin(tf2::Vector3(1, 2, 3));
  input.setBasis(tf2::Matrix3x3(1, 0, 0, 0, 1, 0, 0, 0, 1));
  tmc_eigen_bridge::TFToAffine3d(input, output);

  EXPECT_FLOAT_EQ(1.0, output.translation().x());
  EXPECT_FLOAT_EQ(2.0, output.translation().y());
  EXPECT_FLOAT_EQ(3.0, output.translation().z());

  EXPECT_FLOAT_EQ(1.0, output.linear()(0, 0));
  EXPECT_FLOAT_EQ(0.0, output.linear()(0, 1));
  EXPECT_FLOAT_EQ(0.0, output.linear()(0, 2));
  EXPECT_FLOAT_EQ(0.0, output.linear()(1, 0));
  EXPECT_FLOAT_EQ(1.0, output.linear()(1, 1));
  EXPECT_FLOAT_EQ(0.0, output.linear()(1, 2));
  EXPECT_FLOAT_EQ(0.0, output.linear()(2, 0));
  EXPECT_FLOAT_EQ(0.0, output.linear()(2, 1));
  EXPECT_FLOAT_EQ(1.0, output.linear()(2, 2));
}


int main(int argc, char** argv) {
  //  hsp::Init(argc, argv, "test_eigen_bridge");
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
