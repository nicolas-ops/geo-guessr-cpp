// Copyright (c) 2026 Nicolas Romanov. All Rights Reserved.

#pragma once

#include "TerminalManager.h"
#include <map>
#include <ncursesw/ncurses.h>
#include <string>
#include <vector>

// This class is responsible for handling the Pixels that are shown on the
// Terminal. This version uses Braille to show each pixel instead of "  ", which
// gives us more resolution.
class BrailleTerminalManager : public TerminalManager {
public:
  // Sets up the Ncuses Terminal but with ncursesw
  BrailleTerminalManager();

  // closes the ncurses terminal
  ~BrailleTerminalManager();

  // Draws a pixel with the color at the given row and col, but with Braille
  void drawPixel(int row, int col, int color) override;

  // Draws a regular ncurses type string
  void drawString(int row, int col, const char *str, int color) override;

  // Draws a string with Braille pixels. instead of using rows and cols we use
  // scalars. So x = 0.5 and y = 0.5 would be around in the middle of the
  // screen.
  void drawStringDynamic(float x, float y, std::string str, int color,
                         bool invert, int size = 1) override;

  // Returns the UserInput at the current frame
  UserInput getUserInput() override;

  // refreshes the screen
  void refresh() override;

  // Returns the number of rows and cols of Braille pixels
  int numRows() const override { return numRows_; };
  int numCols() const override { return numCols_; };

  // Returns the size of the upper bar
  int getUpperOffset() const override { return upperOffset; };

  // returns the index of a 1D array
  int getIndex(int row, int col) const { return (row * COLS) + col; }

private:
  // The size of the upper bar, gets set in the constructor
  int upperOffset;

  const int BACKGROUND_COLOR = TerminalManager::Grey;
  const int UPPERBAR_COLOR = TerminalManager::Black;

  // Draws a given character in Braille if its part of the pixelFont dictionary.
  void drawCharacter(int row, int col, wchar_t character, int color, int size);

  // This vector holds all the data about the pixels on the screen
  std::vector<wchar_t> screenBuffer;

  int numRows_;
  int numCols_;

  int numColors_;

  // A custom pixel font. This is inspired off of tom thumbs micro font.
  std::map<wchar_t, std::vector<std::string>> pixelFont = {

      {'!', {"  #  ", "  #  ", "  #  ", "     ", "  #  "}},

      {':', {"     ", "  #  ", "     ", "  #  ", "     "}},
      {'/', {"    #", "   # ", "  #  ", " #   ", "#    "}},

      // numbers
      {'0', {" ### ", "#   #", "#   #", "#   #", " ### "}},
      {'1', {"  ## ", " # # ", "   # ", "   # ", " ### "}},
      {'2', {" ### ", "    #", " ### ", "#    ", "#####"}},
      {'3', {" ### ", "    #", " ### ", "    #", " ### "}},
      {'4', {"#   #", "#   #", "#####", "    #", "    #"}},
      {'5', {"#####", "#    ", "#### ", "    #", "#### "}},
      {'6', {" ### ", "#    ", "#### ", "#   #", " ### "}},
      {'7', {"#####", "    #", "   # ", "  #  ", "  #  "}},
      {'8', {" ### ", "#   #", " ### ", "#   #", " ### "}},
      {'9', {" ### ", "#   #", " ####", "    #", " ### "}},

      // letters
      {'A', {" ### ", "#   #", "#####", "#   #", "#   #"}},
      {'B', {"#### ", "#   #", "#### ", "#   #", "#### "}},
      {'C', {" ####", "#    ", "#    ", "#    ", " ####"}},
      {'D', {"#### ", "#   #", "#   #", "#   #", "#### "}},
      {'E', {"#####", "#    ", "#### ", "#    ", "#####"}},
      {'F', {"#####", "#    ", "#### ", "#    ", "#    "}},
      {'G', {" ####", "#    ", "#  ##", "#   #", " ####"}},
      {'H', {"#   #", "#   #", "#####", "#   #", "#   #"}},
      {'I', {" ### ", "  #  ", "  #  ", "  #  ", " ### "}},
      {'J', {"   ##", "    #", "    #", "#   #", " ### "}},
      {'K', {"#   #", "#  # ", "###  ", "#  # ", "#   #"}},
      {'L', {"#    ", "#    ", "#    ", "#    ", "#####"}},
      {'M', {"#   #", "## ##", "# # #", "#   #", "#   #"}},
      {'N', {"#   #", "##  #", "# # #", "#  ##", "#   #"}},
      {'O', {" ### ", "#   #", "#   #", "#   #", " ### "}},
      {'P', {"#### ", "#   #", "#### ", "#    ", "#    "}},
      {'Q', {" ### ", "#   #", "# # #", "#  ##", " ####"}},
      {'R', {"#### ", "#   #", "#### ", "#  # ", "#   #"}},
      {'S', {" ####", "#    ", " ### ", "    #", "#### "}},
      {'T', {"#####", "  #  ", "  #  ", "  #  ", "  #  "}},
      {'U', {"#   #", "#   #", "#   #", "#   #", " ### "}},
      {'V', {"#   #", "#   #", "#   #", " # # ", "  #  "}},
      {'W', {"#   #", "#   #", "# # #", "## ##", "#   #"}},
      {'X', {"#   #", " # # ", "  #  ", " # # ", "#   #"}},
      {'Y', {"#   #", " # # ", "  #  ", "  #  ", "  #  "}},
      {'Z', {"#####", "   # ", "  #  ", " #   ", "#####"}},
      {L'Ä', {"#   #", " ### ", "#   #", "#####", "#   #"}},
      {L'Ö', {"#   #", " ### ", "#   #", "#   #", " ### "}},
      {L'Ü', {"#   #", "     ", "#   #", "#   #", " ### "}},
      {L'ß', {" ##  ", "#  # ", "###  ", "#  # ", "#    "}},
  };
};
