CXX = g++
CXXFLAGS = -std=gnu++17 -Iinclude -Wall -g
TARGET = snip

SRC = $(shell find include -name "*.cpp")
HDR = $(shell find include -name "*.hpp")

# The target now depends on both CPP and HPP files
$(TARGET): $(SRC) $(HDR)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

all: $(TARGET)

clean:
	rm -f $(TARGET)