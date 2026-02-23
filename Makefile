CXX = g++
CXXFLAGS = -std=gnu++17 -Iinclude -Wall -g -MMD -MP
TARGET = snip

SRC = $(wildcard src/*.cpp)

OBJ = $(SRC:src/%.cpp=build/%.o)

DEP = $(OBJ:.o=.d)

all: prepare $(TARGET)

prepare:
	@mkdir -p build

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

build/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf build $(TARGET)

-include $(DEP)