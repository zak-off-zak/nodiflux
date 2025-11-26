BUILD_DIR := build
EXECUTABLE := $(BUILD_DIR)/test_main

all: build run

configure:
	@echo "[BUILD]: Configuring CMake"
	cmake -B $(BUILD_DIR) -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

build: configure
	@echo "[BUILD]: Building via CMake"
	cmake --build $(BUILD_DIR)

run: build
	@echo "[RUN]: Starting executable"
	@$(EXECUTABLE)

test: build
	@echo "[TEST]: Running Google Tests"
	@cd $(BUILD_DIR) && ctest -V

clean:
	@echo "[BUILD]: Cleaning build directory"
	rm -rf $(BUILD_DIR)

.PHONY: all configure build run test clean
