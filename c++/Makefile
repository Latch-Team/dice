CXX      ?= clang++
CXXFLAGS ?= -std=c++20 -O2 -Wall -Wextra -Wpedantic
INCLUDES := -Ivendor

SRCS := bench.cpp dice_baseline.cpp dice_fast.cpp
BIN  := bench

JSON_HPP    := vendor/nlohmann/json.hpp
JSON_URL    := https://github.com/nlohmann/json/releases/download/v3.11.3/json.hpp

.PHONY: all run clean

all: $(BIN)

$(BIN): $(SRCS) dice_baseline.h dice_fast.h $(JSON_HPP)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SRCS) -o $(BIN)

$(JSON_HPP):
	@mkdir -p $(dir $(JSON_HPP))
	@echo "Downloading nlohmann/json single-header..."
	@curl -sSfL $(JSON_URL) -o $(JSON_HPP)

run: $(BIN)
	./$(BIN)

clean:
	rm -f $(BIN)
