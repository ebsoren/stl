#include "../include/stl/threadsafe_deque.hpp"

#include <array>
#include <atomic>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <thread>
#include <vector>

namespace {

constexpr int producer_count = 4;
constexpr int consumer_count = 4;
constexpr int items_per_producer = 1500;
constexpr int total_items = producer_count * items_per_producer;
constexpr int rounds = 12;

void wait_for_start(const std::atomic<bool>& start) {
    while (!start.load(std::memory_order_acquire)) {
        std::this_thread::yield();
    }
}

void run_round(unsigned round) {
    stl::threadsafe_deque<int> d;
    std::array<std::atomic<unsigned>, total_items> seen{};
    std::atomic<bool> start{false};
    std::atomic<bool> producers_done{false};
    std::atomic<int> popped{0};
    std::atomic<int> invalid_values{0};

    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;
    producers.reserve(producer_count);
    consumers.reserve(consumer_count);

    for (int producer = 0; producer < producer_count; producer++) {
        producers.emplace_back([&, producer] {
            wait_for_start(start);
            unsigned schedule = round * 0x9e3779b9U + static_cast<unsigned>(producer + 1);

            for (int i = 0; i < items_per_producer; i++) {
                d.push_back(producer * items_per_producer + i);

                schedule = schedule * 1664525U + 1013904223U;
                if ((schedule & 31U) == 0) {
                    std::this_thread::yield();
                }
            }
        });
    }

    for (int consumer = 0; consumer < consumer_count; consumer++) {
        consumers.emplace_back([&, consumer] {
            wait_for_start(start);
            unsigned schedule = round * 0x85ebca6bU + static_cast<unsigned>(consumer + 1);

            for (;;) {
                auto value = d.try_pop_front();
                if (value) {
                    popped.fetch_add(1, std::memory_order_relaxed);
                    if (*value < 0 || *value >= total_items) {
                        invalid_values.fetch_add(1, std::memory_order_relaxed);
                    } else {
                        seen[static_cast<size_t>(*value)].fetch_add(1, std::memory_order_relaxed);
                    }
                } else if (producers_done.load(std::memory_order_acquire)) {
                    break;
                } else {
                    schedule = schedule * 1664525U + 1013904223U;
                    if ((schedule & 3U) == 0) {
                        std::this_thread::yield();
                    }
                }
            }
        });
    }

    start.store(true, std::memory_order_release);

    for (auto& producer : producers) {
        producer.join();
    }
    producers_done.store(true, std::memory_order_release);

    for (auto& consumer : consumers) {
        consumer.join();
    }

    assert(invalid_values.load(std::memory_order_relaxed) == 0);
    assert(popped.load(std::memory_order_relaxed) == total_items);
    assert(!d.try_pop_front().has_value());
    for (const auto& count : seen) {
        assert(count.load(std::memory_order_relaxed) == 1);
    }
}

}  // namespace

int main() {
    std::cout << "Running threadsafe_deque multithreaded fuzz test...\n";

    for (unsigned round = 1; round <= rounds; round++) {
        run_round(round);
    }

    std::cout << "  " << rounds << " rounds, " << total_items
              << " unique items per round: PASSED\n";
    return 0;
}
