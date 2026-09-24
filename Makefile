.SUFFIXES:
.PRECIOUS: %.o
.PHONY: all compile checkstyle test clean format

CXX = clang++-18 -std=c++17 -g -Wall -Wextra -Wdeprecated -fsanitize=address -I/usr/include/freetype2 -Iinclude
LIBS = -lncursesw
TESTLIBS = -lgtest -lgtest_main -lpthread

# Find source files in src/
SRCS = $(wildcard src/*.cpp)
MAIN_SRCS = $(wildcard src/*Main.cpp)
TEST_SRCS = $(wildcard src/*Test.cpp)

# Binaries to build
MAIN_BINARIES = $(basename $(notdir $(MAIN_SRCS)))
TEST_BINARIES = $(basename $(notdir $(TEST_SRCS)))

# Shared objects (exclude Main and Test files)
COMMON_SRCS = $(filter-out src/%Main.cpp src/%Test.cpp, $(SRCS))
OBJECTS = $(COMMON_SRCS:.cpp=.o)

all: compile checkstyle test

compile: $(MAIN_BINARIES) $(TEST_BINARIES)

checkstyle:
	clang-format-18 --dry-run -Werror include/*.h src/*.cpp

test: $(TEST_BINARIES)
	for T in $(TEST_BINARIES); do ./$$T || exit; done

# Compile .cpp files from src/ to .o files in src/
src/%.o: src/%.cpp
	$(CXX) -c $< -o $@

# Link Main binaries
%Main: src/%Main.o $(OBJECTS)
	$(CXX) -o $@ $^ $(LIBS)

# Link Test binaries
%Test: src/%Test.o $(OBJECTS)
	$(CXX) -o $@ $^ $(LIBS) $(TESTLIBS)

clean:
	rm -f *Main *Test
	rm -f src/*.o

format:
	clang-format-18 -i src/*.cpp include/*.h
