// Copyright (c) 2026 Nicolas Romanov. All Rights Reserved.

#pragma once

#include "Point.h"
#include <cstddef>
#include <vector>

struct Polygon {
public:
  std::vector<std::vector<Point>> rings_;
};
