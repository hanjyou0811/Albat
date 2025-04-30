CXX = g++
CXXFLAGS = 

TARGET = albat
BIN_DIR = bin

SRC_DIR = src
BUILD_DIR = build
SRC_SUBDIRS = $(SRC_DIR) $(SRC_DIR)/Albat $(SRC_DIR)/Albat/src $(SRC_DIR)/Albat/utils $(SRC_DIR)/Albat/parse $(SRC_DIR)/Library $(SRC_DIR)/Library/src $(SRC_DIR)/Utils

SRCS = $(foreach dir,$(SRC_SUBDIRS),$(wildcard $(dir)/*.cpp))

OBJS = $(patsubst $(SRC_DIR)/%,$(BUILD_DIR)/%,$(patsubst %.cpp,%.o,$(SRCS)))

BUILD_SUBDIRS = $(sort $(dir $(OBJS)))

INCLUDES = -I$(SRC_DIR) -I$(SRC_DIR)/Albat -I$(SRC_DIR)/Library -I$(SRC_DIR)/Utils

all: $(BIN_DIR)/$(TARGET)

$(BIN_DIR)/$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

directories: $(BUILD_SUBDIRS) $(BIN_DIR)
		
$(BUILD_SUBDIRS) $(BIN_DIR):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

-include $(OBJS:.o=.d)

.PHONY: all clean directories