// Copyright (c) 2026 Nicolas Romanov. All Rights Reserved.

#pragma once

#include "BrailleTerminalManager.h"
#include "DataReader.h"
#include "Point.h"
#include "Polygon.h"
#include "TerminalManager.h"
#include "gtest/gtest_prod.h"
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

// This class is responsible for drawing all kinds of Polygons on the screen
// using the TerminalManager. It includes various functions like drawing the
// border of a Polygon, filling the polygon with any color and also calculating
// other things essential for the game logic
class PolygonPainter {
public:
  // Stores the tm in a variable for later and calculates various things to
  // make the polygons centered and all visible.
  PolygonPainter(TerminalManager *tm, DataReader *reader);

  // Draws the border of a given Polygon with the given color on the terminal
  void drawPolygonBorder(const std::string polygonName, int color);

  // Fills the given Polygon on the Terminal with the given color
  void fillPolygon(const std::string polygonName, int color);

  // Returns the new calculated polygon map
  std::unordered_map<std::string, std::vector<Polygon>>
  getCalculatedPolygonMap() const {
    return calculatedPolygonMap_;
  };

  // Returns true if the given points is inside of the polygon with the given
  // name. returns false if not
  bool isPointInsidePolygon(const std::string polygonName, int row, int col);

  // Returns which polygonname has this point. Returns "Null" if theres no
  // polygon
  std::string getPolygonNameOnPoint(int row, int col);

private:
  TerminalManager *tm_;

  // Returns all the intersection points of a given polygon
  std::vector<int> computeIntersectionPoints(const Polygon &polygon, int row);

  // A new polygon map taken from the data reader class, but all polygon
  // coordinates are converted to terminal coordinates. The name of the polygon
  // is the key.
  std::unordered_map<std::string, std::vector<Polygon>> calculatedPolygonMap_;

  // a map which holda all intersection points for each polygon and row. The
  // name of the polygon is the key
  std::unordered_map<std::string, std::map<int, std::vector<int>>>
      intersectionPointsMap_;

  float scalarRows_;
  float scalarCols_;
  float scalar_;

  float mapWidth_;
  float mapHeight_;

  float offsetX_;
  float offsetY_;

  FRIEND_TEST(PolygonPainterTest, fillPolygon);
  FRIEND_TEST(PolygonPainterTest, PointInPolygon);
};
