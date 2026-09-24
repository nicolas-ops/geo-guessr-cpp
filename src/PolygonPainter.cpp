// Copyright (c) 2026 Nicolas Romanov. All Rights Reserved.

#include "PolygonPainter.h"
#include "DataReader.h"
#include "TerminalManager.h"
#include <algorithm>
#include <cstdlib>
#include <string>
#include <vector>

//____________________________________________________________________________
PolygonPainter::PolygonPainter(TerminalManager *tm, DataReader *reader) {
  tm_ = tm;

  int upperoffset = tm_->getUpperOffset();

  int numRows = tm_->numRows() - upperoffset;
  int numCols = tm_->numCols();

  float largestLongitude = reader->getLargestLongitude();
  float smallestLongitude = reader->getSmallestLongitude();
  float largestLatitude = reader->getLargestLatitude();
  float smallestLatitude = reader->getSmallestLatitude();

  // Calculates what the size of each pixel should be and picks the smallest one
  // to maximise the usabillity of the terminal
  scalarCols_ = (float)(numCols - 1) / (largestLongitude - smallestLongitude);
  scalarRows_ = (float)(numRows - 1) / (largestLatitude - smallestLatitude);

  scalar_ = std::min(scalarCols_, scalarRows_);

  mapWidth_ = (largestLongitude - smallestLongitude) * scalar_;
  mapHeight_ = (largestLatitude - smallestLatitude) * scalar_;

  // Offset to center the map and also shift it down a bit for the upper bar
  offsetX_ = ((numCols - 1) - mapWidth_) / 2.0f;
  offsetY_ = ((numRows - 1) - mapHeight_ + ((upperoffset + 2) * 2)) / 2.0f;

  std::unordered_map<std::string, std::vector<Polygon>> rawPolygons =
      reader->getPolygonMap();
  // This loop goes through every point inside of this entire map and converts
  // it into Terminal coordinates, then it puts all the new polyons in the
  // calculatedPolygonMap
  for (auto &pair : rawPolygons) {

    std::string name = pair.first;
    std::vector<Polygon> polygons = pair.second;
    std::vector<Polygon> newPolygons;

    for (Polygon &polygon : polygons) {

      std::vector<std::vector<Point>> rings = polygon.rings_;
      std::vector<std::vector<Point>> newRings;

      for (std::vector<Point> &ring : rings) {

        std::vector<Point> newRing;

        for (Point &p : ring) {
          // convert the points to terminal coordinate points
          int y = (p.lon - smallestLongitude) * scalar_ + offsetX_;
          int x = (largestLatitude - p.lat) * scalar_ + offsetY_;
          x = std::clamp(x, 0, tm_->numRows());
          y = std::clamp(y, 0, tm_->numCols());
          newRing.emplace_back(x, y);
        }
        newRings.push_back(newRing);
      }
      Polygon newPolygon;
      newPolygon.rings_ = newRings;
      newPolygons.push_back(newPolygon);
    }
    calculatedPolygonMap_[name] = newPolygons;
  }

  // this loop fills the intersectionPointsMap up by going through all the
  // polygons and calculates the intersections for each row. It also draws the
  // polygons for the first time in their default colors.
  for (const auto &pair : calculatedPolygonMap_) {
    const std::string &name = pair.first;
    const std::vector<Polygon> &polygons = pair.second;

    for (const Polygon &polygon : polygons) {
      for (int row = 0; row < tm->numRows(); row++) {
        // get intersections and fill the map
        std::vector<int> intersections =
            computeIntersectionPoints(polygon, row);

        if (intersections.size() > 0) {
          std::vector<int> &rowvector = intersectionPointsMap_[name][row];
          rowvector.insert(rowvector.end(), intersections.begin(),
                           intersections.end());
        }
      }
    }
    // sort the intersections, so its from left to right
    for (auto &pair : intersectionPointsMap_[name]) {
      std::vector<int> &intersections = pair.second;
      std::sort(intersections.begin(), intersections.end());
    }

    // Draw all the polygons
    fillPolygon(name, TerminalManager::Black);
    drawPolygonBorder(name, TerminalManager::White);
  }
}

