CXX = g++
CXXFLAGS = -std=gnu++17 -Iinclude -Wall -g
TARGET = snip
SRC = $(wildcard src/*.cpp)

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
