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

/// @file     eigen_bridge.cpp
/// @brief    Various type conversions
/// @version  1.0.0
/// @author   Kiyohiro Sogen
/// @date     2012.01.10
/// @note     Applied for P-Robot coding rule(Ver:x.xx)
#include "tmc_eigen_bridge/eigen_bridge.hpp"
namespace tmc_eigen_bridge {

/// Convert from geometry_msgs::msg::Point to Eigen::Vector3d
void PointMsgToVector3d(const geometry_msgs::msg::Point& input, Eigen::Vector3d& output) {
  output[0] = input.x;
  output[1] = input.y;
  output[2] = input.z;
}

/// Convert from Eigen::Vector3d to geometry_msgs::msg::Point
void Vector3dToPointMsg(const Eigen::Vector3d& input, geometry_msgs::msg::Point& output) {
  output.x = input[0];
  output.y = input[1];
  output.z = input[2];
}

/// Convert from geometry_msgs::msg::Vector3 to Eigen::Vector3d
void Vector3MsgToVector3d(const geometry_msgs::msg::Vector3& input, Eigen::Vector3d& output) {
  output[0] = input.x;
  output[1] = input.y;
  output[2] = input.z;
}

/// Convert from Eigen::Vector3d to geometry_msgs::msg::Vector3
void Vector3dToVector3Msg(const Eigen::Vector3d& input, geometry_msgs::msg::Vector3& output) {
  output.x = input[0];
  output.y = input[1];
  output.z = input[2];
}

/// Convert from Eigen::Affine3d to geometry_msgs::msg::Pose.
void Affine3dToPoseMsg(const Eigen::Affine3d& input, geometry_msgs::msg::Pose& output) {
  output.position.x = input.translation()[0];
  output.position.y = input.translation()[1];
  output.position.z = input.translation()[2];
  Eigen::Quaterniond q = (Eigen::Quaterniond)input.linear();
  output.orientation.x = q.x();
  output.orientation.y = q.y();
  output.orientation.z = q.z();
  output.orientation.w = q.w();
  if (output.orientation.w < 0) {
    output.orientation.x *= -1.0;
    output.orientation.y *= -1.0;
    output.orientation.z *= -1.0;
    output.orientation.w *= -1.0;
  }
}

/// Convert from Eigen::Affine3d to geometry_msgs::msg::Transform.
void Affine3dToTransformMsg(const Eigen::Affine3d& input, geometry_msgs::msg::Transform& output) {
  output.translation.x = input.translation()[0];
  output.translation.y = input.translation()[1];
  output.translation.z = input.translation()[2];
  Eigen::Quaterniond q = (Eigen::Quaterniond)input.linear();
  output.rotation.x = q.x();
  output.rotation.y = q.y();
  output.rotation.z = q.z();
  output.rotation.w = q.w();
  if (output.rotation.w < 0) {
    output.rotation.x *= -1.0;
    output.rotation.y *= -1.0;
    output.rotation.z *= -1.0;
    output.rotation.w *= -1.0;
  }
}

/// Convert from geometry_msgs::msg::Pose to Eigen::Affine3d.
void PoseMsgToAffine3d(const geometry_msgs::msg::Pose& input, Eigen::Affine3d& output) {
  output =
      Eigen::Translation3d(input.position.x, input.position.y, input.position.z) *
      Eigen::Quaternion<double>(input.orientation.w, input.orientation.x, input.orientation.y, input.orientation.z);
}

/// Convert from Eigen::Vector4f to Eigen::Vector3d
void Vector4fToVector3d(const Eigen::Vector4f& input, Eigen::Vector3d& output) {
  output[0] = static_cast<double>(input[0]);
  output[1] = static_cast<double>(input[1]);
  output[2] = static_cast<double>(input[2]);
}

/// Convert from Eigen::Vector3d to Eigen::Vector4f
void Vector3dToVector4f(const Eigen::Vector3d& input, Eigen::Vector4f& output) {
  output[0] = static_cast<float>(input[0]);
  output[1] = static_cast<float>(input[1]);
  output[2] = static_cast<float>(input[2]);
  output[3] = 0.0;
}

/// Convert from tf2::transform to Eigen::Affine3d.
void TFToAffine3d(const tf2::Transform& input, Eigen::Affine3d& output) {
  for (int32_t i = 0; i < 3; ++i) {
    output.matrix()(i, 3) = input.getOrigin()[i];
    for (int32_t j = 0; j < 3; ++j) {
      output.matrix()(i, j) = input.getBasis()[i][j];
    }
  }
  // Fill in identity in last row
  for (int col = 0; col < 3; col++) output.matrix()(3, col) = 0;
  output.matrix()(3, 3) = 1;
}

/// @brief Convert from Eigen::Affine3d to tf2::transform.
void Affine3dToTF(const Eigen::Affine3d& input, tf2::Transform& output) {
  output.setOrigin(tf2::Vector3(input.matrix()(0, 3), input.matrix()(1, 3), input.matrix()(2, 3)));
  output.setBasis(tf2::Matrix3x3(input.matrix()(0, 0), input.matrix()(0, 1), input.matrix()(0, 2), input.matrix()(1, 0),
                                input.matrix()(1, 1), input.matrix()(1, 2), input.matrix()(2, 0), input.matrix()(2, 1),
                                input.matrix()(2, 2)));
}


}  // namespace tmc_eigen_bridge
