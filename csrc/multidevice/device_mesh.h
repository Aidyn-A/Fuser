// clang-format off
/*
 * SPDX-FileCopyrightText: Copyright (c) 2023-present NVIDIA CORPORATION & AFFILIATES.
 * All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause
 */
// clang-format on

#pragma once
#include <exceptions.h>
#include <multidevice/multidevice.h>
#include <visibility.h>

namespace nvfuser {

// The class DeviceMesh represents a set of (unique) devices on which a Pipeline
// Stage will be executed. For now, we only support flat meshes, but later we
// will add support for n-dimensional meshes.
class DeviceMesh final {
 public:
  DeviceMesh(std::vector<DeviceIdxType> devices = {}) {
    setDevices(devices);
  }

  DeviceMesh& operator=(const std::vector<DeviceIdxType>& devices) {
    setDevices(devices);
    return *this;
  }

  // Creates a device mesh of [0 .. num_devices-1]. I didn't make it a
  // constructor because single-element initializer lists would be directed to
  // use that instead of the constructor for vectors.
  static DeviceMesh createForNumDevices(int64_t num_devices);

  std::string toString() const;

  // returns a vector containing the device indices of the mesh
  const auto& vector() const {
    return vector_;
  }

  // returns whether a device is present in the mesh
  bool has(const DeviceIdxType device) const {
    return std::find(vector_.begin(), vector_.end(), device) != vector_.end();
  }

  bool operator==(const DeviceMesh& other) const {
    return vector_ == other.vector();
  }

 private:
  void setDevices(std::vector<DeviceIdxType> devices) {
    vector_ = devices;
    NVF_ERROR(
        std::unique(vector_.begin(), vector_.end()) == vector_.end(),
        "device mesh has duplicates");
  }

  // stores the list of device indices
  std::vector<DeviceIdxType> vector_;
};

NVF_API std::ostream& operator<<(std::ostream& out, const DeviceMesh& mesh);

} // namespace nvfuser
