#ifndef STL_VECTOR_HPP
#define STL_VECTOR_HPP

#include <cstddef>
#include <exception>
#include <initializer_list>
#include <iostream>
#include <memory>

#include "algorithm.hpp"
#include "memory.hpp"
#include "type_traits.hpp"
#include "utility.hpp"

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
        init_data_();
    }

    // Construct with capacity and fill value
    vector(size_t cap, T &&value)
        : cap_(cap) {
        init_data_();
        size_ = cap;
        if constexpr (stl::is_trivially_constructible<T>::value) {
            memplace(data_, data_ + cap_, value);
        } else {
            stl::fill(data_, data_ + size_, value);
        }
    }

    vector(size_t cap, const T &value)
        : cap_(cap) {
        init_data_();
        size_ = cap;
        if constexpr (stl::is_trivially_constructible<T>::value) {
            memplace(data_, data_ + cap_, value);
        } else {
            stl::fill(data_, data_ + size_, value);
        }
    }

    // Copy constructor
    vector(const vector<T> &other)
        : cap_(other.cap_)
        , size_(other.size_) {
        init_data_();
        construct(data_, other.data_, size_);
        
    }

    // Move constructor
    vector(vector<T> &&other)
        : cap_(other.cap_)
        , size_(other.size_) {
        data_ = other.data_;
        other.data_ = nullptr;
    }

    // Initializer list constructor
    vector(std::initializer_list<T> init_list) {
        cap_ = init_list.size();
        size_ = 0;
        init_data_();

        for (const auto &v : init_list) {
            new (data_ + size_++) T(v);
        }
    }

    // Destructor
    ~vector() { release(data_, data_ + size_); }

    // Assignment operator which supports chaining
    vector &operator=(const vector &other) {
        if (this == &other) return *this;

        release(data_, data_ + size_);

        size_ = other.size_;
        cap_ = other.cap_;

        init_data_();
        construct(data_, other.data_, size_);
        
        return *this;
    }

    // Move assignment
    vector &operator=(vector &&other) {
        release(data_, data_ + size_);
        data_ = other.data_;
        size_ = other.size_;
        cap_ = other.cap_;
        other.data_ = nullptr;
        return *this;
    }

    // Reserve space for at least n elements
    void reserve(size_t n) {
        if (cap_ >= n) return;
        realloc_(n);
    }

    // Resize vector to n elements
    void resize(size_t n) {
        // Reallocate if necessary
        if (n > cap_) realloc_(n);

        // Free truncated space if necessary
        if (n < size_) {
            destroy(data_ + n, data_ + size_);
        }

        // Default construct in new space if necessary
        else if (n > size_) {
            fill(data_ + size_, data_ + n, T());
        }

        size_ = n;
    }

    // Random access operator
    T &operator[](size_t i) { return data_[i]; }

    const T &operator[](size_t i) const { return data_[i]; }

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
        if (cap_ == size_) [[unlikely]] {
            realloc_();
        }
        new (data_ + size_++) T(val);
    }

    void push_back(T &&val) {
        if (cap_ == size_) [[unlikely]] {
            realloc_();
        }
        new (data_ + size_++) T(stl::move(val));
    }

    void pop_back() {
        if constexpr (!stl::is_trivially_destructible<T>::value) {
            stl::destroy_at(data_ + size_ - 1);
        }
        size_--;
    }

    void clear() {
        destroy(data_, data_ + size_);
        size_ = 0;
    }

    bool empty() const { return size_ == 0; }

    bool operator==(const vector &other) const { return equals(this, other); }

    bool operator!=(const vector &other) const { return !(this == other); }


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
            realloc_(stl::max(cap_ * 2, size_after_insertion));
            loc = data_ + diff;
        }

        size_t front = size_ - 1;
        for (size_t back = size_after_insertion - 1; back >= size_; --back, --front) {
            data_[back] = stl::move(data_[front]);
        }
        T *res = loc;
        for (size_t i = 0; i < num_insertions; ++i) {
            *loc++ = val;
        }
        size_ += num_insertions;
        return res;
    }

    // Erase object at <loc>
    void erase(T *loc) {
        c_erase(loc, data_ + size_, 1);
        size_--;
    }

    // Erase objects from <start> to <end>
    void erase(T *start, T *end) {
        c_erase(start, data_ + size_, end - start);
        size_ -= end - start;
    }


private:
    T *data_;       // Underlying data
    size_t size_;   // number of elements
    size_t cap_;    // number of elements which can be held before reallocating


    // increase vector capacity, default cap_ * 2
    void realloc_(size_t n = 0) {
        if (n == 0) {
            cap_ = cap_ == 0 ? 2 : cap_ * 2;
        } else {
            cap_ = n;
        }
        T *dest = static_cast<T *>(malloc(cap_ * sizeof(T)));

        // Avoid constructor/destructor logic if type is trivial (i.e. int, char, ...)
        if constexpr (stl::is_trivially_copyable<T>::value) {
            memcpy(dest, data_, size_ * sizeof(T));
            free(data_);
        } else {
            for (size_t i = 0; i < size_; ++i) {
                new (dest + i) T(stl::move(data_[i]));
            }
            release(data_, data_ + size_);
        }
        data_ = dest;
    }

    void init_data_() { data_ = static_cast<T *>(malloc(cap_ * sizeof(T))); }
};


}   // namespace stl

#endif   // STL_VECTOR_HPP