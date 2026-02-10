#include "../include/stl/memory.hpp"

#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <cstring>

using Clock = std::chrono::high_resolution_clock;

// Benchmark configuration
constexpr int SMALL_N = 10000;
constexpr int MEDIUM_N = 100000;
constexpr int LARGE_N = 1000000;
constexpr int ITERATIONS = 500;

template <typename Duration>
double to_ms(Duration d) {
    return std::chrono::duration<double, std::milli>(d).count();
}

// Non-trivial type for testing proper construction/destruction
struct NonTrivial {
    std::string data;
    int value;

    NonTrivial() : data("default"), value(0) {}
    NonTrivial(int v) : data("value"), value(v) {}
    NonTrivial(const NonTrivial& other) : data(other.data), value(other.value) {}
    NonTrivial(NonTrivial&& other) noexcept : data(std::move(other.data)), value(other.value) {}
    ~NonTrivial() { value = -1; }
};

// -----------------------------------------------------------------------------
// Benchmark: destroy (range, trivial)
// -----------------------------------------------------------------------------
void bench_destroy_trivial() {
    std::cout << "\n=== destroy range (int, N=" << LARGE_N << ") ===\n";

    // std::destroy
    double std_total = 0;
    for (int iter = 0; iter < ITERATIONS; iter++) {
        int* data = static_cast<int*>(malloc(LARGE_N * sizeof(int)));
        for (int i = 0; i < LARGE_N; i++) {
            new (data + i) int(i);
        }

        auto start = Clock::now();
        std::destroy(data, data + LARGE_N);
        auto end = Clock::now();
        std_total += to_ms(end - start);

        free(data);
    }

    // stl::destroy
    double stl_total = 0;
    for (int iter = 0; iter < ITERATIONS; iter++) {
        int* data = static_cast<int*>(malloc(LARGE_N * sizeof(int)));
        for (int i = 0; i < LARGE_N; i++) {
            new (data + i) int(i);
        }

        auto start = Clock::now();
        stl::destroy(data, data + LARGE_N);
        auto end = Clock::now();
        stl_total += to_ms(end - start);

        free(data);
    }

    std::cout << "  std::destroy: " << (std_total / ITERATIONS) << " ms\n";
    std::cout << "  stl::destroy: " << (stl_total / ITERATIONS) << " ms\n";
    std::cout << "  ratio:        " << (stl_total / std_total) << "x\n";
}

// -----------------------------------------------------------------------------
// Benchmark: destroy (range, non-trivial)
// -----------------------------------------------------------------------------
void bench_destroy_nontrivial() {
    std::cout << "\n=== destroy range (NonTrivial, N=" << MEDIUM_N << ") ===\n";

    // std::destroy
    double std_total = 0;
    for (int iter = 0; iter < ITERATIONS; iter++) {
        NonTrivial* data = static_cast<NonTrivial*>(malloc(MEDIUM_N * sizeof(NonTrivial)));
        for (int i = 0; i < MEDIUM_N; i++) {
            new (data + i) NonTrivial(i);
        }

        auto start = Clock::now();
        std::destroy(data, data + MEDIUM_N);
        auto end = Clock::now();
        std_total += to_ms(end - start);

        free(data);
    }

    // stl::destroy
    double stl_total = 0;
    for (int iter = 0; iter < ITERATIONS; iter++) {
        NonTrivial* data = static_cast<NonTrivial*>(malloc(MEDIUM_N * sizeof(NonTrivial)));
        for (int i = 0; i < MEDIUM_N; i++) {
            new (data + i) NonTrivial(i);
        }

        auto start = Clock::now();
        stl::destroy(data, data + MEDIUM_N);
        auto end = Clock::now();
        stl_total += to_ms(end - start);

        free(data);
    }

    std::cout << "  std::destroy: " << (std_total / ITERATIONS) << " ms\n";
    std::cout << "  stl::destroy: " << (stl_total / ITERATIONS) << " ms\n";
    std::cout << "  ratio:        " << (stl_total / std_total) << "x\n";
}

// -----------------------------------------------------------------------------
// Benchmark: construct (fill, trivial)
// -----------------------------------------------------------------------------
void bench_construct_fill_trivial() {
    std::cout << "\n=== construct fill (int, N=" << LARGE_N << ") ===\n";

    // std::uninitialized_fill
    double std_total = 0;
    for (int iter = 0; iter < ITERATIONS; iter++) {
        int* data = static_cast<int*>(malloc(LARGE_N * sizeof(int)));

        auto start = Clock::now();
        std::uninitialized_fill(data, data + LARGE_N, 42);
        auto end = Clock::now();
        std_total += to_ms(end - start);

        std::destroy(data, data + LARGE_N);
        free(data);
    }

    // stl::construct (fill)
    double stl_total = 0;
    for (int iter = 0; iter < ITERATIONS; iter++) {
        int* data = static_cast<int*>(malloc(LARGE_N * sizeof(int)));

        auto start = Clock::now();
        stl::fill(data, data + LARGE_N, 42);
        auto end = Clock::now();
        stl_total += to_ms(end - start);

        stl::destroy(data, data + LARGE_N);
        free(data);
    }

    std::cout << "  std::uninitialized_fill: " << (std_total / ITERATIONS) << " ms\n";
    std::cout << "  stl::construct:          " << (stl_total / ITERATIONS) << " ms\n";
    std::cout << "  ratio:                   " << (stl_total / std_total) << "x\n";
}

