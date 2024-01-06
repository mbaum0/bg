SRC_DIR = src
INC_DIR = include
LIB_DIR = lib
BUILD_DIR = build
OBJ_NAME = bg
BUILD_TARGET = $(BUILD_DIR)/$(OBJ_NAME)

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
HEADER_FILES = $(wildcard $(INC_DIR)/*.h)
LIB_FILES = $(wildcard $(LIB_DIR)/*.c)

build: $(BUILD_TARGET)

$(BUILD_TARGET): $(SRC_FILES) $(HEADER_FILES) $(LIB_FILES)
	mkdir -p $(BUILD_DIR)
	cmake -B build -S .
	make -C build

clean:
	rm -rf $(BUILD_DIR)

leaks: $(BUILD_TARGET)
	MallocStackLogging=1 leaks -quiet --atExit -- ./$(BUILD_TARGET)

run: $(BUILD_TARGET)
	./$(BUILD_TARGET)
