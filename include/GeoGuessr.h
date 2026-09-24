// Copyright (c) 2026 Nicolas Romanov. All Rights Reserved.

#pragma once

#include "BrailleTerminalManager.h"
#include "DataReader.h"
#include "PolygonPainter.h"
#include "TerminalManager.h"
#include <cfenv>
#include <gtest/gtest_prod.h>
#include <string>
#include <unordered_map>

enum DecisionResult { INVALID = -1, HIT = 1, MISS = 0 };

// THis class is responsible for all the game Logic, like keeping count of the
// score,
// choosing which Polygon to guess next, handling all the multiple attempts in
// case
// of a wrong guess, etc..
class GeoGuessr {
public:
  // Sets up game parameters
  GeoGuessr(TerminalManager &tm, const std::string filename);

  // The main GeoGuessr game logic with the main loop
  void startGame();

  // Returns the current score the player has in the game
  int getScore() const { return score_; };

private:
  // Gets a random Polygon name (for example Bayern out of the Germany set)
  // and returns it. It also removes the name from the available list so it
  // doesnt show up twice.
  std::string getRandomPolygonName();

  // Checks if the player has chosen the right country, and if the input was
  // valid
  DecisionResult evaluateUserDecision(UserInput &userInput);

  // The current polygon name (for example Bayern if the current round asks
  // you to find Bayern)
  std::string currentPolygonName_;

  // A vector holding all remaining polygon names. After each correct guess or
  // fail a new name gets picked and removed from this vector.
  std::vector<std::string> availablePolygonNames_;

  // The current score of the player in the game
  int score_;

  // the max achievable amount of scores the player can get with the data set
  int maxScore_;

  // the number of attempts the player has left in one round. it will decrease
  // with each wrong guess and reset each round
  int numAttempts_ = 3;

  // a constant map which has all the colors for each attempt. We decide which
  // color the polygon should be depending on how many attempts it took to
  // guess, or fail.
  std::unordered_map<int, int> attemptColors_ = {{3, TerminalManager::Green},
                                                 {2, TerminalManager::Cyan},
                                                 {1, TerminalManager::Yellow},
                                                 {0, TerminalManager::Red}};

  bool playing_ = true;

  TerminalManager *tm_;
  DataReader reader_;
  PolygonPainter painter_;

  FRIEND_TEST(GeoGuessrTest, getRandomPolygonName);
};
