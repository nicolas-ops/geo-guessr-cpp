// Copyright 2026, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Author: Johannes Kalmbach <kalmbach@cs.uni-freiburg.de>.
// Author: Christoph Ullinger <ullingec@cs.uni-freiburg.de>.

#include "./TerminalManager.h"
#include <cmath>
#include <cstdlib>
#include <ncurses.h>

// ____________________________________________________________________________
bool UserInput::isEscape() { return keycode_ == 27; }
bool UserInput::isKeyLeft() { return keycode_ == KEY_LEFT; }
bool UserInput::isKeyRight() { return keycode_ == KEY_RIGHT; }
bool UserInput::isKeyUp() { return keycode_ == KEY_UP; }
bool UserInput::isKeyDown() { return keycode_ == KEY_DOWN; }
bool UserInput::isMouseclick() { return mouseRow_ != -1; }
