#ifndef STL_RED_BLACK_TREE
#define STL_RED_BLACK_TREE

#include "../vector.hpp"
#include <algorithm>

/*

                1
            2       3
        4     5  6     7
1 23 4567 8 9 10 11 12 13 14 15

1

2

4

8

*/


namespace stl {

template <typename T>
class red_black_tree {
    stl::vector<bool> in_use_;
    stl::vector<T> nodes_;


    unsigned int find_(size_t loc, T val) {
        if (loc >= nodes_.size() || !in_use_[loc] || nodes_[loc] == val) return loc;
        return find_((loc + 1) * 2 + (val > nodes_[loc]), val);
    }

    // Rebalance tree
    void rebalance_() {
        // TODO
    }

    // Remove node from tree
    void remove_(unsigned int loc) {
        // TODO
    }

public:
    // TODO: Rebalance
    void insert(const T val) {
        unsigned int loc = find_(0, val);
        while (loc >= nodes_.size()) {
            nodes_.resize(max((size_t)2, nodes_.size() * 2));
            in_use_.resize(max((size_t)2, in_use_.size() * 2));
        }
        in_use_[loc] = true;
        nodes_[loc] = val;
    }

    // TODO: Rebalance
    void erase(const T val) {
        unsigned int loc = find_(0, val);
        if (loc < nodes_.size() && in_use_[loc] && nodes_[loc] == val) {
            remove_(loc);
            in_use_[loc] = false;
        }
    }

    bool contains(const T val) {
        unsigned int loc = find_(0, val);
        return loc < nodes_.size() && in_use_[loc] && nodes_[loc] == val;
    }

    bool empty() { return nodes_.empty(); }

    void clear() {
        in_use_.resize(0);
        nodes_.resize(0);
    }

};

}   // namespace stl


#endif   // STL_RED_BLACK_TREE