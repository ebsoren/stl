#ifndef STL_MEMORY_HPP
#define STL_MEMORY_HPP

#include <cstddef>
#include <initializer_list>

#include "utility.hpp"

namespace stl {

template <typename T>
class unique_ptr {
    T *ptr_;

public:
    unique_ptr(T *ptr = nullptr)
        : ptr_(ptr) {}

    ~unique_ptr() { delete ptr_; }

    // Don't allow copying
    unique_ptr(const unique_ptr &) = delete;
    unique_ptr &operator=(const unique_ptr &) = delete;

    // Move operation is okay as underlying pointer remains singly-owned
    unique_ptr(unique_ptr &&other) noexcept {
        ptr_ = stl::move(other.ptr_);
    }
    unique_ptr &operator=(unique_ptr &&other) noexcept {
        ptr_ = stl::move(other.ptr_);

    }

    // Provide access to base pointer without releasing ownership
    T* get() {
        return ptr_;
    }

    // Provide access to base pointer while also releasing ownership
    T* release() {
        T* temp = ptr_;
        delete ptr_;
        return temp;
    }
};


template <typename T>
class shared_ptr {
    T *ptr_;


public:
    shared_ptr(T *ptr = nullptr)
        : ptr_(ptr) {
    }
};

// Destroy element p points to
template <typename T>
void destroy_at(T* p) {
    p->~T();
}

// Destroy array of elements
template <typename T>
void destroy(T* start, T* end) {
    while (start != end) destroy_at(start++);
}

}   // namespace stl

#endif   // STL_MEMORY_HPP
