# ==============================================================================
# cubeforge.mega-modpack Makefile
# ==============================================================================

BUILD_DIR ?= build
CONFIG ?= Release
DIST_DIR ?= dist

.PHONY: all build test clean dist help

# Default target: build the DLL and copy to dist folder
all: build dist

# Configure CMake if build directory does not exist
$(BUILD_DIR)/CMakeCache.txt:
	@cmake -B $(BUILD_DIR) -A x64

# Compile the mod DLL and binaries in Release mode
build: $(BUILD_DIR)/CMakeCache.txt
	@cmake --build $(BUILD_DIR) --config $(CONFIG)

# Run the automated test suite
test: build
	@ctest --test-dir $(BUILD_DIR) -C $(CONFIG) --output-on-failure

# Package / copy the generated DLL into the dist directory
dist: build
	@powershell -NoProfile -Command "New-Item -ItemType Directory -Force -Path '$(DIST_DIR)' | Out-Null; Copy-Item -Force '$(BUILD_DIR)/$(CONFIG)/*.dll' '$(DIST_DIR)/'; Write-Host '✅ Mod DLL copiada com sucesso para $(DIST_DIR)/' -ForegroundColor Green"

# Clean build artifacts
clean:
	@powershell -NoProfile -Command "if (Test-Path '$(BUILD_DIR)') { Remove-Item -Recurse -Force '$(BUILD_DIR)' }; if (Test-Path '$(DIST_DIR)') { Remove-Item -Recurse -Force '$(DIST_DIR)' }; Write-Host '🧹 Diretórios de build e dist limpos.' -ForegroundColor Yellow"

# Display help information
help:
	@echo "================================================================="
	@echo " cubeforge.mega-modpack - Build Automation"
	@echo "================================================================="
	@echo " make          - Compila o mod DLL e copia para dist/"
	@echo " make build    - Compila os binarios (Release)"
	@echo " make test     - Executa todos os testes unitarios"
	@echo " make dist     - Cria a pasta dist/ com o DLL pronto"
	@echo " make clean    - Remove as pastas build/ e dist/"
	@echo "================================================================="
