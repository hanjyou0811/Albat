CXX = g++
EMCC = em++

CXX_FLAGS =
WASM_FLAGS = -O3 \
	-s USE_WEBGL2=1 \
	-s WASM=1 \
	-s ALLOW_MEMORY_GROWTH=1 \
	-s DEMANGLE_SUPPORT=1 \
	-s EXPORTED_FUNCTIONS='["_main"]' \
	-s EXPORTED_RUNTIME_METHODS='["ccall","cwrap"]' \
	-s MINIFY_HTML=0

TARGET = albat
BIN_DIR = bin
SRC_DIR = src

BUILD_DIR_NATIVE = build/native
BUILD_DIR_WASM   = build/wasm

SRC_SUBDIRS = $(SRC_DIR) \
	$(SRC_DIR)/Albat \
	$(SRC_DIR)/Albat/src \
	$(SRC_DIR)/Albat/utils \
	$(SRC_DIR)/Albat/parse \
	$(SRC_DIR)/Library \
	$(SRC_DIR)/Library/src \
	$(SRC_DIR)/Utils

SRCS = $(foreach dir,$(SRC_SUBDIRS),$(wildcard $(dir)/*.cpp))

INCLUDES = -I$(SRC_DIR) -I$(SRC_DIR)/Albat -I$(SRC_DIR)/Library -I$(SRC_DIR)/Utils

# ===== Native =====

NATIVE_OBJS = $(patsubst $(SRC_DIR)/%,$(BUILD_DIR_NATIVE)/%,$(patsubst %.cpp,%.o,$(SRCS)))

all: native

native: CXX := g++
native: FLAGS := $(CXX_FLAGS)
native: $(BIN_DIR)/$(TARGET)

$(BIN_DIR)/$(TARGET): $(NATIVE_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(FLAGS) -o $@ $^

$(BUILD_DIR_NATIVE)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(FLAGS) $(INCLUDES) -c $< -o $@

# ===== WASM =====

WASM_OBJS = $(patsubst $(SRC_DIR)/%,$(BUILD_DIR_WASM)/%,$(patsubst %.cpp,%.o,$(SRCS)))

wasm: CXX := em++
wasm: FLAGS := $(WASM_FLAGS)
wasm: $(BIN_DIR)/$(TARGET).js

$(BIN_DIR)/$(TARGET).js: $(WASM_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(FLAGS) $(INCLUDES) -o $@ $^

$(BUILD_DIR_WASM)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(FLAGS) $(INCLUDES) -c $< -o $@

# ===== Clean =====
clean:
	rm -rf build $(BIN_DIR)

.PHONY: all clean native wasm
