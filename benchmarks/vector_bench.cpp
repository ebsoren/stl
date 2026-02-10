#include "../include/stl/vector.hpp"

#include <chrono>
#include <iostream>
#include <string>
#include <vector>

using Clock = std::chrono::high_resolution_clock;
using Ms = std::chrono::milliseconds;
using Us = std::chrono::microseconds;

// Benchmark configuration
constexpr int SMALL_N = 10000;
constexpr int MEDIUM_N = 100000;
constexpr int LARGE_N = 1000000;
constexpr int ITERATIONS = 500;

template <typename Duration>
double to_ms(Duration d) {
    return std::chrono::duration<double, std::milli>(d).count();
}

// -----------------------------------------------------------------------------
// Benchmark: push_back (int)
// -----------------------------------------------------------------------------
void bench_push_back_int() {
    std::cout << "\n=== push_back (int, N=" << LARGE_N << ") ===\n";

    // std::vector
    double std_total = 0;
    for (int iter = 0; iter < ITERATIONS; iter++) {
        std::vector<int> v;
        auto start = Clock::now();
        for (int i = 0; i < LARGE_N; i++) {
            v.push_back(i);
        }
        auto end = Clock::now();
        std_total += to_ms(end - start);
    }

    // stl::vector
    double stl_total = 0;
    for (int iter = 0; iter < ITERATIONS; iter++) {
        stl::vector<int> v;
        auto start = Clock::now();
        for (int i = 0; i < LARGE_N; i++) {
            v.push_back(i);
        }
        auto end = Clock::now();
        stl_total += to_ms(end - start);
    }

    std::cout << "  std::vector: " << (std_total / ITERATIONS) << " ms\n";
    std::cout << "  stl::vector: " << (stl_total / ITERATIONS) << " ms\n";
    std::cout << "  ratio:       " << (stl_total / std_total) << "x\n";
}

// -----------------------------------------------------------------------------
// Benchmark: push_back with reserve (int)
// -----------------------------------------------------------------------------
void bench_push_back_reserved_int() {
    std::cout << "\n=== push_back with reserve (int, N=" << LARGE_N << ") ===\n";

    // std::vector
    double std_total = 0;
    for (int iter = 0; iter < ITERATIONS; iter++) {
        std::vector<int> v;
        v.reserve(LARGE_N);
        auto start = Clock::now();
        for (int i = 0; i < LARGE_N; i++) {
            v.push_back(i);
        }
        auto end = Clock::now();
        std_total += to_ms(end - start);
    }

    // stl::vector
    double stl_total = 0;
    for (int iter = 0; iter < ITERATIONS; iter++) {
        stl::vector<int> v;
        v.reserve(LARGE_N);
        auto start = Clock::now();
        for (int i = 0; i < LARGE_N; i++) {
            v.push_back(i);
        }
        auto end = Clock::now();
        stl_total += to_ms(end - start);
    }

    std::cout << "  std::vector: " << (std_total / ITERATIONS) << " ms\n";
    std::cout << "  stl::vector: " << (stl_total / ITERATIONS) << " ms\n";
    std::cout << "  ratio:       " << (stl_total / std_total) << "x\n";
}

// -----------------------------------------------------------------------------
// Benchmark: push_back (string)
// -----------------------------------------------------------------------------
void bench_push_back_string() {
    std::cout << "\n=== push_back (string, N=" << MEDIUM_N << ") ===\n";

    std::string test_str = "hello world test string";

    // std::vector
    double std_total = 0;
    for (int iter = 0; iter < ITERATIONS; iter++) {
        std::vector<std::string> v;
        auto start = Clock::now();
        for (int i = 0; i < MEDIUM_N; i++) {
            v.push_back(test_str);
        }
        auto end = Clock::now();
        std_total += to_ms(end - start);
    }

    // stl::vector
    double stl_total = 0;
    for (int iter = 0; iter < ITERATIONS; iter++) {
        stl::vector<std::string> v;
        auto start = Clock::now();
        for (int i = 0; i < MEDIUM_N; i++) {
            v.push_back(test_str);
        }
        auto end = Clock::now();
        stl_total += to_ms(end - start);
    }

    std::cout << "  std::vector: " << (std_total / ITERATIONS) << " ms\n";
    std::cout << "  stl::vector: " << (stl_total / ITERATIONS) << " ms\n";
    std::cout << "  ratio:       " << (stl_total / std_total) << "x\n";
}

