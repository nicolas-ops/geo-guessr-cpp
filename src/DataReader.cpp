// Copyright (c) 2026 Nicolas Romanov. All Rights Reserved.

#include "DataReader.h"
#include "Point.h"
#include <cstdlib>
#include <cstring>
#include <exception>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <string_view>
#include <vector>

// Takes the string form of a bunch of points and turns them into a vector of
// points
std::vector<Point> stringToPoints(std::string_view view) {
  std::vector<Point> points;
  int lastcomma = -1;
  for (size_t i = 0; i < view.size(); i++) {
    char c = view[i];
    if (c == ',') {
      std::string_view numbers = view.substr(lastcomma + 1, i - lastcomma - 1);
      int numbersplit = numbers.find(" ");
      std::string longitudestring = std::string(numbers.substr(0, numbersplit));
      std::string latitudestring = std::string(
          numbers.substr(numbersplit, numbers.size() - numbersplit));

      float longitude = std::atof(longitudestring.c_str());
      float latitude = std::atof(latitudestring.c_str());
      points.emplace_back(longitude, latitude);
      lastcomma = i;
    } else if (i == view.size() - 1) {
      std::string_view numbers = view.substr(lastcomma + 1);

      int numbersplit = numbers.find(" ");
      std::string longitudestring = std::string(numbers.substr(0, numbersplit));
      std::string latitudestring = std::string(numbers.substr(numbersplit + 1));

      float longitude = std::atof(longitudestring.c_str());
      float latitude = std::atof(latitudestring.c_str());
      points.emplace_back(longitude, latitude);
    }
  }
  return points;
}

//_____________________________________________________________________________
DataReader::DataReader(std::string filename) {
  // Read the file, if it doesnt exist then exit the program
  std::ifstream file(filename);
  if (!file) {
    std::cout << "File " << filename << " not found, aborting!" << std::endl;
    std::terminate();
  }
  std::string line;

  // Checks if the first line has this form, idk if this is important to check
  if (std::getline(file, line)) {
    if (!std::strcmp(line.c_str(), "?label  ?geometry")) {
      std::cout << line << std::endl;
      std::terminate();
    }
  }

  while (std::getline(file, line)) {
    std::string_view view(line);

    size_t nameStartPos = view.find('"', 0) + 1;
    size_t nameEndPos = view.find('"', nameStartPos);

    std::string_view name =
        view.substr(nameStartPos, nameEndPos - nameStartPos);

    // Adds the name of the current entry into the vecotr
    polygonNames_.emplace_back(name);

    int startpos = view.find("(");

    int depth = 0;
    int polygondepth = 1;
    int ringdepth = 2;

    if (view.find("MULTIPOLYGON") != std::string::npos) {
      polygondepth = 2;
      ringdepth = 3;
    }

    std::string_view dataview = view.substr(startpos);

    int lastparentheses = 0;

    Polygon currentPolygon;
    std::vector<Polygon> polygons;
    std::vector<Point> currentRing;

    // this goes through every character to parse the data inside of the
    // parentheses
    for (size_t i = 0; i < dataview.size(); i++) {
      char c = dataview[i];
      if (c == '(') {
        lastparentheses = i;
        depth++;

      } else if (c == ')') {
        if (depth == ringdepth) {
          // reached the end of a ring so we save it for when we leave the ring
          currentRing = stringToPoints(
              dataview.substr(lastparentheses + 1, i - lastparentheses - 1));
          for (Point p : currentRing) {
            if (p.lon > largestLon_) {
              largestLon_ = p.lon;
            } else if (p.lon < smallestLon_) {
              smallestLon_ = p.lon;
            } else if (p.lat > largestLat_) {
              largestLat_ = p.lat;
            } else if (p.lat < smallestLat_) {
              smallestLat_ = p.lat;
            }
          }
        } else if (depth == polygondepth) {
          // reached the end of a polygon so we have to add it to the vector and
          // start with a clear one
          polygons.push_back(currentPolygon);
          currentPolygon.rings_.clear();
        }
        depth--;
        if (depth == polygondepth) {
          // after leaving a ring we add it to the current polygon
          currentPolygon.rings_.push_back(currentRing);
        }
      }
    }
    polygonMap_[std::string(name)] = polygons;
  }
}
