// Copyright (c) 2026 Nicolas Romanov. All Rights Reserved.

#include "BrailleTerminalManager.h"
#include "TerminalManager.h"
#include "locale.h"
#include <cctype>
#include <cmath>
#include <cwchar>
#include <cwctype>
#include <ncurses.h>
#include <string>
#include <unistd.h>

//____________________________________________________________________________
BrailleTerminalManager::BrailleTerminalManager() {
  setlocale(LC_ALL, "");
  initscr();
  start_color();
  cbreak();
  noecho();
  curs_set(false);
  nodelay(stdscr, true);
  keypad(stdscr, true);
  use_default_colors();
  // Catch mouse events
  mousemask(ALL_MOUSE_EVENTS, NULL);
  mouseinterval(0);

  numColors_ = (COLORS < 16) ? COLORS : 16;
  for (int c = 0; c < numColors_; ++c) {
    init_pair(c + 1, c, BACKGROUND_COLOR);
    init_pair(numColors_ + c + 1, c, UPPERBAR_COLOR);
  }
  numRows_ = LINES * 4;
  numCols_ = COLS * 2;

  screenBuffer.assign(LINES * COLS, 0x2800);

  // Set a background color
  int bgpair = BACKGROUND_COLOR + 1;
  bkgd(COLOR_PAIR(bgpair));

  // Calculate the black size of the bar at the top of the screen
  upperOffset = numRows_ / 10;

  int barpair = numColors_ + UPPERBAR_COLOR + 1;

  // draw the bar.cpp
  for (int row = 0; row < upperOffset / 4; row++) {
    for (int col = 0; col < COLS; col++) {
      attron(COLOR_PAIR(barpair));
      mvprintw(row, col, " ");
      attroff(COLOR_PAIR(barpair));
    }
  }
  refresh();
  // Check if the console is tall enough
  if (LINES < 30) {
    drawString(0, 0,
               "Console is too small for ideal playing experience!, consider "
               "increasing the resolution.",
               TerminalManager::Red + numColors_);
    refresh();
    sleep(3);
  }
}

//____________________________________________________________________________
BrailleTerminalManager::~BrailleTerminalManager() { endwin(); }

// Converts local x y coordinates into a braille character
wchar_t getBrailleChar(int x, int y) {

  wchar_t braille_base = 0x2800;
  const int brailleValues[4][2] = {
      {0x01, 0x08}, {0x02, 0x10}, {0x04, 0x20}, {0x40, 0x80}};
  if (x < 0 || x > 1 || y < 0 || y > 3) {
    return braille_base;
  };

  return braille_base + brailleValues[y][x];
}

//____________________________________________________________________________
void BrailleTerminalManager::drawPixel(int row, int col, int color) {

  int terminalRow = int(row / 4);
  int terminalCols = int(col / 2);

  // return if its out of bounds to prevent errors
  if (terminalRow < 0 || terminalRow >= LINES || terminalCols < 0 ||
      terminalCols >= COLS) {
    return;
  }

  int brailley = row % 4;
  int braillex = col % 2;

  int index = getIndex(terminalRow, terminalCols);
  wchar_t character = getBrailleChar(braillex, brailley) - 0x2800;

  wchar_t newcharacter;

  int basecolor = color;
  if (basecolor >= numColors_) {
    basecolor -= numColors_;
  }

  int currentBackgroundColor =
      row < upperOffset ? UPPERBAR_COLOR : BACKGROUND_COLOR;

  // If the color we want to draw the pixel with, is the same as the background
  // color, then we remove the pixel. if its not the same we draw it
  if (basecolor == currentBackgroundColor) {
    newcharacter = screenBuffer[index] & ~character;
    screenBuffer[index] = newcharacter;
  } else {
    newcharacter = screenBuffer[index] | character;
    screenBuffer[index] = newcharacter;
  }

  int pair = color + 1;

  attron(COLOR_PAIR(pair));
  mvprintw(terminalRow, terminalCols, "%lc", newcharacter);
  attroff(COLOR_PAIR(pair));
}

// ___________________________________________________________________________
void BrailleTerminalManager::drawString(int row, int col, const char *output,
                                        int color) {
  attron(COLOR_PAIR(color + 1));
  mvaddstr(row, 2 * col, output);
  // clear the line
  clrtoeol();
  attroff(COLOR_PAIR(color + 1));
}

//____________________________________________________________________________
void BrailleTerminalManager::drawCharacter(int row, int col, wchar_t character,
                                           int color, int size) {

  wchar_t upperLetter = std::towupper(character);

  // return if the character is not part of our dictionary
  if (pixelFont.find(upperLetter) == pixelFont.end()) {
    return;
  }

  std::vector<std::string> data = pixelFont[upperLetter];
  // Go throuhg every row and collum inside of the string vector to read where
  // there are bits and where theres none
  for (size_t i = 0; i < data.size(); i++) {
    std::string bits = data[i].c_str();
    for (size_t j = 0; j < bits.size(); j++) {
      if (bits[j] == '#') {

        // Draw "fatter" pixels for each character depending on the size.
        for (int drow = 0; drow < size; drow++) {
          for (int dcol = 0; dcol < size; dcol++) {
            drawPixel(row + (i * size) + drow, col + (j * size) + dcol, color);
          }
        }
      }
    }
  }
}

//____________________________________________________________________________
void BrailleTerminalManager::drawStringDynamic(float x, float y,
                                               std::string str, int color,
                                               bool invert, int size) {

  // Converts our regular string into a wstring to support 2 byte wcharacters
  // like Ä,Ö,Ü
  std::wstring wstr;
  wstr.resize(str.size());
  size_t outsize = std::mbstowcs(&wstr[0], str.c_str(), str.size());
  if (outsize != (size_t)-1) {
    wstr.resize(outsize);
  } else {
    // if it doesnt work just clear the string
    wstr.clear();
  }

  int characterHeight = (numRows_ / 50) * size;
  int scale = std::max(1, (int)std::round(characterHeight / 5.0f));

  int row = int(numRows_ * y);
  // To center the text we do some more math here
  int col = int(numCols_ * x - str.size() * 3 * scale);

  // Switches from the regular backgroundcolor to the upperbar color
  if (invert) {
    color += numColors_;
  }

  for (size_t i = 0; i < wstr.size(); i++) {
    drawCharacter(row, col + i * 6 * scale, wstr[i], color, scale);
  }
  refresh();
}

//____________________________________________________________________________
UserInput BrailleTerminalManager::getUserInput() {
  UserInput userInput;
  userInput.keycode_ = getch();
  if (userInput.keycode_ == KEY_MOUSE) {
    MEVENT mouseEvent;
    if (getmouse(&mouseEvent) == OK) {
      if (mouseEvent.bstate & BUTTON1_PRESSED) {
        userInput.mouseRow_ = mouseEvent.y;
        userInput.mouseCol_ = mouseEvent.x / 2;
      }
    }
  }
  return userInput;
}

//____________________________________________________________________________
void BrailleTerminalManager::refresh() { ::refresh(); }
