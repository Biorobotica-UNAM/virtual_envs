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
/// @file     eigen_utils.cpp
/// @brief    Utility functions for using eigen
/// @author   Koji Terada
/// @version  1.0.0
/// @date     2012.2.23
/// @note     [1.0.0] 2012.2.23 Newly created
///           For now, roll-pitch-yaw angles

#include <tmc_eigen_utils/eigen_utils.hpp>

namespace {
const double kThresholdSingularRPY = 0.99999999;
}

namespace tmc_eigen_utils {


/// Convert RPY representation to Quaternion
/// @param[in] rpy 3D vector representing RPY [rad]
/// @retval Quaternion representing rotation
Eigen::Quaterniond RPYToQuaternion(const Eigen::Vector3d& rpy) {
  double cos_r2 = cos(rpy(0) * 0.5);
  double sin_r2 = sin(rpy(0) * 0.5);
  double cos_p2 = cos(rpy(1) * 0.5);
  double sin_p2 = sin(rpy(1) * 0.5);
  double cos_y2 = cos(rpy(2) * 0.5);
  double sin_y2 = sin(rpy(2) * 0.5);

  return Eigen::Quaterniond(cos_y2 * cos_p2 * cos_r2 + sin_y2 * sin_p2 * sin_r2,   // w
                            cos_y2 * cos_p2 * sin_r2 - sin_y2 * sin_p2 * cos_r2,   // x
                            cos_y2 * sin_p2 * cos_r2 + sin_y2 * cos_p2 * sin_r2,   // y
                            sin_y2 * cos_p2 * cos_r2 - cos_y2 * sin_p2 * sin_r2);  // z
}

/// Convert RPY representation to Quaternion
/// @param[in] rpy 3D vector representing RPY [rad]
/// @retval Quaternion representing rotation
Eigen::Quaternionf RPYToQuaternion(const Eigen::Vector3f& rpy) {
  float cos_r2 = cos(rpy(0) * 0.5);
  float sin_r2 = sin(rpy(0) * 0.5);
  float cos_p2 = cos(rpy(1) * 0.5);
  float sin_p2 = sin(rpy(1) * 0.5);
  float cos_y2 = cos(rpy(2) * 0.5);
  float sin_y2 = sin(rpy(2) * 0.5);

  return Eigen::Quaternionf(cos_y2 * cos_p2 * cos_r2 + sin_y2 * sin_p2 * sin_r2,   // w
                            cos_y2 * cos_p2 * sin_r2 - sin_y2 * sin_p2 * cos_r2,   // x
                            cos_y2 * sin_p2 * cos_r2 + sin_y2 * cos_p2 * sin_r2,   // y
                            sin_y2 * cos_p2 * cos_r2 - cos_y2 * sin_p2 * sin_r2);  // z
}

/// Convert Quaternion representation to RPY representation
Eigen::Vector3d QuaternionToRPY(const Eigen::Quaterniond& quaternion) {
  double roll(0.0);
  double pitch(0.0);
  double yaw(0.0);
  // Copy and normalize
  Eigen::Quaterniond q(quaternion);
  q.normalize();
  // Calculate sin_p = 2(wy-xz)
  double sin_p = 2.0 * (q.w() * q.y() - q.x() * q.z());

  // Check for gimbal lock
  if ((sin_p > kThresholdSingularRPY) || (sin_p < -kThresholdSingularRPY)) {
    // In case of gimbal lock

    // Fix roll to 0
    roll = 0.0;

    // Represent orientation with yaw
    yaw = atan2(q.w() * q.z() - q.x() * q.y(), 0.5 - q.x() * q.x() - q.z() * q.z());

    // Calculate pitch angle
    if (sin_p < -1.0) {
      pitch = -M_PI * 0.5;
    } else if (sin_p > 1.0) {
      pitch = M_PI * 0.5;
    } else {
      pitch = 0.5 * M_PI * sin_p;
    }
  } else {
    // If not in gimbal lock state
    roll = atan2(q.w() * q.x() + q.y() * q.z(), 0.5 - q.x() * q.x() - q.y() * q.y());
    pitch = asin(sin_p);
    yaw = atan2(q.x() * q.y() + q.w() * q.z(), 0.5 - q.y() * q.y() - q.z() * q.z());
  }

  return Eigen::Vector3d(roll, pitch, yaw);
}

/// Convert Quaternion representation to RPY representation
Eigen::Vector3f QuaternionToRPY(const Eigen::Quaternionf& quaternion) {
  float roll(0.0);
  float pitch(0.0);
  float yaw(0.0);
  // Copy and normalize
  Eigen::Quaternionf q(quaternion);
  q.normalize();
  // Calculate sin_p = 2(wy-xz)
  float sin_p = 2.0 * (q.w() * q.y() - q.x() * q.z());

  // Check for gimbal lock
  if ((sin_p > kThresholdSingularRPY) || (sin_p < -kThresholdSingularRPY)) {
    // In case of gimbal lock

    // Fix roll to 0
    roll = 0.0;

    // Represent orientation with yaw
    yaw = atan2(q.w() * q.z() - q.x() * q.y(), 0.5 - q.x() * q.x() - q.z() * q.z());

    // Calculate pitch angle
    if (sin_p < -1.0) {
      pitch = -M_PI * 0.5;
    } else if (sin_p > 1.0) {
      pitch = M_PI * 0.5;
    } else {
      pitch = 0.5 * M_PI * sin_p;
    }
  } else {
    /* If not in gimbal lock state */
    roll = atan2(q.w() * q.x() + q.y() * q.z(), 0.5 - q.x() * q.x() - q.y() * q.y());
    pitch = asin(sin_p);
    yaw = atan2(q.x() * q.y() + q.w() * q.z(), 0.5 - q.y() * q.y() - q.z() * q.z());
  }

  return Eigen::Vector3f(roll, pitch, yaw);
}
}  // namespace tmc_eigen_utils
