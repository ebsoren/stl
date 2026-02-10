#include "../include/stl/vector.hpp"

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

void test_default_construction() {
    stl::vector<int> v;
    assert(v.empty());
    assert(v.size() == 0);
    assert(v.capacity() == 0);
    std::cout << "  default_construction: PASSED\n";
}

void test_capacity_construction() {
    stl::vector<int> v(10);
    assert(v.empty());
    assert(v.size() == 0);
    assert(v.capacity() == 10);
    std::cout << "  capacity_construction: PASSED\n";
}

void test_fill_construction() {
    stl::vector<int> v(5, 42);
    assert(v.size() == 5);
    for (size_t i = 0; i < v.size(); i++) {
        assert(v[i] == 42);
    }
    std::cout << "  fill_construction: PASSED\n";
}

void test_initializer_list() {
    stl::vector<int> v = {1, 2, 3, 4, 5};
    assert(v.size() == 5);
    assert(v[0] == 1);
    assert(v[4] == 5);
    std::cout << "  initializer_list: PASSED\n";
}

void test_copy_construction() {
    stl::vector<int> v1 = {1, 2, 3};
    stl::vector<int> v2(v1);
    assert(v2.size() == 3);
    assert(v2[0] == 1);
    assert(v2[2] == 3);
    // Ensure deep copy
    v1[0] = 100;
    assert(v2[0] == 1);
    std::cout << "  copy_construction: PASSED\n";
}

void test_move_construction() {
    stl::vector<int> v1 = {1, 2, 3};
    int* original_data = v1.data();
    stl::vector<int> v2(stl::move(v1));
    assert(v2.size() == 3);
    assert(v2.data() == original_data);  // Same underlying pointer
    assert(v1.data() == nullptr);
    std::cout << "  move_construction: PASSED\n";
}

void test_copy_assignment() {
    stl::vector<int> v1 = {1, 2, 3};
    stl::vector<int> v2;
    v2 = v1;
    assert(v2.size() == 3);
    assert(v2[0] == 1);
    v1[0] = 100;
    assert(v2[0] == 1);
    std::cout << "  copy_assignment: PASSED\n";
}

void test_move_assignment() {
    stl::vector<int> v1 = {1, 2, 3};
    int* original_data = v1.data();
    stl::vector<int> v2;
    v2 = stl::move(v1);
    assert(v2.size() == 3);
    assert(v2.data() == original_data);
    assert(v1.data() == nullptr);
    std::cout << "  move_assignment: PASSED\n";
}

void test_push_back() {
    stl::vector<int> v;
    for (int i = 0; i < 100; i++) {
        v.push_back(i);
    }
    assert(v.size() == 100);
    for (int i = 0; i < 100; i++) {
        assert(v[i] == i);
    }
    std::cout << "  push_back: PASSED\n";
}

void test_push_back_move() {
    Tracked::reset();
    stl::vector<Tracked> v;
    v.push_back(Tracked(42));  // Should move, not copy
    assert(v.size() == 1);
    assert(v[0].value == 42);
    assert(Tracked::moves > 0);
    std::cout << "  push_back_move: PASSED\n";
}

void test_pop_back() {
    stl::vector<int> v = {1, 2, 3};
    v.pop_back();
    assert(v.size() == 2);
    assert(v.back() == 2);
    v.pop_back();
    v.pop_back();
    assert(v.empty());
    std::cout << "  pop_back: PASSED\n";
}

void test_pop_back_destroys() {
    Tracked::reset();
    {
        stl::vector<Tracked> v;
        v.push_back(Tracked(1));
        v.push_back(Tracked(2));
        int destructions_before = Tracked::destructions;
        v.pop_back();
        assert(Tracked::destructions > destructions_before);
    }
    std::cout << "  pop_back_destroys: PASSED\n";
}

void test_clear() {
    stl::vector<int> v = {1, 2, 3, 4, 5};
    size_t old_cap = v.capacity();
    v.clear();
    assert(v.empty());
    assert(v.size() == 0);
    assert(v.capacity() == old_cap);  // Capacity unchanged
    std::cout << "  clear: PASSED\n";
}

void test_clear_destroys() {
    Tracked::reset();
    {
        stl::vector<Tracked> v;
        v.push_back(Tracked(1));
        v.push_back(Tracked(2));
        v.push_back(Tracked(3));
        int destructions_before = Tracked::destructions;
        v.clear();
        assert(Tracked::destructions >= destructions_before + 3);
    }
    std::cout << "  clear_destroys: PASSED\n";
}

