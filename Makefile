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

prun:
	@echo "[PLATFROM_IO]: Creating an image"
	pio run

pup:
	@echo "[PLATFROM_IO]: Uploading to the target"
	pio run -t upload

pm:
	@echo "[PLATFROM_IO]: Monitor"
	pio device monitor

.PHONY: all configure build run test
