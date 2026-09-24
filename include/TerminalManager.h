// Copyright (c) 2026 Nicolas Romanov. All Rights Reserved.

#pragma once

#include <string>

// User input (a key press or a mouse click), shared by all `TerminalManager`
// subclasses that read input
class UserInput {
public:
  bool isEscape();
  bool isKeyLeft();
  bool isKeyRight();
  bool isKeyUp();
  bool isKeyDown();
  bool isMouseclick();
  int keycode_;
  // The position of a mouse click, or -1/-1 if the event was a key press.
  int mouseRow_ = -1;
  int mouseCol_ = -1;
};

// An abstract base class that defines an interface for drawing pixels
// on a screen.
class TerminalManager {
public:
  // Colors usable with on any TerminalManager implementation, matching the
  // standard ANSI/ncurses 8-color palette (0..7) plus a "bright black"/grey
  // at 8.
  static constexpr int Black = 0;
  static constexpr int Red = 1;
  static constexpr int Green = 2;
  static constexpr int Yellow = 3;
  static constexpr int Blue = 4;
  static constexpr int Magenta = 5;
  static constexpr int Cyan = 6;
  static constexpr int White = 7;
  // "Bright black". Needs a terminal with at least 16 colors.
  static constexpr int Grey = 8;
  // "Orange". Not part of the base ANSI palette; ncurses-based :backends fall
  // back to their palette's "bright red" (color 9).
  static constexpr int Orange = 9;
  // "Bright yellow". Needs a terminal with at least 16 colors.
  static constexpr int BrightYellow = 11;

  // Virtual destructor.
  virtual ~TerminalManager() = default;

  // Draw a pixel at the given position in the given color. The meaning of a
  // color value is up to the concrete implementation.
  virtual void drawPixel(int row, int col, int color) = 0;

  // Draw a string at the given position in the given color.
  virtual void drawString(int row, int col, const char *str, int color) = 0;

  // Draw a dynamic string in the given scalar in the given color and size
  virtual void drawStringDynamic(float x, float y, std::string str, int color,
                                 bool invert, int size = 1) = 0;

  // Get input from the user (a key press or a mouse click). The default
  // implementation reports "no input this frame", for backends (like
  // MockTerminalManager).
  virtual UserInput getUserInput() { return UserInput{}; }

  // Refresh the screen.
  virtual void refresh() = 0;

  // Get the dimensions of the screen.
  virtual int numRows() const = 0;
  virtual int numCols() const = 0;

  virtual int getUpperOffset() const { return 0; };
};
