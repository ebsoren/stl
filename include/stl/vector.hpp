/*

TODO: fix memory management, fix cap-value constructor

*/
#ifndef STL_VECTOR_HPP
#define STL_VECTOR_HPP

#include <cstddef>
#include <exception>
#include <initializer_list>
#include <memory>

#include "algorithm.hpp"
#include "utility.hpp"
#include "type_traits.hpp"
#include "memory.hpp"

namespace stl {

template <typename T>
class vector {
public:
    // Default constructor
    vector() {
        size_ = 0;
        cap_ = 0;
        data_ = nullptr;
    }

    // Construct with specified capacity
    vector(size_t cap)
        : cap_(cap) {
        size_ = 0;
        data_ = malloc(cap_ * sizeof(T));
    }

    // Construct with capacity and fill value
    vector(size_t cap, T &&value)
        : cap_(cap) {
        data_ = malloc(cap_ * sizeof(T));
        size_ = cap;
        memset(data_, value, cap_ * sizeof(T));
    }

    vector(size_t cap, const T &value)
        : cap_(cap) {
        data_ = malloc(cap_ * sizeof(T));
        size_ = cap;
        memset(data_, value, cap_ * sizeof(T));
    }

    // Copy constructor
    vector(const vector<T> &other)
        : cap_(other.cap_)
        , size_(other.size_) {
        data_ = malloc(cap_ * sizeof(T));
        memcpy(data_, other.data_, size_ * sizeof(T));
    }

    // Move constructor
    vector(vector<T> &&other)
        : cap_(other.cap_)
        , size_(other.size_) {
        data_ = other.data_;
        other.data_ = nullptr;
    }

    // Destructor
    ~vector() {
        if (data_) free(data_);
    }

    // Assignment operator which supports chaining
    vector &operator=(const vector &other) {
        if (this == &other) return *this;

        size_ = other.size_;
        cap_ = other.cap_;

        if (data_) free(data_);
        data_ = malloc(cap_ * sizeof(T));
        memcpy(data_, other.data_, size_ * sizeof(T));
        return *this;
    }

    // Move assignment
    vector &operator=(vector &&other) {
        size_ = other.size_;
        cap_ = other.cap_;

        if (data_) free(data_);
        data_ = malloc(cap_ * sizeof(T));
        memcpy(data_, other.data_, size_ * sizeof(T));
        return *this;
    }


    // Reserve space for at least n elements
    void reserve(size_t n) {
        if (cap_ >= n) return;
        realloc_(n);
    }

    // TODO
    void resize(size_t n) {
        if (n > cap_) realloc_(n);

        if (n < size_) {}

        size_ = n;
        cap_ = n;
    }

    // Random access operator
    T &operator[](size_t i) { return *(data_ + i); }

    const T &operator[](size_t i) const { return *(data_ + i); }

    T &at(size_t i) {
        if (i >= size_) throw std::out_of_range("vector .at() called with index outside of range");
        return data_[i];
    }

    const T &at(size_t i) const {
        if (i >= size_) throw std::out_of_range("vector .at() called with index outside of range");
        return data_[i];
    }

    // Various access methods

    T &front() { return data_[0]; }
    T &back() { return data_[size_ - 1]; }

    T *data() { return data_; }

    const T &front() const { return data_[0]; }
    const T &back() const { return data_[size_ - 1]; }

    const T *data() const { return data_; }

    T *begin() { return data_; }
    T *end() { return data_ + size_; }

    const T *begin() const { return data_; }
    const T *end() const { return data_ + size_; }

    // Size and capacity
    std::size_t size() const { return size_; }
    std::size_t capacity() const { return cap_; }

    // Push
    void push_back(const T &val) {
        if (cap_ == size_) {
            realloc_();
        }
        data_[size_++] = val;
    }

    void push_back(T &&val) {
        if (cap_ == size_) {
            realloc_();
        }
        data_[size_++] = stl::move(val);
    }

    void pop_back() { size_--; }

    void clear() {

    }

    bool empty() { return size_ == 0; }

    bool operator==(const vector &other) { return equals(this, other); }

    bool operator!=(const vector &other) { return !(this == other); }


    void swap(vector &other) {
        stl::copy_swap(size_, other.size_);
        stl::copy_swap(cap_, other.cap_);
        stl::copy_swap(data_, other.data_);
    }

    // insert <val> <num_insertion> times starting at <loc>
    T *insert(T *loc, T &&val, size_t num_insertions = 1) {
        size_t size_after_insertion = size_ + num_insertions;
        if (size_after_insertion > cap_) {
            size_t diff = loc - data_;
            realloc(stl::max(cap_ * 2, size_after_insertion));
            loc = data_ + diff;
        }

        size_t front = size_ - 1;
        for (size_t back = size_after_insertion - 1; back >= size_; --back, --front) {
            data_[back] = std::move(data_[front]);
        }
        T *res = loc;
        for (size_t i = 0; i < num_insertions; ++i) {
            *loc++ = val;
        }
        size_ += num_insertions;
        return res;
    }

    // TODO after fixing/understanding memory
    void erase(T *loc) {}
    void erase(T *start, T *end);


private:
    T *data_;
    size_t size_;
    size_t cap_;


    // increase vector capacity, default cap_ * 2
    void realloc_(size_t n = 0) {
        if (n == 0) {
            cap_ = cap_ == 0 ? 2 : cap_ * 2;
        } else {
            cap_ = n;
        }
        T *dest = static_cast<T*>(malloc(cap_ * sizeof(T)));

        // Avoid constructor logic if type is trivial (i.e. int, char, ...)
        if constexpr (is_trivially_copyable<T>::value) {
            memcpy(dest, data_, size_ * sizeof(T));
        }
        else {
            for (size_t i = 0; i < size_; ++i) {
                new (dest + i) T(stl::move(data_[i]));
            }
            destroy(data_, data_ + size_);
        }
        if (data_) free(data_);
        data_ = dest;
    }
};


}   // namespace stl

#endif   // STL_VECTOR_HPP


/*
3. Still need to implement
  ┌─────────────────────────────┬─────────────────────────┐
  │           Method            │         Purpose         │
  ├─────────────────────────────┼─────────────────────────┤
  ├─────────────────────────────┼─────────────────────────┤
  │ emplace_back(Args&&...)     │ Construct in place      │
  ├─────────────────────────────┼─────────────────────────┤
  │ clear()                     │ Remove all elements     │
  ├─────────────────────────────┼─────────────────────────┤
  │ vector(initializer_list<T>) │ Brace initialization    │
  ├─────────────────────────────┼─────────────────────────┤

*/