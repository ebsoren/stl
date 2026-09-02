#include "../include/stl/threadsafe_deque.hpp"

#include <array>
#include <atomic>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

namespace {

struct Scenario {
    const char* name;
    int producer_count;
    int consumer_count;
    int items_per_producer;
    int rounds;
};

constexpr std::array scenarios{
    Scenario{"single producer / single consumer", 1, 1, 100000, 10},
    Scenario{"producer-heavy", 8, 1, 50000, 10},
    Scenario{"consumer-heavy", 1, 8, 100000, 10},
    Scenario{"balanced contention", 4, 4, 50000, 80},
    Scenario{"maximum contention", 8, 8, 25000, 40},
};

void wait_for_start(const std::atomic<bool>& start) {
    while (!start.load(std::memory_order_acquire)) {
        std::this_thread::yield();
    }
}

void run_round(const Scenario& scenario, unsigned round) {
    const int total_items = scenario.producer_count * scenario.items_per_producer;
    stl::threadsafe_deque<int> d;
    auto seen = std::make_unique<std::atomic<unsigned>[]>(static_cast<size_t>(total_items));
    std::atomic<bool> start{false};
    std::atomic<bool> producers_done{false};
    std::atomic<int> popped{0};
    std::atomic<int> invalid_values{0};

    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;
    producers.reserve(static_cast<size_t>(scenario.producer_count));
    consumers.reserve(static_cast<size_t>(scenario.consumer_count));

    for (int producer = 0; producer < scenario.producer_count; producer++) {
        producers.emplace_back([&, producer] {
            wait_for_start(start);
            unsigned schedule = round * 0x9e3779b9U + static_cast<unsigned>(producer + 1);

            for (int i = 0; i < scenario.items_per_producer; i++) {
                d.push_back(producer * scenario.items_per_producer + i);

                schedule = schedule * 1664525U + 1013904223U;
                if ((schedule & 31U) == 0) {
                    std::this_thread::yield();
                }
            }
        });
    }

    for (int consumer = 0; consumer < scenario.consumer_count; consumer++) {
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
    for (int i = 0; i < total_items; i++) {
        assert(seen[static_cast<size_t>(i)].load(std::memory_order_relaxed) == 1);
    }
}

}  // namespace

int main() {
    std::cout << "Running threadsafe_deque multithreaded fuzz test...\n";

    long long total_values = 0;
    for (const auto& scenario : scenarios) {
        for (int round = 1; round <= scenario.rounds; round++) {
            run_round(scenario, static_cast<unsigned>(round));
        }
        total_values += static_cast<long long>(scenario.producer_count)
            * scenario.items_per_producer * scenario.rounds;
        std::cout << "  " << scenario.name << ": PASSED\n";
    }

    std::cout << "  " << total_values << " unique values checked: PASSED\n";
    return 0;
}
