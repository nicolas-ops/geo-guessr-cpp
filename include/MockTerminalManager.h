// Copyright (c) 2026 Nicolas Romanov

#pragma once

#include "./TerminalManager.h"
#include <cstddef>
#include <string>
#include <unordered_map>

// Custom hash functor for 2D coordinate pairs to use with std::unordered_map.

struct ScreenCoordHash {
  std::size_t operator()(const std::pair<int, int> &point) const noexcept {
    // Standard bit-combining hash (Boost/Murmur-style)
    std::size_t h1 = std::hash<int>{}(point.first);
    std::size_t h2 = std::hash<int>{}(point.second);
    return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
  }
};

//@brief Mock implementation of TerminalManager designed for headless unit
// testing. Records pixel operations in-memory without initializing an
// interactive terminal session.
class MockTerminalManager : public TerminalManager {
public:
  MockTerminalManager(int rows, int cols) : rowCount_(rows), colCount_(cols) {}

  ~MockTerminalManager() override = default;

  // TerminalManager interface overrides
  void drawPixel(int row, int col, int color) override;
  void drawString(int, int, const char *, int) override {}
  void drawStringDynamic(float, float, std::string, int, bool, int) override {}
  void refresh() override {}

  int numRows() const override { return rowCount_; }
  int numCols() const override { return colCount_; }

  // Test verification utilities
  bool isPixelDrawn(int row, int col) const;
  int getColor(int row, int col) const;
  void clear();

private:
  int rowCount_;
  int colCount_;
  std::unordered_map<std::pair<int, int>, int, ScreenCoordHash> pixelBuffer_;
};
