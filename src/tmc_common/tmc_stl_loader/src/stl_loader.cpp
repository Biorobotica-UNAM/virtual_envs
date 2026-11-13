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
/// @file     stl_loader.cpp
/// @brief    Read STL file
/// @version  0.6.0
/// @author   Takao Yasuda
/// @note     Applied for Partner-Robot Coding Rule(Ver:x.xx)
#include "tmc_stl_loader/stl_loader.hpp"
#include <cassert>
#include <stdint.h>
#include <fstream>
#include <string>
#include <vector>
#include <ament_index_cpp/get_package_share_directory.hpp>

namespace {
/// Number of bytes in STL file header
uint32_t const kHeaderByte = 80;
/// Number of bytes for one triangle in STL file
uint32_t const kTriangleByte = 50;
/// Number of bytes for float in STL file
uint32_t const kFloatSize = 4;
/// Number of dimensions (3D)
uint32_t const kTri = 3;
/// Not detected
uint32_t const kNonExist = -1;
/// File mode (read)
const char* const kFineModeRead = "r";
}  // end namespace
namespace tmc_stl_loader {
/// @brief Search for the target vector from a group of 3D vectors
/// @param[in] vertices Group of 3D vectors
/// @param[in] vector3 3D vector
/// @param[out] vector_id Vector ID (-1 == NonExist: non-existent address)
void GetVertexIndex(const std::vector<Eigen::Vector3f> vertices, const Eigen::Vector3f& vector3, uint32_t& vector_id) {
  vector_id = kNonExist;
  // Obtain the corresponding vector ID from 3D data
  for (uint32_t i = 0; i < vertices.size(); ++i) {
    if (vertices[i] == vector3) {
      vector_id = i;
    }
  }
}
/// @param[in] file_name Address (file name)
/// @param[out] mesh_out Mesh data
/// @note Binary format has no remainder when (file size-84)/50, see below
/// @note http://ja.wikipedia.org/wiki/Standard_Triangulated_Language
void STLLoader::Load(const std::string& file_name, Mesh& mesh_out) {
  mesh_out = Mesh();

  // If the file is blank or in a different format, do not process and return empty
  std::string extension;
  extension.clear();
  for (uint32_t i = file_name.find_last_of(".") + 1; i < file_name.size(); ++i) {
    extension.push_back(file_name.at(i));
  }
  if ((file_name == "") || ((extension != "stl") && (extension != "STL"))) {
    return;
  }

  std::string file_name_impl = file_name;
  if (file_name.find("package://") == 0) {
    file_name_impl.erase(0, strlen("package://"));

    const auto pos = file_name_impl.find("/");
    if (pos == std::string::npos) {
      return;
    }

    std::string package = file_name_impl.substr(0, pos);
    if (package.empty()) {
      return;
    }
    file_name_impl.erase(0, pos);

    std::string package_path;
    try {
      package_path = ament_index_cpp::get_package_share_directory(package);
    } catch (...) {
      return;
    }
    file_name_impl = package_path + file_name_impl;
  }

  // If the file size is 0, do not process and return empty
  std::ifstream ifs(file_name_impl.c_str(), std::ios::binary);
  ifs.peek();
  if (ifs.rdbuf()->in_avail() == 0) {
    return;
  }
  // If not in binary format, do not process and return empty
  if ((static_cast<int>(ifs.seekg(0, std::ios::end).tellg()) - 84) % 50 != 0) {
    return;
  }
  // Open the STL file in read-only mode
  FILE* file = fopen(file_name_impl.c_str(), kFineModeRead);
  if (file == NULL) {
    // If the file does not exist, do not process and return empty
    fclose(file);
    return;
  }
  // Read the STL file and obtain mesh data
  ReadBinary_(file, mesh_out);
  // Termination process
  fclose(file);
  file = NULL;
}
/// @param[in] data Pointer to the start of the byte sequence to read
/// @param[out] byte_out Data read as 4 bytes (float type)
void STLLoader::ReadFloat_(uint8_t* data, float& byte_out) {
  union {
    float yfloat;
    char ychar[4];
  } y;
  for (uint32_t i = 0; i < 4; ++i) {
    y.ychar[i] = data[i];
  }
  byte_out = y.yfloat;
}
/// @param[in] file File
/// @param[out] byte_out Data read as 4 bytes (float type)
void STLLoader::ReadFloat_(FILE* file, float& byte_out) {
  byte_out = 0.0;
  if (fread(&byte_out, sizeof(byte_out), 1, file) == 0) {
    assert(!"Error in STLLoader::ReadFloat");
  }
}
/// @param[in] data Pointer to the start of the byte sequence to read
/// @param[out] byte_out Data read as 4 bytes (uint32 type)
void STLLoader::ReadLongInt_(uint8_t* data, uint32_t& byte_out) {
  union {
    uint32_t yint;
    char ychar[4];
  } y;
  for (uint32_t i = 0; i < 4; ++i) {
    y.ychar[i] = data[i];
  }
  byte_out = y.yint;
}
/// @param[in] file File
/// @param[out] byte_out Data read as 4 bytes (uint32 type)
void STLLoader::ReadLongInt_(FILE* file, uint32_t& byte_out) {
  union {
    uint32_t yint;
    char ychar[4];
  } y;
  y.ychar[0] = fgetc(file);
  y.ychar[1] = fgetc(file);
  y.ychar[2] = fgetc(file);
  y.ychar[3] = fgetc(file);
  byte_out = y.yint;
}
/// @param[in] file File
/// @param[out] mesh Mesh data
void STLLoader::ReadBinary_(FILE* file, tmc_stl_loader::Mesh& mesh) {
  mesh = Mesh();
  // Skip header bytes
  for (uint32_t i = 0; i < kHeaderByte; ++i) {
    if (fgetc(file) == EOF) {
      assert(!"Not Found STL Data in File.");
    }
  }
  uint32_t face_num = 0.0;
  ReadLongInt_(file, face_num);
  // Read each triangle, refer to the URL below for the format.
  // http://ja.wikipedia.org/wiki/Standard_Triangulated_Language
  for (uint32_t iface = 0; iface < face_num; ++iface) {
    float nx = 0.0;
    float ny = 0.0;
    float nz = 0.0;
    ReadFloat_(file, nx);
    ReadFloat_(file, ny);
    ReadFloat_(file, nz);
    Eigen::Vector3f normal(nx, ny, nz);
    mesh.normals.push_back(normal);
    for (uint32_t i = 0; i < kTri; ++i) {
      float vx = 0.0;
      float vy = 0.0;
      float vz = 0.0;
      ReadFloat_(file, vx);
      ReadFloat_(file, vy);
      ReadFloat_(file, vz);
      Eigen::Vector3f vertex(vx, vy, vz);
      uint32_t index = kNonExist;
      GetVertexIndex(mesh.vertices, vertex, index);
      // Add if not existing
      if (index == kNonExist) {
        mesh.vertices.push_back(vertex);
        index = mesh.vertices.size() - 1;
      }
      mesh.indices.push_back(index);
    }
    // Return the last 2 bytes
    fgetc(file);
    fgetc(file);
  }
}
}  // end namespace tmc_stl_loader
