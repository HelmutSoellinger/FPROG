# Compiler
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

# Target executable name
TARGET = red_black_tree

# Source file
SRCS = Red-Black-Tree.cpp

# Default target: build the executable
all: $(TARGET)

# Compile the program
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Clean up build files
clean:
	rm -f $(TARGET)

# Phony targets
.PHONY: all clean
