#include "../include/stl/deque.hpp"

#include <chrono>
#include <deque>
#include <iostream>
#include <string>

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

    double std_total = 0;
    for (int iter = 0; iter < ITERATIONS; iter++) {
        std::deque<int> d;
        auto start = Clock::now();
        for (int i = 0; i < LARGE_N; i++) {
            d.push_back(i);
        }
        auto end = Clock::now();
        std_total += to_ms(end - start);
    }

    double stl_total = 0;
    for (int iter = 0; iter < ITERATIONS; iter++) {
        stl::deque<int> d;
        auto start = Clock::now();
        for (int i = 0; i < LARGE_N; i++) {
            d.push_back(i);
        }
        auto end = Clock::now();
        stl_total += to_ms(end - start);
    }

    std::cout << "  std::deque: " << (std_total / ITERATIONS) << " ms\n";
    std::cout << "  stl::deque: " << (stl_total / ITERATIONS) << " ms\n";
    std::cout << "  ratio:      " << (stl_total / std_total) << "x\n";
}

// -----------------------------------------------------------------------------
// Benchmark: push_back (string)
// -----------------------------------------------------------------------------
void bench_push_back_string() {
    std::cout << "\n=== push_back (string, N=" << MEDIUM_N << ") ===\n";

    std::string test_str = "hello world test string";

    double std_total = 0;
    for (int iter = 0; iter < ITERATIONS; iter++) {
        std::deque<std::string> d;
        auto start = Clock::now();
        for (int i = 0; i < MEDIUM_N; i++) {
            d.push_back(test_str);
        }
        auto end = Clock::now();
        std_total += to_ms(end - start);
    }

    double stl_total = 0;
    for (int iter = 0; iter < ITERATIONS; iter++) {
        stl::deque<std::string> d;
        auto start = Clock::now();
        for (int i = 0; i < MEDIUM_N; i++) {
            d.push_back(test_str);
        }
        auto end = Clock::now();
        stl_total += to_ms(end - start);
    }

    std::cout << "  std::deque: " << (std_total / ITERATIONS) << " ms\n";
    std::cout << "  stl::deque: " << (stl_total / ITERATIONS) << " ms\n";
    std::cout << "  ratio:      " << (stl_total / std_total) << "x\n";
}

// -----------------------------------------------------------------------------
// Benchmark: push_front (int)
// -----------------------------------------------------------------------------
void bench_push_front_int() {
    std::cout << "\n=== push_front (int, N=" << LARGE_N << ") ===\n";

    double std_total = 0;
    for (int iter = 0; iter < ITERATIONS; iter++) {
        std::deque<int> d;
        auto start = Clock::now();
        for (int i = 0; i < LARGE_N; i++) {
            d.push_front(i);
        }
        auto end = Clock::now();
        std_total += to_ms(end - start);
    }

    double stl_total = 0;
    for (int iter = 0; iter < ITERATIONS; iter++) {
        stl::deque<int> d;
        auto start = Clock::now();
        for (int i = 0; i < LARGE_N; i++) {
            d.push_front(i);
        }
        auto end = Clock::now();
        stl_total += to_ms(end - start);
    }

    std::cout << "  std::deque: " << (std_total / ITERATIONS) << " ms\n";
    std::cout << "  stl::deque: " << (stl_total / ITERATIONS) << " ms\n";
    std::cout << "  ratio:      " << (stl_total / std_total) << "x\n";
}

// -----------------------------------------------------------------------------
// Benchmark: pop_back (int)
// -----------------------------------------------------------------------------
void bench_pop_back() {
    std::cout << "\n=== pop_back (int, N=" << LARGE_N << ") ===\n";

    double std_total = 0;
    for (int iter = 0; iter < ITERATIONS; iter++) {
        std::deque<int> d;
        for (int i = 0; i < LARGE_N; i++) d.push_back(i);

        auto start = Clock::now();
        while (!d.empty()) {
            d.pop_back();
        }
        auto end = Clock::now();
        std_total += to_ms(end - start);
    }

    double stl_total = 0;
    for (int iter = 0; iter < ITERATIONS; iter++) {
        stl::deque<int> d;
        for (int i = 0; i < LARGE_N; i++) d.push_back(i);

        auto start = Clock::now();
        while (!d.empty()) {
            d.pop_back();
        }
        auto end = Clock::now();
        stl_total += to_ms(end - start);
    }

    std::cout << "  std::deque: " << (std_total / ITERATIONS) << " ms\n";
    std::cout << "  stl::deque: " << (stl_total / ITERATIONS) << " ms\n";
    std::cout << "  ratio:      " << (stl_total / std_total) << "x\n";
}

