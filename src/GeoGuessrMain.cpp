// Copyright (c) 2026 Nicolas Romanov. All Rights Reserved.

#include "BrailleTerminalManager.h"
#include "GeoGuessr.h"
#include <iostream>

int main(int argc, char *argv[]) {

  if (argc != 2) {
    std::cout << "Arguments missing: <filepath>" << std::endl;
    return 0;
  }

  std::string filename(argv[1]);

  BrailleTerminalManager tm;

  GeoGuessr game(tm, filename);

  game.startGame();

  return 0;
}
