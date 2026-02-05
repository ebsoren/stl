#include <stl/vector.hpp>
#include <cassert>
#include <iostream>

int main() {
    // Default construction
    stl::vector<int> v;
    assert(v.empty());
    assert(v.size() == 0);

    // push_back
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    assert(v.size() == 3);
    assert(!v.empty());

    // Element access
    assert(v[0] == 1);
    assert(v[1] == 2);
    assert(v[2] == 3);

    // pop_back
    v.pop_back();
    assert(v.size() == 2);

    // clear
    v.clear();
    assert(v.empty());

    std::cout << "All tests passed!\n";
    return 0;
}
