SHELL := /bin/bash

ROOT := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
GODOT_SRC ?= $(ROOT)/godot
SPX_MODULE_SRC ?= $(ROOT)/godot_modules/spx
BUILD_DIR ?= $(ROOT)/.spx-build
PROJECT_DIR ?= $(ROOT)/debug_project
GDEXAMPLE_DIR ?= $(ROOT)/gdexample
PLATFORM ?= macos
ARCH ?= arm64
JOBS ?= $(shell sysctl -n hw.ncpu 2>/dev/null || getconf _NPROCESSORS_ONLN 2>/dev/null || echo 4)
SCONS ?= $(shell command -v scons 2>/dev/null || true)
GODOT_EXTRA_SUFFIX ?= spx-debug
GODOT_BIN := $(GODOT_SRC)/bin/godot.$(PLATFORM).editor.dev.$(ARCH).$(GODOT_EXTRA_SUFFIX)
COMPILE_DB := $(BUILD_DIR)/compile_commands.json
GDEXAMPLE_COMPILE_DB := $(BUILD_DIR)/gdexample-compile_commands.json
GDEXAMPLE_COMPILE_DB_PATCHER := $(GDEXAMPLE_DIR)/tools/patch_compile_db.py

SCONS_ARGS := \
	platform=$(PLATFORM) \
	arch=$(ARCH) \
	target=editor \
	dev_build=yes \
	debug_symbols=true \
	optimize=none \
	lto=none \
	vulkan=false \
	use_volk=no \
	openxr=false \
	tests=yes \
	compiledb=yes \
	custom_modules=$(SPX_MODULE_SRC) \
	custom_modules_recursive=false \
	extra_suffix=$(GODOT_EXTRA_SUFFIX)

.DEFAULT_GOAL := help
.PHONY: help doctor build-debug compile-db test-spx test-all run-debug clean

help:
	@printf '%s\n' \
		'Commands:' \
		'  make doctor       Check the local Godot, SPX module, and SCons paths' \
		'  make build-debug  Build the unoptimized Godot editor with SPX and symbols' \
		'  make compile-db  Generate Godot and gdexample compile databases' \
		'  make test-spx     Run SPX tests (TEST_CASE_FILTER=*SPX* can narrow them)' \
		'  make test-all     Run the complete Godot test suite' \
		'  make run-debug    Launch the blank debug project in the custom editor' \
		'  make clean        Remove only this workspace\'s generated build metadata'

doctor:
	@test -f "$(GODOT_SRC)/SConstruct" || { echo "Godot source not found: $(GODOT_SRC)" >&2; exit 1; }
	@test -f "$(SPX_MODULE_SRC)/SCsub" || { echo "SPX module not found: $(SPX_MODULE_SRC)" >&2; exit 1; }
	@test -n "$(SCONS)" || { echo 'SCons is not installed or not on PATH' >&2; exit 1; }
	@echo "Godot source: $(GODOT_SRC)"
	@echo "SPX module:   $(SPX_MODULE_SRC)"
	@echo "SCons:        $(SCONS)"
	@echo "Architecture: $(PLATFORM)/$(ARCH)"
	@echo "Debug binary: $(GODOT_BIN)"

build-debug: doctor
	@mkdir -p "$(BUILD_DIR)"
	@echo "Building Godot + SPX with symbols and optimize=none..."
	@cd "$(GODOT_SRC)" && "$(SCONS)" $(SCONS_ARGS) -j$(JOBS)
	@test -x "$(GODOT_BIN)" || { echo "Build completed without expected binary: $(GODOT_BIN)" >&2; exit 1; }
	@cp "$(GODOT_SRC)/compile_commands.json" "$(COMPILE_DB)"
	@cd "$(GDEXAMPLE_DIR)" && "$(SCONS)" compiledb=yes compiledb_file="$(GDEXAMPLE_COMPILE_DB)" compiledb
	@python3 "$(GDEXAMPLE_COMPILE_DB_PATCHER)" "$(GDEXAMPLE_COMPILE_DB)"
	@echo "Built: $(GODOT_BIN)"
	@echo "Godot compile DB: $(COMPILE_DB)"
	@echo "gdexample compile DB: $(GDEXAMPLE_COMPILE_DB)"

compile-db: build-debug

test-spx: build-debug
	@"$(GODOT_BIN)" --headless --path "$(PROJECT_DIR)" --test --test-case="$(or $(TEST_CASE_FILTER),*SPX*)"

test-all: build-debug
	@"$(GODOT_BIN)" --headless --path "$(PROJECT_DIR)" --test

run-debug: build-debug
	@"$(GODOT_BIN)" --editor --path "$(PROJECT_DIR)"

clean:
	@rm -rf "$(BUILD_DIR)"
	@echo "Removed $(BUILD_DIR)"
