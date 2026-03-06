#include "../include/stl/internal/red_black_tree.hpp"

#include <cassert>
#include <iostream>

// --- Insertion / contains tests ---

void test_empty_tree() {
    stl::red_black_tree<int> t;
    assert(t.empty());
    assert(!t.contains(0));
    assert(!t.contains(42));
    std::cout << "  empty_tree: PASSED\n";
}

void test_insert_single() {
    stl::red_black_tree<int> t;
    t.insert(10);
    assert(t.contains(10));
    assert(!t.contains(9));
    assert(!t.contains(11));
    assert(!t.empty());
    std::cout << "  insert_single: PASSED\n";
}

void test_insert_two_left() {
    stl::red_black_tree<int> t;
    t.insert(10);
    t.insert(5);
    assert(t.contains(10));
    assert(t.contains(5));
    assert(!t.contains(7));
    std::cout << "  insert_two_left: PASSED\n";
}

void test_insert_two_right() {
    stl::red_black_tree<int> t;
    t.insert(10);
    t.insert(15);
    assert(t.contains(10));
    assert(t.contains(15));
    assert(!t.contains(12));
    std::cout << "  insert_two_right: PASSED\n";
}

void test_insert_three_balanced() {
    stl::red_black_tree<int> t;
    t.insert(10);
    t.insert(5);
    t.insert(15);
    assert(t.contains(10));
    assert(t.contains(5));
    assert(t.contains(15));
    assert(!t.contains(1));
    assert(!t.contains(7));
    assert(!t.contains(20));
    std::cout << "  insert_three_balanced: PASSED\n";
}

void test_insert_left_spine() {
    stl::red_black_tree<int> t;
    t.insert(50);
    t.insert(40);
    t.insert(30);
    t.insert(20);
    t.insert(10);
    assert(t.contains(50));
    assert(t.contains(40));
    assert(t.contains(30));
    assert(t.contains(20));
    assert(t.contains(10));
    assert(!t.contains(25));
    assert(!t.contains(45));
    std::cout << "  insert_left_spine: PASSED\n";
}

void test_insert_right_spine() {
    stl::red_black_tree<int> t;
    t.insert(10);
    t.insert(20);
    t.insert(30);
    t.insert(40);
    t.insert(50);
    assert(t.contains(10));
    assert(t.contains(20));
    assert(t.contains(30));
    assert(t.contains(40));
    assert(t.contains(50));
    assert(!t.contains(15));
    assert(!t.contains(55));
    std::cout << "  insert_right_spine: PASSED\n";
}

void test_insert_duplicate() {
    stl::red_black_tree<int> t;
    t.insert(7);
    t.insert(7);
    assert(t.contains(7));
    assert(!t.contains(6));
    std::cout << "  insert_duplicate: PASSED\n";
}

void test_insert_many() {
    stl::red_black_tree<int> t;
    for (int i = 1; i <= 15; ++i) {
        t.insert(i);
    }
    for (int i = 1; i <= 15; ++i) {
        assert(t.contains(i));
    }
    assert(!t.contains(0));
    assert(!t.contains(16));
    std::cout << "  insert_many: PASSED\n";
}

void test_insert_mixed_order() {
    stl::red_black_tree<int> t;
    int vals[] = {8, 4, 12, 2, 6, 10, 14, 1, 3, 5, 7, 9, 11, 13, 15};
    for (int v : vals) {
        t.insert(v);
    }
    for (int v : vals) {
        assert(t.contains(v));
    }
    assert(!t.contains(0));
    assert(!t.contains(16));
    std::cout << "  insert_mixed_order: PASSED\n";
}

void test_clear_after_insert() {
    stl::red_black_tree<int> t;
    t.insert(1);
    t.insert(2);
    t.insert(3);
    assert(!t.empty());
    assert(t.contains(2));
    t.clear();
    assert(t.empty());
    assert(!t.contains(1));
    assert(!t.contains(2));
    assert(!t.contains(3));
    std::cout << "  clear_after_insert: PASSED\n";
}

void test_insert_after_clear() {
    stl::red_black_tree<int> t;
    t.insert(5);
    t.clear();
    t.insert(99);
    assert(t.contains(99));
    assert(!t.contains(5));
    std::cout << "  insert_after_clear: PASSED\n";
}

int main() {
    std::cout << "Running red_black_tree tests...\n\n";

    std::cout << "State tests:\n";
    test_empty_tree();

    std::cout << "\nInsertion tests:\n";
    test_insert_single();
    test_insert_two_left();
    test_insert_two_right();
    test_insert_three_balanced();
    test_insert_left_spine();
    test_insert_right_spine();
    test_insert_duplicate();
    test_insert_many();
    test_insert_mixed_order();

    std::cout << "\nClear tests:\n";
    test_clear_after_insert();
    test_insert_after_clear();

    std::cout << "\n=== All tests passed! ===\n";
    return 0;
}