// -----------------------------------------------------------------------------
// Benchmark: pop_front (int)
// -----------------------------------------------------------------------------
void bench_pop_front() {
    std::cout << "\n=== pop_front (int, N=" << LARGE_N << ") ===\n";

    double std_total = 0;
    for (int iter = 0; iter < ITERATIONS; iter++) {
        std::deque<int> d;
        for (int i = 0; i < LARGE_N; i++) d.push_back(i);

        auto start = Clock::now();
        while (!d.empty()) {
            d.pop_front();
        }
        auto end = Clock::now();
        std_total += to_ms(end - start);
    }

    double stl_total = 0;
    for (int iter = 0; iter < ITERATIONS; iter++) {
        stl::deque<int> d;
        for (int i = 0; i < LARGE_N; i++) d.push_back(i);

        auto start = Clock::now();
        while (!d.empty()) {
            d.pop_front();
        }
        auto end = Clock::now();
        stl_total += to_ms(end - start);
    }

    std::cout << "  std::deque: " << (std_total / ITERATIONS) << " ms\n";
    std::cout << "  stl::deque: " << (stl_total / ITERATIONS) << " ms\n";
    std::cout << "  ratio:      " << (stl_total / std_total) << "x\n";
}

// -----------------------------------------------------------------------------
// Benchmark: push_back many (multi-node, N > BUFFER_SIZE)
// -----------------------------------------------------------------------------
void bench_push_back_many() {
    std::cout << "\n=== push_back multi-node (int, N=" << MEDIUM_N << ") ===\n";

    double std_total = 0;
    for (int iter = 0; iter < ITERATIONS; iter++) {
        std::deque<int> d;
        auto start = Clock::now();
        for (int i = 0; i < MEDIUM_N; i++) {
            d.push_back(i);
        }
        auto end = Clock::now();
        std_total += to_ms(end - start);
    }

    double stl_total = 0;
    for (int iter = 0; iter < ITERATIONS; iter++) {
        stl::deque<int> d;
        auto start = Clock::now();
        for (int i = 0; i < MEDIUM_N; i++) {
            d.push_back(i);
        }
        auto end = Clock::now();
        stl_total += to_ms(end - start);
    }

    std::cout << "  std::deque: " << (std_total / ITERATIONS) << " ms\n";
    std::cout << "  stl::deque: " << (stl_total / ITERATIONS) << " ms\n";
    std::cout << "  ratio:      " << (stl_total / std_total) << "x\n";
}

// -----------------------------------------------------------------------------
// Benchmark: push/pop interleaved
// -----------------------------------------------------------------------------
void bench_push_pop_interleaved() {
    std::cout << "\n=== push/pop interleaved (int, N=" << MEDIUM_N << ") ===\n";

    double std_total = 0;
    for (int iter = 0; iter < ITERATIONS; iter++) {
        std::deque<int> d;
        auto start = Clock::now();
        for (int i = 0; i < MEDIUM_N; i++) {
            d.push_back(i);
            d.push_front(i);
            d.pop_front();
            d.pop_back();
        }
        auto end = Clock::now();
        std_total += to_ms(end - start);
    }

    double stl_total = 0;
    for (int iter = 0; iter < ITERATIONS; iter++) {
        stl::deque<int> d;
        auto start = Clock::now();
        for (int i = 0; i < MEDIUM_N; i++) {
            d.push_back(i);
            d.push_front(i);
            d.pop_front();
            d.pop_back();
        }
        auto end = Clock::now();
        stl_total += to_ms(end - start);
    }

    std::cout << "  std::deque: " << (std_total / ITERATIONS) << " ms\n";
    std::cout << "  stl::deque: " << (stl_total / ITERATIONS) << " ms\n";
    std::cout << "  ratio:      " << (stl_total / std_total) << "x\n";
}

// -----------------------------------------------------------------------------
// Benchmark: range construction
// -----------------------------------------------------------------------------
void bench_range_construction() {
    std::cout << "\n=== range construction (int, N=" << LARGE_N << ") ===\n";

    // Setup source array
    int* arr = new int[LARGE_N];
    for (int i = 0; i < LARGE_N; i++) arr[i] = i;

    double std_total = 0;
    for (int iter = 0; iter < ITERATIONS; iter++) {
        auto start = Clock::now();
        std::deque<int> d(arr, arr + LARGE_N);
        auto end = Clock::now();
        std_total += to_ms(end - start);
        (void)d;
    }

    double stl_total = 0;
    for (int iter = 0; iter < ITERATIONS; iter++) {
        auto start = Clock::now();
        stl::deque<int> d(arr, arr + LARGE_N);
        auto end = Clock::now();
        stl_total += to_ms(end - start);
        (void)d;
    }

    delete[] arr;

    std::cout << "  std::deque: " << (std_total / ITERATIONS) << " ms\n";
    std::cout << "  stl::deque: " << (stl_total / ITERATIONS) << " ms\n";
    std::cout << "  ratio:      " << (stl_total / std_total) << "x\n";
}

int main() {
    std::cout << "Deque Benchmark: stl::deque vs std::deque\n";
    std::cout << "==========================================\n";
    std::cout << "Iterations per benchmark: " << ITERATIONS << "\n";

    bench_push_back_int();
    bench_push_back_string();
    bench_push_front_int();
    bench_pop_back();
    bench_pop_front();
    bench_push_back_many();
    bench_push_pop_interleaved();
    bench_range_construction();

    std::cout << "\n==========================================\n";
    std::cout << "Benchmark complete.\n";
    std::cout << "Ratio < 1.0 means stl::deque is faster\n";
    std::cout << "Ratio > 1.0 means std::deque is faster\n";

    return 0;
}
