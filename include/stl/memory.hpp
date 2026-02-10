#ifndef STL_MEMORY_HPP
#define STL_MEMORY_HPP

#include <cstddef>
#include <initializer_list>
#include <cstring>

#include "type_traits.hpp"
#include "utility.hpp"

namespace stl {

template <typename T>
class unique_ptr {
    T *ptr_;

public:
    explicit unique_ptr(T *ptr = nullptr)
        : ptr_(ptr) {}

    ~unique_ptr() { delete ptr_; }
    // Don't allow copying
    unique_ptr(const unique_ptr &) = delete;
    unique_ptr &operator=(const unique_ptr &) = delete;

    // Move operation is okay as underlying pointer remains singly-owned
    unique_ptr(unique_ptr &&other) noexcept
        : ptr_(other.ptr_) {
        other.ptr_ = nullptr;
    }
    unique_ptr &operator=(unique_ptr &&other) noexcept {
        delete ptr_;
        ptr_ = other.ptr_;
        other.ptr_ = nullptr;
    }

    // Provide access to base pointer without releasing ownership
    T *get() const { return ptr_; }

    // Provide access to base pointer while also releasing ownership
    T *release() {
        T *temp = ptr_;
        ptr_ = nullptr;
        return temp;
    }
};


template <typename T>
class shared_ptr {
    T *ptr_;


public:
    shared_ptr(T *ptr = nullptr)
        : ptr_(ptr) {}
};


// Construct element at memory location that p points to
template <typename T>
void construct_at(T *p, const T &object) {
    new (p) T(object);
}


// for trivial types only, essentially construct() without constructor calls.
// Like memset for multi byte values
template <typename T>
void memplace(T *start, T *end, const T &object) {
    while (start != end) {
        *start++ = object;
    }
}

// construct array of elements with value <object>
template <typename T>
void fill(T *start, T *end, const T &object) {
    if constexpr (stl::is_trivially_copyable<T>::value) {
        memplace(start, end, object);
    } else {
        while (start != end) construct_at(start++, object);
    }
}

// construct elements at src -> src + len at dest -> dest + len
template <typename T>
void construct(T *dest, T *src, size_t len) {
    if constexpr (stl::is_trivially_copyable<T>::value) {
        memcpy(dest, src, len * sizeof(T));
    } else {
        for (size_t i = 0; i < len; ++i) {
            construct_at(dest + i, *(src + i));
        }
    }
}


// Destroy element p points to
template <typename T>
void destroy_at(T *p) {
    p->~T();
}

// Destroy array of elements
template <typename T>
void destroy(T *start, T *end) {
    while (start != end) stl::destroy_at(start++);
}

// Destroy array of elements, and free the space contained
template <typename T>
void release(T *start, T *end) {
    if (!start) return;
    if constexpr (!is_trivially_destructible<T>::value) {
        destroy(start, end);
    }
    free(start);
}


// Erase n elements starting at <start> for container ending at <end>
template <typename T>
void c_erase(T *start, T *end, size_t n) {
    while (start != (end - n)) {
        *start = stl::move(*(start + n));
        start++;
    }
    destroy(end - n, end);
}


}   // namespace stl

#endif   // STL_MEMORY_HPP
