// Copyright (c) 2026 Nicolas Romanov. All Rights Reserved.

#include "DataReader.h"
#include "gtest/gtest.h"

DataReader reader("./testpolygons.tsv");

// Test if the numbers of polygons, rings and points are correct and if the
// values match the TSV file
TEST(ReaderTest, polygonfield) {
  auto map = reader.getPolygonMap();

  auto polygonfield = map["Test1"];
  ASSERT_EQ(polygonfield.size(), 1u);

  auto polygon = polygonfield[0];
  ASSERT_EQ(polygon.rings_.size(), 1u);

  auto ring = polygon.rings_[0];
  for (size_t i = 0; i < ring.size(); i++) {
    auto point = ring[i];
    ASSERT_EQ(point.lon, i + 1);
    ASSERT_EQ(point.lat, i + 1);
  }
}

// Test if it everything from Test 1 works with an additional ring and different
// values
TEST(ReaderTest, Test2) {
  auto map = reader.getPolygonMap();

  auto polygonfield = map["Test2"];
  ASSERT_EQ(polygonfield.size(), 1u);

  auto polygon = polygonfield[0];
  ASSERT_EQ(polygon.rings_.size(), 2u);

  auto ring1 = polygon.rings_[0];
  for (size_t i = 0; i < ring1.size(); i++) {
    auto point = ring1[i];
    ASSERT_EQ(point.lon, i + 1);
    ASSERT_EQ(point.lat, i + 1);
  }

  auto ring2 = polygon.rings_[1];
  for (size_t i = 0; i < ring2.size(); i++) {
    auto point = ring2[i];
    ASSERT_EQ(point.lon, i + 1 + ring1.size());
    ASSERT_EQ(point.lat, i + 1 + ring1.size());
  }
}

// Test if the size stays correct even with many more rings
TEST(ReaderTest, Test3) {
  auto map = reader.getPolygonMap();

  auto polygonfield = map["Test3"];
  ASSERT_EQ(polygonfield.size(), 1u);

  auto polygon = polygonfield[0];
  ASSERT_EQ(polygon.rings_.size(), 6u);
}

// Test if a MULTIPOLYGON works with the same structure as the normal on in Test
// 1
TEST(ReaderTest, Test4) {
  auto map = reader.getPolygonMap();

  auto polygonfield = map["Test4"];
  ASSERT_EQ(polygonfield.size(), 1u);

  auto polygon = polygonfield[0];
  ASSERT_EQ(polygon.rings_.size(), 1u);

  auto ring = polygon.rings_[0];
  for (size_t i = 0; i < ring.size(); i++) {
    auto point = ring[i];
    ASSERT_EQ(point.lon, i + 1);
    ASSERT_EQ(point.lat, i + 1);
  }
}

// Test if multiple rings work in a MULTIPOLYGON
TEST(ReaderTest, Test5) {
  auto map = reader.getPolygonMap();

  auto polygonfield = map["Test5"];
  ASSERT_EQ(polygonfield.size(), 1u);

  auto polygon = polygonfield[0];
  ASSERT_EQ(polygon.rings_.size(), 2u);

  auto ring1 = polygon.rings_[0];
  for (size_t i = 0; i < ring1.size(); i++) {
    auto point = ring1[i];
    ASSERT_EQ(point.lon, i + 1);
    ASSERT_EQ(point.lat, i + 1);
  }

  auto ring2 = polygon.rings_[1];
  for (size_t i = 0; i < ring2.size(); i++) {
    auto point = ring2[i];
    ASSERT_EQ(point.lon, i + 1 + ring1.size());
    ASSERT_EQ(point.lat, i + 1 + ring1.size());
  }
}

// Test if multiple polygons work with multiple rings inside of a MULTIPOLYGON
TEST(ReaderTest, Test6) {
  auto map = reader.getPolygonMap();

  auto polygonfield = map["Test6"];
  ASSERT_EQ(polygonfield.size(), 2u);

  auto polygon1 = polygonfield[0];
  ASSERT_EQ(polygon1.rings_.size(), 2u);

  auto polygon2 = polygonfield[1];
  ASSERT_EQ(polygon2.rings_.size(), 2u);
}