void test_element_access() {
    stl::vector<int> v = {10, 20, 30};

    // operator[]
    assert(v[0] == 10);
    assert(v[1] == 20);
    assert(v[2] == 30);

    // at()
    assert(v.at(0) == 10);
    assert(v.at(1) == 20);

    // at() bounds checking
    bool threw = false;
    try {
        v.at(100);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);

    // front/back
    assert(v.front() == 10);
    assert(v.back() == 30);

    // data
    assert(v.data()[0] == 10);

    std::cout << "  element_access: PASSED\n";
}

void test_iterators() {
    stl::vector<int> v = {1, 2, 3, 4, 5};

    // begin/end
    int sum = 0;
    for (auto it = v.begin(); it != v.end(); ++it) {
        sum += *it;
    }
    assert(sum == 15);

    // range-based for
    sum = 0;
    for (int x : v) {
        sum += x;
    }
    assert(sum == 15);

    std::cout << "  iterators: PASSED\n";
}

void test_reserve() {
    stl::vector<int> v;
    v.reserve(100);
    assert(v.capacity() >= 100);
    assert(v.size() == 0);

    // Adding elements shouldn't reallocate
    int* ptr = v.data();
    for (int i = 0; i < 100; i++) {
        v.push_back(i);
    }
    assert(v.data() == ptr);

    std::cout << "  reserve: PASSED\n";
}

void test_resize() {
    stl::vector<int> v = {1, 2, 3};

    // Grow
    v.resize(5);
    assert(v.size() == 5);
    assert(v[0] == 1);
    assert(v[2] == 3);

    // Shrink
    v.resize(2);
    assert(v.size() == 2);
    assert(v[0] == 1);
    assert(v[1] == 2);

    std::cout << "  resize: PASSED\n";
}

void test_swap() {
    stl::vector<int> v1 = {1, 2, 3};
    stl::vector<int> v2 = {4, 5};

    v1.swap(v2);

    assert(v1.size() == 2);
    assert(v1[0] == 4);
    assert(v2.size() == 3);
    assert(v2[0] == 1);

    std::cout << "  swap: PASSED\n";
}

void test_with_strings() {
    stl::vector<std::string> v;
    v.push_back("hello");
    v.push_back("world");
    v.push_back("test");

    assert(v.size() == 3);
    assert(v[0] == "hello");
    assert(v[1] == "world");
    assert(v[2] == "test");

    v.pop_back();
    assert(v.size() == 2);

    v.clear();
    assert(v.empty());

    std::cout << "  with_strings: PASSED\n";
}

void test_destructor_called() {
    Tracked::reset();
    {
        stl::vector<Tracked> v;
        v.push_back(Tracked(1));
        v.push_back(Tracked(2));
        v.push_back(Tracked(3));
    }
    // All elements should be destroyed when vector goes out of scope
    assert(Tracked::destructions >= 3);
    std::cout << "  destructor_called: PASSED\n";
}

void test_reallocation_moves() {
    Tracked::reset();
    stl::vector<Tracked> v;

    // Force several reallocations
    for (int i = 0; i < 20; i++) {
        v.push_back(Tracked(i));
    }

    // Should have used moves, not copies, during reallocation
    assert(Tracked::moves > 0);
    std::cout << "  reallocation_moves: PASSED (moves=" << Tracked::moves << ", copies=" << Tracked::copies << ")\n";
}

int main() {
    std::cout << "Running vector tests...\n\n";

    std::cout << "Construction tests:\n";
    test_default_construction();
    test_capacity_construction();
    test_fill_construction();
    test_initializer_list();
    test_copy_construction();
    test_move_construction();

    std::cout << "\nAssignment tests:\n";
    test_copy_assignment();
    test_move_assignment();

    std::cout << "\nModifier tests:\n";
    test_push_back();
    test_push_back_move();
    test_pop_back();
    test_pop_back_destroys();
    test_clear();
    test_clear_destroys();

    std::cout << "\nAccess tests:\n";
    test_element_access();
    test_iterators();

    std::cout << "\nCapacity tests:\n";
    test_reserve();
    test_resize();

    std::cout << "\nOther tests:\n";
    test_swap();
    test_with_strings();
    test_destructor_called();
    test_reallocation_moves();

    std::cout << "\n=== All tests passed! ===\n";
    return 0;
}
