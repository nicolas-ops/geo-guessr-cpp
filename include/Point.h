// Copyright (c) 2026 Nicolas Romanov. All Rights Reserved.

#pragma once

struct Point {
  Point(float longitude, float latitude) {
    lon = longitude;
    lat = latitude;
  }

  float lon;
  float lat;

  bool operator==(const Point &other) const {
    return lon == other.lon && lat == other.lat;
  }
};