//____________________________________________________________________________
std::vector<int>
PolygonPainter::computeIntersectionPoints(const Polygon &polygon, int row) {

  // This is the same code as shown in the lecture. we go through all points and
  // use the formula to calculate every intersection.
  std::vector<int> intersectionCols;
  for (auto &ring : polygon.rings_) {
    for (size_t i = 0; i < ring.size(); i++) {
      Point p1 = ring[i];
      Point p2 = ring[(i + 1) % ring.size()];
      int r1 = p1.lon;
      int c1 = p1.lat;
      int r2 = p2.lon;
      int c2 = p2.lat;
      if (row >= std::max(r1, r2) || row < std::min(r1, r2)) {
        continue;
      }
      int intersectionCol = c1 + (c2 - c1) * (row - r1) / (r2 - r1);
      intersectionCols.push_back(intersectionCol);
    }
  }
  return intersectionCols;
}

//____________________________________________________________________________
void PolygonPainter::drawPolygonBorder(const std::string polygonName,
                                       int color) {
  std::vector<Polygon> &polygons = calculatedPolygonMap_[polygonName];

  for (Polygon polygon : polygons) {
    for (std::vector<Point> ring : polygon.rings_) {
      if (ring.size() < 2)
        continue;

      for (size_t i = 0; i < ring.size(); i++) {
        Point &p1 = ring[i];
        Point &p2 = ring[(i + 1) % ring.size()];

        // Bresenham's algorithm for drawing lines between two points

        // get the points
        int x0 = p1.lat;
        int x1 = p2.lat;
        int y0 = p1.lon;
        int y1 = p2.lon;

        // get the distance between the points
        int dx = std::abs(x0 - x1);
        int dy = -std::abs(y0 - y1);

        // get the direction towards the goal
        int sx = x0 < x1 ? 1 : -1;
        int sy = y0 < y1 ? 1 : -1;

        // initialize the error variable as the sum of the distances
        int error = dx + dy;
        while (true) {
          tm_->drawPixel(y0, x0, color);

          // target reached
          if (x0 == x1 && y0 == y1)
            break;

          // multiply the error by two, if its too large in either direction we
          // step in that direction and continue.
          int e2 = 2 * error;
          if (e2 >= dy) {
            if (x0 == x1)
              break;
            error = error + dy;
            x0 = x0 + sx;
          }
          if (e2 <= dx) {
            if (y0 == y1)
              break;
            error = error + dx;
            y0 = y0 + sy;
          }
        }
      }
    }
  }
  tm_->refresh();
}

//____________________________________________________________________________
void PolygonPainter::fillPolygon(const std::string polygonName, int color) {
  std::map<int, std::vector<int>> &intersectionRowMap =
      intersectionPointsMap_[polygonName];

  // go through everz row
  for (int row = 0; row < tm_->numRows(); row++) {
    std::vector<int> &intersections = intersectionRowMap[row];

    // this loop goes through every intersection and every collum inside the
    // intersections and draws it.
    for (size_t i = 0; i < intersections.size(); i += 2) {
      int beginIntersection = intersections[i];
      int endIntersection = intersections[i + 1];

      for (int col = beginIntersection; col < endIntersection; col++) {
        tm_->drawPixel(row, col, color);
      }
    }
  }
  tm_->refresh();
}

//____________________________________________________________________________
bool PolygonPainter::isPointInsidePolygon(const std::string polygonName,
                                          int row, int col) {

  if (intersectionPointsMap_[polygonName].find(row) ==
      intersectionPointsMap_[polygonName].end())
    return false;

  const std::vector<int> &intersections =
      intersectionPointsMap_[polygonName][row];
  int rightIntersectionCounter = 0;
  // this loop adds a counter if an intersection is to the right of the point
  for (int intersection : intersections) {
    if (intersection > col) {
      rightIntersectionCounter++;
    }
  }
  // returns true if the number of intersections to the right is uneven, which
  // means the points is inside the polygon
  return rightIntersectionCounter % 2 != 0;
}

//____________________________________________________________________________
std::string PolygonPainter::getPolygonNameOnPoint(int row, int col) {
  // Go through all the polygons and check if the point is in one of them.
  for (auto pair : calculatedPolygonMap_) {
    std::string name = pair.first;
    if (isPointInsidePolygon(name, row, col)) {
      return name;
    }
  }
  // If theres no polygon then return Null
  return "Null";
}
