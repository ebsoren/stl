# Compiler settings
CXX := clang++
CXXFLAGS := -std=c++20 -Wall -Wextra -Wpedantic -g
INCLUDES := -I include

# Directories
BUILD_DIR := build
TEST_DIR := tests
BENCH_DIR := benchmarks

# Find all source files
TEST_SRCS := $(wildcard $(TEST_DIR)/*.cpp)
BENCH_SRCS := $(wildcard $(BENCH_DIR)/*.cpp)

# Generate executable names
TEST_BINS := $(patsubst $(TEST_DIR)/%.cpp,$(BUILD_DIR)/tests/%,$(TEST_SRCS))
BENCH_BINS := $(patsubst $(BENCH_DIR)/%.cpp,$(BUILD_DIR)/benchmarks/%,$(BENCH_SRCS))

# Default target
.PHONY: all
all: tests

# Create build directories
$(BUILD_DIR)/tests $(BUILD_DIR)/benchmarks:
	mkdir -p $@

# Build tests
.PHONY: tests
tests: $(TEST_BINS)

$(BUILD_DIR)/tests/%: $(TEST_DIR)/%.cpp | $(BUILD_DIR)/tests
	$(CXX) $(CXXFLAGS) $(INCLUDES) $< -o $@

# Build benchmarks
.PHONY: benchmarks
benchmarks: $(BENCH_BINS)

$(BUILD_DIR)/benchmarks/%: $(BENCH_DIR)/%.cpp | $(BUILD_DIR)/benchmarks
	$(CXX) $(CXXFLAGS) -O3 $(INCLUDES) $< -o $@

# Run all tests
.PHONY: test
test: tests
	@echo "Running tests..."
	@for t in $(TEST_BINS); do \
		echo "\n=== Running $$t ==="; \
		./$$t || exit 1; \
	done
	@echo "\nAll tests passed!"

# Run benchmarks
.PHONY: bench
bench: benchmarks
	@echo "Running benchmarks..."
	@for b in $(BENCH_BINS); do \
		echo "\n=== Running $$b ==="; \
		./$$b; \
	done

# Format code
.PHONY: format
format:
	find include tests benchmarks -name "*.hpp" -o -name "*.cpp" | xargs clang-format -i

# Check formatting (doesn't modify files)
.PHONY: format-check
format-check:
	find include tests benchmarks -name "*.hpp" -o -name "*.cpp" | xargs clang-format --dry-run -Werror

# Clean build artifacts
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

# Help
.PHONY: help
help:
	@echo "Available targets:"
	@echo "  all          - Build tests (default)"
	@echo "  tests        - Build all test executables"
	@echo "  test         - Build and run all tests"
	@echo "  benchmarks   - Build all benchmarks"
	@echo "  bench        - Build and run all benchmarks"
	@echo "  format       - Format all source files with clang-format"
	@echo "  format-check - Check formatting without modifying files"
	@echo "  clean        - Remove build directory"
	@echo "  help         - Show this help"
