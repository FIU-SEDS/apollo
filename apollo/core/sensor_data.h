#pragma once

struct SensorData {
  float altitude = 0.0f;
  float velocity = 0.0f;
  float accel_x = 0.0f;
  float accel_y = 0.0f;
  float accel_z = 0.0f;
  bool gps_lock = false;
  bool radio_lock = false;
  float battery = 0.0f;
};
