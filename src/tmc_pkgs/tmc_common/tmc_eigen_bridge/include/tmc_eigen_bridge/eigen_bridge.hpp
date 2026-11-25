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
/// @file     eigen_bridge.hpp
/// @brief    Various type conversion headers
/// @version  1.0.0
/// @author   Kiyohiro Sogen
/// @date     2012.01.10
/// @note     Applied for P-Robot coding rule(Ver:x.xx)
#ifndef TMC_EIGEN_BRIDGE_EIGEN_BRIDGE_HPP_
#define TMC_EIGEN_BRIDGE_EIGEN_BRIDGE_HPP_
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <geometry_msgs/msg/point_stamped.hpp>

#include <geometry_msgs/msg/pose.hpp>
#include <geometry_msgs/msg/transform.hpp>
#include <geometry_msgs/msg/vector3.hpp>
#include <tf2/LinearMath/Transform.h>

namespace tmc_eigen_bridge {

/// @brief Convert from geometry_msgs::msg::Point to Eigen::Vector3d
/// @param[in] input geometry_msgs::msg::Point
/// @param[out] output Eigen::Vector3d
void PointMsgToVector3d(const geometry_msgs::msg::Point& input, Eigen::Vector3d& output);

/// @brief Convert from Eigen::Vector3d to geometry_msgs::msg::Point
/// @param[in] input Eigen::Vector3d
/// @param[out] output geometry_msgs::msg::Point
void Vector3dToPointMsg(const Eigen::Vector3d& input, geometry_msgs::msg::Point& output);

/// @brief Convert from geometry_msgs::msg::Vector3 to Eigen::Vector3d
/// @param[in] input geometry_msgs::msg::Vector3
/// @param[out] output Eigen::Vector3d
void Vector3MsgToVector3d(const geometry_msgs::msg::Vector3& input, Eigen::Vector3d& output);

/// @brief Convert from Eigen::Vector3d to geometry_msgs::msg::Vector3
/// @param[in] input Eigen::Vector3d
/// @param[out] output geometry_msgs::msg::Vector3
void Vector3dToVector3Msg(const Eigen::Vector3d& input, geometry_msgs::msg::Vector3& output);

/// @brief Convert from Affine3d to geometry_msgs::msg::Pose.
/// @param[in] input Eigen::Affine3d
/// @param[out] output geometry_msgs::msg::Pose
void Affine3dToPoseMsg(const Eigen::Affine3d& input, geometry_msgs::msg::Pose& output);

/// @brief Convert from Affine3d to geometry_msgs::msg::Transform.
/// @param[in] input Eigen::Affine3d
/// @param[out] output geometry_msgs::msg::Transform
void Affine3dToTransformMsg(const Eigen::Affine3d& input, geometry_msgs::msg::Transform& output);

/// @brief Convert from geometry_msgs::msg::Pose to Eigen::Affine3d.
/// @param[in] input geometry_msgs::msg::Pose
/// @param[out] output Eigen::Affine3d
void PoseMsgToAffine3d(const geometry_msgs::msg::Pose& input, Eigen::Affine3d& output);

/// @brief Convert from Eigen::Vector4f to Eigen::Vector3d
/// @param[in] input Eigen::Vector4f
/// @param[out] output Eigen::Vector3d
void Vector4fToVector3d(const Eigen::Vector4f& input, Eigen::Vector3d& output);

/// @brief Convert from Eigen::Vector3d to Eigen::Vector4f
/// @param[in] input Eigen::Vector3d
/// @param[out] output Eigen::Vector4f
void Vector3dToVector4f(const Eigen::Vector3d& input, Eigen::Vector4f& output);

/// @brief Convert from tf2::transform to Eigen::Affine3d.
/// @param[in] input tf2::transform
/// @param[out] output Eigen::Affine3d
void TFToAffine3d(const tf2::Transform& input, Eigen::Affine3d& output);

/// @brief Convert from Eigen::Affine3d to tf2::transform.
/// @param[in] input Eigen::Affine3d
/// @param[out] output tf2::transform
void Affine3dToTF(const Eigen::Affine3d& input, tf2::Transform& output);

}  // namespace tmc_eigen_bridge

#endif  // TMC_EIGEN_BRIDGE_EIGEN_BRIDGE_HPP_
