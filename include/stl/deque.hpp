#ifndef STL_DEQUE_HPP
#define STL_DEQUE_HPP

#include <cstddef>
#include <exception>
#include <initializer_list>
#include <iostream>
#include <memory>

#include "algorithm.hpp"
#include "memory.hpp"
#include "type_traits.hpp"
#include "utility.hpp"

static constexpr unsigned int BUFFER_SIZE = 512;


enum class NodeType {
    FRONT = 0,
    MID = 1,
    BACK = 2,
};

namespace stl {


// deque supports efficient push() and pop() operations by maintaining a non-contigious linked list
// of buffers. On an insert, either add to the last buffer or, if full, allocate a new one.
// On a delete, either delete from the first buffer, or if only one element remains,
// delete the buffer entirely.
template <typename T>
class deque {
    // Node in deque's linked list
    class node {
        char *data_ = nullptr;
        size_t buf_front_ = 0;
        size_t buf_back_ = 0;
        node *next_ = nullptr;
        node *prev_ = nullptr;
        void init_data_() { data_ = static_cast<T *>(malloc(BUFFER_SIZE * sizeof(T))); }


    public:
        node(NodeType type) {
            switch (type) {
            case NodeType::FRONT:
                buf_front_ = BUFFER_SIZE - 1;
                buf_back_ = BUFFER_SIZE - 1;

            case NodeType::BACK:
                buf_front_ = 0;
                buf_back_ = 0;

            case NodeType::MID:
                buf_front_ = BUFFER_SIZE / 2;
                buf_back_ = BUFFER_SIZE / 2;
            }
            init_data_();
        }

        node(T *src, size_t size, NodeType type) {
            init_data_();
            if (type == NodeType::MID) {
                buf_front_ = (BUFFER_SIZE - size) / 2;
                buf_back_ = buf_front_ + size;
            } else {
                buf_front_ = 0;
                buf_back_ = size - 1;
            }
            construct(data_ + buf_front_, src, size);
        }

        ~node() { release(data_, data_ + BUFFER_SIZE); }

        bool empty() { return buf_front_ = buf_back_; }

        bool full_back() { return buf_back_ == BUFFER_SIZE - 1; }
        bool full_front() { return buf_front_ == 0; }

        void push_frontbuf(const T &val) { data_[--buf_front_] = val; }

        void push_frontbuf(T &&val) { data_[--buf_front_] = val; }

        void push_backbuf(const T &val) { data_[++buf_back_] = val; }

        void push_backbuf(T &&val) { data_[++buf_back_] = val; }

        void pop_frontbuf() { destroy_at(data_[++buf_front_]); }
        void pop_backbuf() { destroy_at(data_[--buf_back_]); }
    };

    // Front of the data list
    node *front_;

    // Back of the data list
    node *back_;

    // Global capacity and size
    size_t cap_;
    size_t size_;


    void add_back(T *data, size_t size) {
        back_->next_ = node(data, size, NodeType::BACK);
        back_->next_->prev_ = back_;
        back_ = back_->next;
    }
    void add_back() {
        back_->next_ = node(NodeType::BACK);
        back_->next_->prev_ = back_;
        back_ = back_->next;
    }

    void add_front() {
        front_->prev_ = node(NodeType::FRONT);
        front_->prev_->next_ = front_;
        front_ = front_->prev_;
    }

public:
    // Default constructor
    deque() {
        front_ = nullptr;
        back_ = nullptr;
    }

    // Contiguous data constructor
    deque(const T *start, const T *end)
        : size_(end - start)
        , cap_(end - start - ((end - start) % BUFFER_SIZE) + BUFFER_SIZE) {
        while (end - start >= 0) {
            if (!front_) {
                front_ = node(start, end - start, NodeType::MID);
                back_ = front_;
            } else {
                add_back(start, start - end);
            }
            start += BUFFER_SIZE;
        }
    }

    // Destroy everything between front and back. front_ and back_
    // constructors will run by default
    ~deque() {
        while (front_ != back_) {
            front_ = front_->next;
            node *next = front_->next;
            front_->~node();
            front_ = next;
        }
        back_ = nullptr;   // Avoid double free since front == back now.
    }

    void push_front(const T &val) {
        if (front_->full_front()) {
            add_front();
        }
        front_->pfront(val);
    }

    void push_front(T &&val) {
        if (front_->full_front()) {
            add_front();
        }
        front_->pfront(val);
    }

    void push_back(const T &val) {
        if (back_->full()) {
            add_back();
        }
        back_->pback(val);
        size_++;
    }

    void push_back(T &&val) {
        if (back_->full()) {
            add_back();
        }
        back_->pback(val);
        size_++;
    }

    void pop_front() {
        if (front_->empty()) {
            front_ = front_->next_;
        }
        front_->pop_frontbuf;
    }

    void pop_back() {
        if (back_->empty()) {
            back_ = back_->prev_;
        }
        back_->pop_backbuf;
    }
};

}   // namespace stl


#endif   // STL_DEQUE_HPP