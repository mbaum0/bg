SRC_DIR = src
INC_DIR = include
REPORT_DIR = report
LIB_DIR = lib
BUILD_DIR = build
OBJ_NAME = bg
BUILD_TARGET = $(BUILD_DIR)/$(OBJ_NAME)
FONT_FILE=assets/NotoSans-Medium.ttf
FONT_TARGET=assets/NotoSans-Medium

SRC_FILES = $(shell find $(SRC_DIR) -type f \( -name '*.c' \))
HEADER_FILES = $(wildcard $(INC_DIR)/*.h)
LIB_SRC_FILES = $(shell find $(LIB_DIR) -name '*.c')
LIB_HEADER_FILES = $(shell find $(LIB_DIR) -name '*.h')

build: $(BUILD_TARGET)

$(BUILD_TARGET): $(SRC_FILES) $(HEADER_FILES) $(LIB_SRC_FILES) $(LIB_HEADER_FILES)
	mkdir -p $(BUILD_DIR)
	cmake -B build -S .
	make -C build

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(REPORT_DIR)

leaks: $(BUILD_TARGET)
	MallocStackLogging=1 leaks -quiet --atExit -- ./$(BUILD_TARGET)

format: $(SRC_FILES) $(HEADER_FILES) $(LIB_SRC_FILES) $(LIB_HEADER_FILES)
	find $(SRC_DIR) $(INC_DIR) $(LIB_DIR) -type f \( -name "*.c" -o -name "*.h" \) -exec clang-format -i {} -style=file \;

static:
	make clean
	/opt/homebrew/opt/llvm/bin/scan-build -o report "make build"

run: $(BUILD_TARGET)
	./$(BUILD_TARGET)

font:
	fontbm --font-file $(FONT_FILE) --font-size 40 --output $(FONT_TARGET) --data-format bin