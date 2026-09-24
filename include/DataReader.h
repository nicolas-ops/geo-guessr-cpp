// Copyright (c) 2026 Nicolas Romanov. All Rights Reserved.

#pragma once

#include "Polygon.h"
#include <cmath>
#include <string>
#include <unordered_map>
#include <vector>

// This class is responsible for reading the data of a TSV file and returning a
// nice usable field with all the data inside.
class DataReader {
public:
  // Takes the data from the given file and sets the member variables to the
  // data
  DataReader(std::string filename);

  // Returns a field that contains all the names of each Polygon
  std::vector<std::string> getPolygonNames() const { return polygonNames_; };

  // Returns the polygon map
  std::unordered_map<std::string, std::vector<Polygon>> getPolygonMap() const {
    return polygonMap_;
  };

  float getLargestLongitude() const { return largestLon_; };
  float getSmallestLongitude() const { return smallestLon_; };
  float getLargestLatitude() const { return largestLat_; };
  float getSmallestLatitude() const { return smallestLat_; };

private:
  // a field that contains all the naes of each Polygon
  std::vector<std::string> polygonNames_;

  // A map which has the polygon name as a key and a field of a ring of Polygons
  std::unordered_map<std::string, std::vector<Polygon>> polygonMap_;

  float largestLon_ = -INFINITY;
  float smallestLon_ = INFINITY;
  float largestLat_ = -INFINITY;
  float smallestLat_ = INFINITY;
};
