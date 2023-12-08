SRC_DIR = src
INC_DIR = include
BUILD_DIR = build
OBJ_NAME = bg
BUILD_TARGET = $(BUILD_DIR)/$(OBJ_NAME)

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
HEADER_FILES = $(wildcard $(HEADERS_DIR)/*.h)

build: $(BUILD_TARGET)
	mkdir -p $(BUILD_DIR)
	cmake -B build -S .


$(BUILD_TARGET): $(SRC_FILES) $(HEADER_FILES)
	./build.sh

clean:
	rm -rf $(BUILD_DIR)

suppress: $(BUILD_TARGET)
	rm memcheck.log
	valgrind --leak-check=full --show-reachable=yes --error-limit=no --gen-suppressions=all --suppressions=sdl.supp -v --log-file=memcheck.log ./$(BUILD_TARGET)

leaks: $(BUILD_TARGET)
	valgrind --leak-check=full --show-reachable=yes --show-leak-kinds=all --suppressions=sdl.supp ./$(BUILD_TARGET)

docker:
	docker build -t valgrind .
	docker run -it -v .:/valgrind -e DISPLAY=host.docker.internal:0 valgrind:latest make leaks