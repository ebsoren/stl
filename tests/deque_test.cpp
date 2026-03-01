#include "../include/stl/deque.hpp"

#include <cassert>
#include <iostream>
#include <string>

// Helper to track construction/destruction
struct Tracked {
    static int constructions;
    static int destructions;
    static int copies;
    static int moves;

    int value;

    Tracked(int v = 0) : value(v) { constructions++; }
    Tracked(const Tracked& other) : value(other.value) { copies++; }
    Tracked(Tracked&& other) noexcept : value(other.value) { moves++; other.value = -1; }
    ~Tracked() { destructions++; }

    Tracked& operator=(const Tracked& other) { value = other.value; copies++; return *this; }
    Tracked& operator=(Tracked&& other) noexcept { value = other.value; moves++; other.value = -1; return *this; }

    static void reset() { constructions = destructions = copies = moves = 0; }
};

int Tracked::constructions = 0;
int Tracked::destructions = 0;
int Tracked::copies = 0;
int Tracked::moves = 0;

// --- Construction tests ---

void test_default_construction() {
    stl::deque<int> d;
    // No accessors yet; just verify it constructs without crashing
    std::cout << "  default_construction: PASSED\n";
}

void test_range_construction() {
    int arr[] = {1, 2, 3, 4, 5};
    stl::deque<int> d(arr, arr + 5);
    std::cout << "  range_construction: PASSED\n";
}

void test_range_construction_large() {
    // Larger than BUFFER_SIZE (512) to exercise multi-node path
    const size_t N = 600;
    int arr[N];
    for (size_t i = 0; i < N; i++) arr[i] = static_cast<int>(i);
    stl::deque<int> d(arr, arr + N);
    std::cout << "  range_construction_large: PASSED\n";
}

// --- push_back tests ---

void test_push_back_lvalue() {
    stl::deque<int> d;
    int x = 42;
    d.push_back(x);
    d.push_back(x);
    std::cout << "  push_back_lvalue: PASSED\n";
}

void test_push_back_rvalue() {
    stl::deque<int> d;
    d.push_back(1);
    d.push_back(2);
    d.push_back(3);
    std::cout << "  push_back_rvalue: PASSED\n";
}

void test_push_back_move_semantics() {
    Tracked::reset();
    stl::deque<Tracked> d;
    d.push_back(Tracked(10));  // Should move, not copy
    assert(Tracked::moves > 0);
    std::cout << "  push_back_move_semantics: PASSED\n";
}

void test_push_back_many() {
    // Push more than BUFFER_SIZE elements to exercise node allocation
    stl::deque<int> d;
    for (int i = 0; i < 600; i++) {
        d.push_back(i);
    }
    std::cout << "  push_back_many: PASSED\n";
}

// --- push_front tests ---

void test_push_front_lvalue() {
    stl::deque<int> d;
    int x = 99;
    d.push_front(x);
    d.push_front(x);
    std::cout << "  push_front_lvalue: PASSED\n";
}

void test_push_front_rvalue() {
    stl::deque<int> d;
    d.push_front(1);
    d.push_front(2);
    d.push_front(3);
    std::cout << "  push_front_rvalue: PASSED\n";
}

void test_push_front_move_semantics() {
    Tracked::reset();
    stl::deque<Tracked> d;
    d.push_front(Tracked(10));  // Should move, not copy
    assert(Tracked::moves > 0);
    std::cout << "  push_front_move_semantics: PASSED\n";
}

// --- pop tests ---

void test_pop_back() {
    stl::deque<int> d;
    d.push_back(1);
    d.push_back(2);
    d.push_back(3);
    d.pop_back();
    d.pop_back();
    std::cout << "  pop_back: PASSED\n";
}

void test_pop_front() {
    stl::deque<int> d;
    d.push_back(1);
    d.push_back(2);
    d.push_back(3);
    d.pop_front();
    d.pop_front();
    std::cout << "  pop_front: PASSED\n";
}

void test_push_pop_interleaved() {
    stl::deque<int> d;
    d.push_back(1);
    d.push_back(2);
    d.push_front(0);
    d.pop_front();
    d.push_back(3);
    d.pop_back();
    std::cout << "  push_pop_interleaved: PASSED\n";
}

// --- Destructor tests ---

void test_destructor_called() {
    Tracked::reset();
    {
        stl::deque<Tracked> d;
        d.push_back(Tracked(1));
        d.push_back(Tracked(2));
        d.push_back(Tracked(3));
    }
    assert(Tracked::destructions >= 3);
    std::cout << "  destructor_called: PASSED\n";
}

void test_destructor_multi_node() {
    Tracked::reset();
    {
        stl::deque<Tracked> d;
        // Push enough to span multiple nodes
        for (int i = 0; i < 600; i++) {
            d.push_back(Tracked(i));
        }
        int before = Tracked::destructions;
        // Destructor runs here — should clean up all nodes
        (void)before;
    }
    assert(Tracked::destructions >= 600);
    std::cout << "  destructor_multi_node: PASSED\n";
}

// --- String type tests ---

void test_with_strings() {
    stl::deque<std::string> d;
    d.push_back("hello");
    d.push_back("world");
    d.push_front("start");
    d.pop_back();
    d.pop_front();
    std::cout << "  with_strings: PASSED\n";
}

int main() {
    std::cout << "Running deque tests...\n\n";

    std::cout << "Construction tests:\n";
    test_default_construction();
    test_range_construction();
    test_range_construction_large();

    std::cout << "\npush_back tests:\n";
    test_push_back_lvalue();
    test_push_back_rvalue();
    test_push_back_move_semantics();
    test_push_back_many();

    std::cout << "\npush_front tests:\n";
    test_push_front_lvalue();
    test_push_front_rvalue();
    test_push_front_move_semantics();

    std::cout << "\npop tests:\n";
    test_pop_back();
    test_pop_front();
    test_push_pop_interleaved();

    std::cout << "\nDestructor tests:\n";
    test_destructor_called();
    test_destructor_multi_node();

    std::cout << "\nOther tests:\n";
    test_with_strings();

    std::cout << "\n=== All tests passed! ===\n";
    return 0;
}
