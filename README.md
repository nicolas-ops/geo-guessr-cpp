# Terminal GeoGuessr

A , terminal-based GeoGuessr clone written in C++. This project renders maps and polygons directly in the console and calculates spatial distances to let players guess locations.

## Features

- Custom terminal-based map rendering
- Spatial coordinate algorithms and polygon painting
- Comprehensive unit testing setup using Google Test (`gtest`)

## Project Structure

- `src/` - C++ source files (`.cpp`)
- `include/` - C++ header files (`.h`)

## Prerequisites

To build and run this project, you need:
- A C++17 compatible compiler (e.g., `clang++-18`)
- `make`
- `ncurses` (linked via `-lncursesw`)
- `gtest` (for running unit tests)

## Build & Run

The project includes a `Makefile` for easy building. Run the following commands in the root directory:

**1. Compile the project:**
```bash
make
```
**2. Run the game
```bash
./GeoGuessrMain <dataset-filepath>
```
# Academic Integrity & Datasets
**Note: This project was originally developed as part of a computer science course at the University of Freiburg. To comply with academic guidelines and copyright policies, all proprietary university datasets and specific evaluation classes have been excluded from this public repository.

The repository includes independent, clean-room mock implementations (e.g., `MockTerminalManager`) and synthetic dummy datasets so the code builds and tests run successfully out-of-the-box.