// -----------------------------------------------------------------------------
// Benchmark: construct (fill, non-trivial)
// -----------------------------------------------------------------------------
void bench_construct_fill_nontrivial() {
    std::cout << "\n=== construct fill (NonTrivial, N=" << MEDIUM_N << ") ===\n";

    NonTrivial fill_val(99);

    // std::uninitialized_fill
    double std_total = 0;
    for (int iter = 0; iter < ITERATIONS; iter++) {
        NonTrivial* data = static_cast<NonTrivial*>(malloc(MEDIUM_N * sizeof(NonTrivial)));

        auto start = Clock::now();
        std::uninitialized_fill(data, data + MEDIUM_N, fill_val);
        auto end = Clock::now();
        std_total += to_ms(end - start);

        std::destroy(data, data + MEDIUM_N);
        free(data);
    }

    // stl::construct (fill)
    double stl_total = 0;
    for (int iter = 0; iter < ITERATIONS; iter++) {
        NonTrivial* data = static_cast<NonTrivial*>(malloc(MEDIUM_N * sizeof(NonTrivial)));

        auto start = Clock::now();
        stl::fill(data, data + MEDIUM_N, fill_val);
        auto end = Clock::now();
        stl_total += to_ms(end - start);

        stl::destroy(data, data + MEDIUM_N);
        free(data);
    }

    std::cout << "  std::uninitialized_fill: " << (std_total / ITERATIONS) << " ms\n";
    std::cout << "  stl::construct:          " << (stl_total / ITERATIONS) << " ms\n";
    std::cout << "  ratio:                   " << (stl_total / std_total) << "x\n";
}

// -----------------------------------------------------------------------------
// Benchmark: construct (copy, trivial)
// -----------------------------------------------------------------------------
void bench_construct_copy_trivial() {
    std::cout << "\n=== construct copy (int, N=" << LARGE_N << ") ===\n";

    // Setup source
    int* src = static_cast<int*>(malloc(LARGE_N * sizeof(int)));
    for (int i = 0; i < LARGE_N; i++) {
        new (src + i) int(i);
    }

    // std::uninitialized_copy
    double std_total = 0;
    for (int iter = 0; iter < ITERATIONS; iter++) {
        int* dest = static_cast<int*>(malloc(LARGE_N * sizeof(int)));

        auto start = Clock::now();
        std::uninitialized_copy(src, src + LARGE_N, dest);
        auto end = Clock::now();
        std_total += to_ms(end - start);

        std::destroy(dest, dest + LARGE_N);
        free(dest);
    }

    // stl::construct (copy)
    double stl_total = 0;
    for (int iter = 0; iter < ITERATIONS; iter++) {
        int* dest = static_cast<int*>(malloc(LARGE_N * sizeof(int)));

        auto start = Clock::now();
        stl::construct(dest, src, LARGE_N);
        auto end = Clock::now();
        stl_total += to_ms(end - start);

        stl::destroy(dest, dest + LARGE_N);
        free(dest);
    }

    std::destroy(src, src + LARGE_N);
    free(src);

    std::cout << "  std::uninitialized_copy: " << (std_total / ITERATIONS) << " ms\n";
    std::cout << "  stl::construct:          " << (stl_total / ITERATIONS) << " ms\n";
    std::cout << "  ratio:                   " << (stl_total / std_total) << "x\n";
}

// -----------------------------------------------------------------------------
// Benchmark: construct (copy, non-trivial)
// -----------------------------------------------------------------------------
void bench_construct_copy_nontrivial() {
    std::cout << "\n=== construct copy (NonTrivial, N=" << MEDIUM_N << ") ===\n";

    // Setup source
    NonTrivial* src = static_cast<NonTrivial*>(malloc(MEDIUM_N * sizeof(NonTrivial)));
    for (int i = 0; i < MEDIUM_N; i++) {
        new (src + i) NonTrivial(i);
    }

    // std::uninitialized_copy
    double std_total = 0;
    for (int iter = 0; iter < ITERATIONS; iter++) {
        NonTrivial* dest = static_cast<NonTrivial*>(malloc(MEDIUM_N * sizeof(NonTrivial)));

        auto start = Clock::now();
        std::uninitialized_copy(src, src + MEDIUM_N, dest);
        auto end = Clock::now();
        std_total += to_ms(end - start);

        std::destroy(dest, dest + MEDIUM_N);
        free(dest);
    }

    // stl::construct (copy)
    double stl_total = 0;
    for (int iter = 0; iter < ITERATIONS; iter++) {
        NonTrivial* dest = static_cast<NonTrivial*>(malloc(MEDIUM_N * sizeof(NonTrivial)));

        auto start = Clock::now();
        stl::construct(dest, src, MEDIUM_N);
        auto end = Clock::now();
        stl_total += to_ms(end - start);

        stl::destroy(dest, dest + MEDIUM_N);
        free(dest);
    }

    std::destroy(src, src + MEDIUM_N);
    free(src);

    std::cout << "  std::uninitialized_copy: " << (std_total / ITERATIONS) << " ms\n";
    std::cout << "  stl::construct:          " << (stl_total / ITERATIONS) << " ms\n";
    std::cout << "  ratio:                   " << (stl_total / std_total) << "x\n";
}

