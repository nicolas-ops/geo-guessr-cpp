// Copyright (c) 2026 Nicolas Romanov. All Rights Reserved.

#include "MockTerminalManager.h"
#include <stdexcept>

void MockTerminalManager::drawPixel(int row, int col, int color) {
  // Record pixel coordinates into the buffer, matching safe ncurses bounds
  // behavior
  pixelBuffer_[{row, col}] = color;
}

bool MockTerminalManager::isPixelDrawn(int row, int col) const {
  return pixelBuffer_.find({row, col}) != pixelBuffer_.end();
}

int MockTerminalManager::getColor(int row, int col) const {
  auto it = pixelBuffer_.find({row, col});
  if (it == pixelBuffer_.end()) {
    throw std::out_of_range("Requested pixel coordinates are not drawn.");
  }
  return it->second;
}

void MockTerminalManager::clear() { pixelBuffer_.clear(); }
