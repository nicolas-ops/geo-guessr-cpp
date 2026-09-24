// Copyright (c) 2026 Nicolas Romanov. All Rights Reserved.

#include "PolygonPainter.h"
#include "DataReader.h"
#include "MockTerminalManager.h"
#include "gtest/gtest.h"
#include <unistd.h>

MockTerminalManager tm(100, 100);
DataReader reader("./testpolygons.tsv");
PolygonPainter painter(&tm, &reader);
std::unordered_map<std::string, std::vector<Polygon>> polygonMap =
    painter.getCalculatedPolygonMap();

// We check if all the points have successfully been mapped to fit inside of the
// 100x100 terminal
TEST(PolygonPainterTest, CalculatedPolygonMap) {
  for (auto &pair : polygonMap) {
    auto polygons = pair.second;
    for (auto &polygon : polygons) {
      for (auto &ring : polygon.rings_) {
        for (auto &point : ring) {
          ASSERT_TRUE(point.lon >= 0 && point.lat >= 0 && point.lon <= 100 &&
                      point.lat <= 100);
        }
      }
    }
  }
}

// We go through all the points in this test Polygon to test if its painted
// correctly
TEST(PolygonPainterTest, drawPolygonBorder) {
  painter.drawPolygonBorder("Test8", TerminalManager::Red);
  std::vector<Polygon> polygons = polygonMap["Test8"];
  for (auto &polygon : polygons) {
    for (auto &ring : polygon.rings_) {
      for (auto &point : ring) {
        ASSERT_TRUE(tm.isPixelDrawn(point.lon, point.lat));
        ASSERT_EQ(tm.getColor(point.lon, point.lat), TerminalManager::Red);
      }
    }
  }
}

// Go through all the insides of the polygon, similar to the way in the fill
// polygon method and check if its painted correctly
TEST(PolygonPainterTest, fillPolygon) {
  painter.fillPolygon("Test8", TerminalManager::Blue);
  std::map<int, std::vector<int>> &intersectionRowMap8 =
      painter.intersectionPointsMap_["Test8"];

  for (int row = 0; row < tm.numRows(); row++) {
    std::vector<int> &intersections = intersectionRowMap8[row];
    for (size_t i = 0; i < intersections.size(); i += 2) {
      int beginIntersection = intersections[i];
      int endIntersection = intersections[i + 1];

      for (int col = beginIntersection; col < endIntersection; col++) {
        ASSERT_TRUE(tm.isPixelDrawn(row, col));
        ASSERT_EQ(tm.getColor(row, col), TerminalManager::Blue);
      }
    }
  }

  painter.fillPolygon("Test9", TerminalManager::Blue);
  std::map<int, std::vector<int>> &intersectionRowMap9 =
      painter.intersectionPointsMap_["Test9"];

  for (int row = 0; row < tm.numRows(); row++) {
    std::vector<int> &intersections = intersectionRowMap9[row];
    for (size_t i = 0; i < intersections.size(); i += 2) {
      int beginIntersection = intersections[i];
      int endIntersection = intersections[i + 1];

      for (int col = beginIntersection; col < endIntersection; col++) {
        ASSERT_TRUE(tm.isPixelDrawn(row, col));
        ASSERT_EQ(tm.getColor(row, col), TerminalManager::Blue);
      }
    }
  }
}

// We go through all the inside points of these polygons and check if its inside
// the polygon
TEST(PolygonPainterTest, PointInPolygon) {
  painter.fillPolygon("Test8", TerminalManager::Blue);
  std::map<int, std::vector<int>> &intersectionRowMap8 =
      painter.intersectionPointsMap_["Test8"];

  for (int row = 0; row < tm.numRows(); row++) {
    std::vector<int> &intersections = intersectionRowMap8[row];
    for (size_t i = 0; i < intersections.size(); i += 2) {
      int beginIntersection = intersections[i];
      int endIntersection = intersections[i + 1];

      for (int col = beginIntersection; col < endIntersection; col++) {
        ASSERT_TRUE(painter.isPointInsidePolygon("Test8", row, col));
        ASSERT_EQ(painter.getPolygonNameOnPoint(row, col), "Test8");
      }
      ASSERT_FALSE(
          painter.isPointInsidePolygon("Test8", row, endIntersection + 1));
      ASSERT_EQ(painter.getPolygonNameOnPoint(row, endIntersection + 1),
                "Null");
    }
  }

  painter.fillPolygon("Test9", TerminalManager::Blue);
  std::map<int, std::vector<int>> &intersectionRowMap9 =
      painter.intersectionPointsMap_["Test9"];

  for (int row = 0; row < tm.numRows(); row++) {
    std::vector<int> &intersections = intersectionRowMap9[row];
    for (size_t i = 0; i < intersections.size(); i += 2) {
      int beginIntersection = intersections[i];
      int endIntersection = intersections[i + 1];

      for (int col = beginIntersection; col < endIntersection; col++) {
        ASSERT_TRUE(painter.isPointInsidePolygon("Test9", row, col));
        ASSERT_EQ(painter.getPolygonNameOnPoint(row, col), "Test9");
      }
    }
  }
}
