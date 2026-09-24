// Copyright (c) 2026 Nicolas Romanov. All Rights Reserved.

#include "GeoGuessr.h"
#include "DataReader.h"
#include "PolygonPainter.h"
#include "TerminalManager.h"
#include <cstdlib>
#include <ctime>
#include <string>
#include <unistd.h>

GeoGuessr::GeoGuessr(TerminalManager &tm, std::string filename)
    : tm_(&tm), reader_(filename), painter_(&tm, &reader_) {

  availablePolygonNames_ = reader_.getPolygonNames();

  maxScore_ = availablePolygonNames_.size();
}

//____________________________________________________________________________
void GeoGuessr::startGame() {

  std::string maxscoresstr = std::to_string(maxScore_);
  std::string scorestr = std::to_string(score_);

  std::srand(time(0));

  // ----- START ANIMATION -------
  for (int i = 0; i < 5; i++) {

    int randomcolor = std::rand() % 15 + 1;

    tm_->drawStringDynamic(0.5, 0.02, "GeoGuessr", randomcolor, true, 3);
    usleep(200000);
    tm_->drawStringDynamic(0.5, 0.02, "GeoGuessr", TerminalManager::Black, true,
                           3);
  }
  tm_->drawStringDynamic(0.5, 0.02, "GeoGuessr", TerminalManager::White, true,
                         3);
  usleep(1000000);
  tm_->drawStringDynamic(0.5, 0.02, "GeoGuessr", TerminalManager::Black, true,
                         3);
  // ----- END OF START ANIMATIOM -------

  while (playing_) {

    if (availablePolygonNames_.size() > 0) {
      numAttempts_ = 3;
      tm_->drawStringDynamic(0.5, 0, "Find: " + getRandomPolygonName(),
                             TerminalManager::White, true, 2);
      tm_->drawStringDynamic(0.5, (0.5) / 10,
                             "Attempts: " + std::to_string(numAttempts_),
                             TerminalManager::White, true, 1);
      tm_->drawStringDynamic(0.7, (0.5) / 10, scorestr + "/" + maxscoresstr,
                             TerminalManager::White, true, 1);
    } else {
      break;
    }

    while (true) {
      UserInput input = tm_->getUserInput();
      if (input.isEscape()) {
        playing_ = false;
        break;
      } else {
        DecisionResult decision = evaluateUserDecision(input);
        if (decision == INVALID) {
          continue;
        } else if (decision == HIT) {
          // Fill the polygon and redraw the border since it gets overdrawn
          painter_.fillPolygon(currentPolygonName_,
                               attemptColors_[numAttempts_]);
          painter_.drawPolygonBorder(currentPolygonName_,
                                     TerminalManager::White);

          // Remove all the current text before changing the variables so it can
          // get redrawn above without things overlapping
          tm_->drawStringDynamic(0.5, 0, "Find: " + currentPolygonName_,
                                 TerminalManager::Black, true, 2);
          tm_->drawStringDynamic(0.5, (0.5) / 10,
                                 "Attempts: " + std::to_string(numAttempts_),
                                 TerminalManager::Black, true, 1);
          tm_->drawStringDynamic(0.7, (0.5) / 10, scorestr + "/" + maxscoresstr,
                                 TerminalManager::Black, true, 1);

          score_++;
          scorestr = std::to_string(score_);
          break;
        } else if (decision == MISS) {
          // Remove the attempts text before changing anything. because in a
          // miss it will change either way
          tm_->drawStringDynamic(0.5, (0.5) / 10,
                                 "Attempts: " + std::to_string(numAttempts_),
                                 TerminalManager::Black, true, 1);

          numAttempts_ -= 1;
          if (numAttempts_ == 0) {
            // Draw the polygon with the failed color. at the bottom of the map.
            painter_.fillPolygon(currentPolygonName_,
                                 attemptColors_[numAttempts_]);
            painter_.drawPolygonBorder(currentPolygonName_,
                                       TerminalManager::White);
            // Remove the name string because the next round will have a
            // different one and we dont want overlaps
            tm_->drawStringDynamic(0.5, 0, "Find: " + currentPolygonName_,
                                   TerminalManager::Black, true, 2);
            break;
          }

          tm_->drawStringDynamic(0.5, (0.5) / 10,
                                 "Attempts: " + std::to_string(numAttempts_),
                                 TerminalManager::White, true, 1);
        }
      }
    }
  }

  tm_->drawStringDynamic(0.7, (0.5) / 10, scorestr + "/" + maxscoresstr,
                         TerminalManager::Black, true, 1);

  // Game over string containing your score
  tm_->drawStringDynamic(0.5, 0, "Game Over!", TerminalManager::White, true, 2);
  tm_->drawStringDynamic(0.5, (0.5) / 10,
                         "Points: " + scorestr + "/" + maxscoresstr,
                         TerminalManager::White, true, 1);

  // Last loop waiting for you to press escape
  while (playing_) {
    UserInput input = tm_->getUserInput();
    if (input.isEscape()) {
      playing_ = false;
      break;
    }
  }
}

//____________________________________________________________________________
std::string GeoGuessr::getRandomPolygonName() {
  size_t count = availablePolygonNames_.size();
  int randomNumber = std::rand() % count;

  currentPolygonName_ = availablePolygonNames_[randomNumber];
  availablePolygonNames_.erase(availablePolygonNames_.begin() + randomNumber);

  return currentPolygonName_;
}

//____________________________________________________________________________
DecisionResult GeoGuessr::evaluateUserDecision(UserInput &input) {
  if (input.isMouseclick()) {
    int row = input.mouseRow_ * 4;
    int col = input.mouseCol_ * 4;

    std::string polygonName = painter_.getPolygonNameOnPoint(row, col);

    if (polygonName == currentPolygonName_) {
      return HIT;
    } else if (polygonName == "Null") {
      return INVALID;
    } else {
      return MISS;
    }

  } else {
    return INVALID;
  }
}