// -----------------------------------------------------------------------------
// Benchmark: iteration
// -----------------------------------------------------------------------------
void bench_iteration() {
    std::cout << "\n=== iteration (int, N=" << LARGE_N << ") ===\n";

    // Setup
    std::vector<int> std_v;
    stl::vector<int> stl_v;
    for (int i = 0; i < LARGE_N; i++) {
        std_v.push_back(i);
        stl_v.push_back(i);
    }

    // std::vector
    double std_total = 0;
    volatile long sum = 0;
    for (int iter = 0; iter < ITERATIONS; iter++) {
        auto start = Clock::now();
        for (int x : std_v) {
            sum += x;
        }
        auto end = Clock::now();
        std_total += to_ms(end - start);
    }

    // stl::vector
    double stl_total = 0;
    for (int iter = 0; iter < ITERATIONS; iter++) {
        auto start = Clock::now();
        for (int x : stl_v) {
            sum += x;
        }
        auto end = Clock::now();
        stl_total += to_ms(end - start);
    }

    std::cout << "  std::vector: " << (std_total / ITERATIONS) << " ms\n";
    std::cout << "  stl::vector: " << (stl_total / ITERATIONS) << " ms\n";
    std::cout << "  ratio:       " << (stl_total / std_total) << "x\n";
    (void)sum;  // suppress unused warning
}

// -----------------------------------------------------------------------------
// Benchmark: random access
// -----------------------------------------------------------------------------
void bench_random_access() {
    std::cout << "\n=== random access (int, N=" << LARGE_N << ") ===\n";

    // Setup
    std::vector<int> std_v;
    stl::vector<int> stl_v;
    for (int i = 0; i < LARGE_N; i++) {
        std_v.push_back(i);
        stl_v.push_back(i);
    }

    // Generate random indices
    std::vector<size_t> indices;
    for (int i = 0; i < LARGE_N; i++) {
        indices.push_back((i * 7919) % LARGE_N);  // pseudo-random
    }

    // std::vector
    double std_total = 0;
    volatile long sum = 0;
    for (int iter = 0; iter < ITERATIONS; iter++) {
        auto start = Clock::now();
        for (size_t idx : indices) {
            sum += std_v[idx];
        }
        auto end = Clock::now();
        std_total += to_ms(end - start);
    }

    // stl::vector
    double stl_total = 0;
    for (int iter = 0; iter < ITERATIONS; iter++) {
        auto start = Clock::now();
        for (size_t idx : indices) {
            sum += stl_v[idx];
        }
        auto end = Clock::now();
        stl_total += to_ms(end - start);
    }

    std::cout << "  std::vector: " << (std_total / ITERATIONS) << " ms\n";
    std::cout << "  stl::vector: " << (stl_total / ITERATIONS) << " ms\n";
    std::cout << "  ratio:       " << (stl_total / std_total) << "x\n";
    (void)sum;
}