// -----------------------------------------------------------------------------
// Benchmark: destroy_at (single element, non-trivial)
// -----------------------------------------------------------------------------
void bench_destroy_at() {
    std::cout << "\n=== destroy_at (NonTrivial, N=" << LARGE_N << ") ===\n";

    // std::destroy_at
    double std_total = 0;
    for (int iter = 0; iter < ITERATIONS; iter++) {
        NonTrivial* data = static_cast<NonTrivial*>(malloc(LARGE_N * sizeof(NonTrivial)));
        for (int i = 0; i < LARGE_N; i++) {
            new (data + i) NonTrivial(i);
        }

        auto start = Clock::now();
        for (int i = 0; i < LARGE_N; i++) {
            std::destroy_at(data + i);
        }
        auto end = Clock::now();
        std_total += to_ms(end - start);

        free(data);
    }

    // stl::destroy_at
    double stl_total = 0;
    for (int iter = 0; iter < ITERATIONS; iter++) {
        NonTrivial* data = static_cast<NonTrivial*>(malloc(LARGE_N * sizeof(NonTrivial)));
        for (int i = 0; i < LARGE_N; i++) {
            new (data + i) NonTrivial(i);
        }

        auto start = Clock::now();
        for (int i = 0; i < LARGE_N; i++) {
            stl::destroy_at(data + i);
        }
        auto end = Clock::now();
        stl_total += to_ms(end - start);

        free(data);
    }

    std::cout << "  std::destroy_at: " << (std_total / ITERATIONS) << " ms\n";
    std::cout << "  stl::destroy_at: " << (stl_total / ITERATIONS) << " ms\n";
    std::cout << "  ratio:           " << (stl_total / std_total) << "x\n";
}

// -----------------------------------------------------------------------------
// Benchmark: construct_at (single element, non-trivial)
// -----------------------------------------------------------------------------
void bench_construct_at() {
    std::cout << "\n=== construct_at (NonTrivial, N=" << LARGE_N << ") ===\n";

    NonTrivial val(42);

    // std::construct_at (C++20) - use placement new for compatibility
    double std_total = 0;
    for (int iter = 0; iter < ITERATIONS; iter++) {
        NonTrivial* data = static_cast<NonTrivial*>(malloc(LARGE_N * sizeof(NonTrivial)));

        auto start = Clock::now();
        for (int i = 0; i < LARGE_N; i++) {
            new (data + i) NonTrivial(val);
        }
        auto end = Clock::now();
        std_total += to_ms(end - start);

        std::destroy(data, data + LARGE_N);
        free(data);
    }

    // stl::construct_at
    double stl_total = 0;
    for (int iter = 0; iter < ITERATIONS; iter++) {
        NonTrivial* data = static_cast<NonTrivial*>(malloc(LARGE_N * sizeof(NonTrivial)));

        auto start = Clock::now();
        for (int i = 0; i < LARGE_N; i++) {
            stl::construct_at(data + i, val);
        }
        auto end = Clock::now();
        stl_total += to_ms(end - start);

        stl::destroy(data, data + LARGE_N);
        free(data);
    }

    std::cout << "  placement new:       " << (std_total / ITERATIONS) << " ms\n";
    std::cout << "  stl::construct_at:   " << (stl_total / ITERATIONS) << " ms\n";
    std::cout << "  ratio:               " << (stl_total / std_total) << "x\n";
}

int main() {
    std::cout << "Memory Benchmark: stl::memory vs std::memory\n";
    std::cout << "=============================================\n";
    std::cout << "Iterations per benchmark: " << ITERATIONS << "\n";

    bench_destroy_trivial();
    bench_destroy_nontrivial();
    bench_construct_fill_trivial();
    bench_construct_fill_nontrivial();
    bench_construct_copy_trivial();
    bench_construct_copy_nontrivial();
    bench_destroy_at();
    bench_construct_at();

    std::cout << "\n=============================================\n";
    std::cout << "Benchmark complete.\n";
    std::cout << "Ratio < 1.0 means stl:: is faster\n";
    std::cout << "Ratio > 1.0 means std:: is faster\n";

    return 0;
}
