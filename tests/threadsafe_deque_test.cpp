#include "../include/stl/threadsafe_deque.hpp"

#include <cassert>
#include <iostream>
#include <string>

void test_empty_pop() {
    stl::threadsafe_deque<int> d;

    assert(!d.try_pop_front().has_value());
    assert(!d.try_pop_front().has_value());

    std::cout << "  empty_pop: PASSED\n";
}

void test_fifo_order() {
    stl::threadsafe_deque<int> d;
    d.push_back(1);
    d.push_back(2);
    d.push_back(3);

    assert(d.try_pop_front() == 1);
    assert(d.try_pop_front() == 2);
    assert(d.try_pop_front() == 3);
    assert(!d.try_pop_front().has_value());

    std::cout << "  fifo_order: PASSED\n";
}

void test_reuse_after_empty() {
    stl::threadsafe_deque<int> d;
    d.push_back(1);
    assert(d.try_pop_front() == 1);
    assert(!d.try_pop_front().has_value());

    d.push_back(2);
    assert(d.try_pop_front() == 2);
    assert(!d.try_pop_front().has_value());

    std::cout << "  reuse_after_empty: PASSED\n";
}

void test_crosses_buffer_boundaries() {
    stl::threadsafe_deque<int> d;
    constexpr int count = 1200;

    for (int i = 0; i < count; i++) {
        d.push_back(i);
    }
    for (int i = 0; i < count; i++) {
        assert(d.try_pop_front() == i);
    }
    assert(!d.try_pop_front().has_value());

    std::cout << "  crosses_buffer_boundaries: PASSED\n";
}

void test_with_strings() {
    stl::threadsafe_deque<std::string> d;
    d.push_back("hello");
    d.push_back("world");

    assert(d.try_pop_front() == "hello");
    assert(d.try_pop_front() == "world");
    assert(!d.try_pop_front().has_value());

    std::cout << "  with_strings: PASSED\n";
}

int main() {
    std::cout << "Running threadsafe_deque tests...\n\n";

    test_empty_pop();
    test_fifo_order();
    test_reuse_after_empty();
    test_crosses_buffer_boundaries();
    test_with_strings();

    std::cout << "\n=== All tests passed! ===\n";
    return 0;
}