// -----------------------------------------------------------------------------
// Benchmark: copy
// -----------------------------------------------------------------------------
void bench_copy() {
    std::cout << "\n=== copy (int, N=" << LARGE_N << ") ===\n";

    // Setup
    std::vector<int> std_src;
    stl::vector<int> stl_src;
    for (int i = 0; i < LARGE_N; i++) {
        std_src.push_back(i);
        stl_src.push_back(i);
    }

    // std::vector
    double std_total = 0;
    for (int iter = 0; iter < ITERATIONS; iter++) {
        auto start = Clock::now();
        std::vector<int> copy(std_src);
        auto end = Clock::now();
        std_total += to_ms(end - start);
        (void)copy;
    }

    // stl::vector
    double stl_total = 0;
    for (int iter = 0; iter < ITERATIONS; iter++) {
        auto start = Clock::now();
        stl::vector<int> copy(stl_src);
        auto end = Clock::now();
        stl_total += to_ms(end - start);
        (void)copy;
    }

    std::cout << "  std::vector: " << (std_total / ITERATIONS) << " ms\n";
    std::cout << "  stl::vector: " << (stl_total / ITERATIONS) << " ms\n";
    std::cout << "  ratio:       " << (stl_total / std_total) << "x\n";
}

// -----------------------------------------------------------------------------
// Benchmark: clear and refill
// -----------------------------------------------------------------------------
void bench_clear_refill() {
    std::cout << "\n=== clear and refill (int, N=" << MEDIUM_N << ") ===\n";

    // std::vector
    double std_total = 0;
    for (int iter = 0; iter < ITERATIONS; iter++) {
        std::vector<int> v;
        auto start = Clock::now();
        for (int round = 0; round < 10; round++) {
            for (int i = 0; i < MEDIUM_N; i++) {
                v.push_back(i);
            }
            v.clear();
        }
        auto end = Clock::now();
        std_total += to_ms(end - start);
    }

    // stl::vector
    double stl_total = 0;
    for (int iter = 0; iter < ITERATIONS; iter++) {
        stl::vector<int> v;
        auto start = Clock::now();
        for (int round = 0; round < 10; round++) {
            for (int i = 0; i < MEDIUM_N; i++) {
                v.push_back(i);
            }
            v.clear();
        }
        auto end = Clock::now();
        stl_total += to_ms(end - start);
    }

    std::cout << "  std::vector: " << (std_total / ITERATIONS) << " ms\n";
    std::cout << "  stl::vector: " << (stl_total / ITERATIONS) << " ms\n";
    std::cout << "  ratio:       " << (stl_total / std_total) << "x\n";
}

// -----------------------------------------------------------------------------
// Benchmark: pop_back
// -----------------------------------------------------------------------------
void bench_pop_back() {
    std::cout << "\n=== pop_back (int, N=" << LARGE_N << ") ===\n";

    // std::vector
    double std_total = 0;
    for (int iter = 0; iter < ITERATIONS; iter++) {
        std::vector<int> v;
        for (int i = 0; i < LARGE_N; i++) v.push_back(i);

        auto start = Clock::now();
        while (!v.empty()) {
            v.pop_back();
        }
        auto end = Clock::now();
        std_total += to_ms(end - start);
    }

    // stl::vector
    double stl_total = 0;
    for (int iter = 0; iter < ITERATIONS; iter++) {
        stl::vector<int> v;
        for (int i = 0; i < LARGE_N; i++) v.push_back(i);

        auto start = Clock::now();
        while (!v.empty()) {
            v.pop_back();
        }
        auto end = Clock::now();
        stl_total += to_ms(end - start);
    }

    std::cout << "  std::vector: " << (std_total / ITERATIONS) << " ms\n";
    std::cout << "  stl::vector: " << (stl_total / ITERATIONS) << " ms\n";
    std::cout << "  ratio:       " << (stl_total / std_total) << "x\n";
}

int main() {
    std::cout << "Vector Benchmark: stl::vector vs std::vector\n";
    std::cout << "=============================================\n";
    std::cout << "Iterations per benchmark: " << ITERATIONS << "\n";

    bench_push_back_int();
    bench_push_back_reserved_int();
    bench_push_back_string();
    bench_iteration();
    bench_random_access();
    bench_copy();
    bench_clear_refill();
    bench_pop_back();

    std::cout << "\n=============================================\n";
    std::cout << "Benchmark complete.\n";
    std::cout << "Ratio < 1.0 means stl::vector is faster\n";
    std::cout << "Ratio > 1.0 means std::vector is faster\n";

    return 0;
}
