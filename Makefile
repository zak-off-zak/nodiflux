BUILD_DIR := build
EXECUTABLE := $(BUILD_DIR)/test_main
NATIVE_ENV := native
ESP_ENV := nodemcu-32s

all: build run

configure:
	@echo "[BUILD]: Configuring PlatformIO + clangd"
	pio init --ide vim
	pio run -t compiledb

build: configure
	@echo "[BUILD]: Building via CMake"
	cmake --build $(BUILD_DIR)

run: build
	@echo "[RUN]: Starting executable"
	@$(EXECUTABLE)

clean:
	@echo "[BUILD]: Cleaning build directory"
	rm -rf $(BUILD_DIR)

prun:
	@echo "[PLATFROM_IO]: Creating an image"
	pio run -e $(ESP_ENV)

ptest: 
	@echo "[TEST]: Running Google Tests"
	pio test -e $(NATIVE_ENV)

pup:
	@echo "[PLATFROM_IO]: Uploading to the target"
	pio run -e $(ESP_ENV) -t upload

pm:
	@echo "[PLATFROM_IO]: Monitor"
	pio device monitor -b 115200

.PHONY: all configure build run test
