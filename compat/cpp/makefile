# Frozen reference implementation, not used by the Rust build or runtime.
CXX ?= g++
CXXFLAGS ?= -std=c++17 -D_GLIBCXX_ASSERTIONS
SOURCES := $(wildcard src/*.cpp src/*/*.cpp src/*/*/*.cpp)
OBJECTS := $(patsubst src/%.cpp,../../build/reference/%.o,$(SOURCES))
TARGET := ../../build/reference/albat
INCLUDES := -Isrc -Isrc/Albat -Isrc/Library -Isrc/Utils

all: $(TARGET)
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^
../../build/reference/%.o: src/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@
-include $(OBJECTS:.o=.d)
.PHONY: all
