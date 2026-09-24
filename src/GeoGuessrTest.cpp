// Copyright (c) 2026 Nicolas Romanov. All Rights Reserved.

#include "GeoGuessr.h"
#include "MockTerminalManager.h"
#include "gtest/gtest.h"
#include <string>

MockTerminalManager tm(100, 100);
GeoGuessr geoGuessr(tm, "./testpolygons.tsv");

// Check if the method properly removes elements from the vector
// if they have already been chosen.
TEST(GeoGuessrTest, getRandomPolygonName) {
  ASSERT_EQ(geoGuessr.availablePolygonNames_.size(), 9u);

  std::string result = geoGuessr.getRandomPolygonName();

  // check if size has properly decreased
  ASSERT_EQ(geoGuessr.availablePolygonNames_.size(), 8u);

  // Search for chosen element to ensure its gone
  for (std::string name : geoGuessr.availablePolygonNames_) {
    ASSERT_FALSE(name == result);
  }

  // Remove remaining elements and check if elements get removen properly
  for (int i = 0; i < 8; i++) {
    geoGuessr.getRandomPolygonName();
    ASSERT_EQ(geoGuessr.availablePolygonNames_.size(), 8 - (i + 1));
  }
}
