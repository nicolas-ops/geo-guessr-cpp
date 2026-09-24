.SUFFIXES:
.PRECIOUS: %.o
.PHONY: all compile checkstyle test clean format

CXX = clang++-18 -std=c++17 -g -Wall -Wextra -Wdeprecated -fsanitize=address -I/usr/include/freetype2 -Iinclude
MAIN_BINARIES = $(basename $(wildcard *Main.cpp))
TEST_BINARIES = $(basename $(wildcard *Test.cpp))
LIBS = -lncursesw

TESTLIBS = -lgtest -lgtest_main -lpthread
OBJECTS = $(addsuffix .o, $(basename $(filter-out %Main.cpp %Test.cpp, $(wildcard *.cpp))))

all: compile checkstyle test

compile: $(MAIN_BINARIES) $(TEST_BINARIES)

checkstyle:
	clang-format-18 --dry-run -Werror include/*.h *.cpp

test: $(TEST_BINARIES)
	for T in $(TEST_BINARIES); do ./$$T || exit; done

%.o: %.cpp include/*.h
	$(CXX) -c $<

%Main: %Main.o $(OBJECTS)
	$(CXX) -o $@ $^ $(LIBS)

%Test: %Test.o $(OBJECTS)
	$(CXX) -o $@ $^ $(LIBS) $(TESTLIBS)

clean:
	rm -f *Main
	rm -f *Test
	rm -f *.o

format:
	clang-format-18 -i *.cpp include/*.h
