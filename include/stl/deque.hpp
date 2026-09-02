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

// Enum class to specify how to construct a node. A single node should be MID constructed,
// while a node added to the front of the list (or back) should also be specified.
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
template <typename T, size_t BUFFER_SIZE = 512>
class deque {
    // Node in deque's linked list
    struct node {
        T *data_ = nullptr;

        // Front of buffer where a push_front would allocate
        size_t buf_front_ = 0;

        // push_bacl location
        size_t buf_back_ = 0;

        // Next node in the list
        node *next_ = nullptr;

        // Previous node in the list
        node *prev_ = nullptr;

        // Allocate memory for the buffer
        void init_data_() { stl::init(data_, BUFFER_SIZE); }

        // Construct a node and specify whether data pointers should be centered, left, or right on the buffer
        node(NodeType type) {
            switch (type) {
            // Node added to the front of the list should allocate all space at the front.
            case NodeType::FRONT:
                buf_front_ = BUFFER_SIZE - 1;
                buf_back_ = BUFFER_SIZE - 1;
                break;

            // Node added to the back of the list should allocate all space at the back.
            case NodeType::BACK:
                buf_front_ = 0;
                buf_back_ = 0;
                break;

            // Individual node should allow both push_front and push_back without allocating another node.
            case NodeType::MID:
                buf_front_ = BUFFER_SIZE / 2;
                buf_back_ = BUFFER_SIZE / 2;
                break;
            }
            init_data_();
        }

        // Create a node from existing data.
        node(T *src, size_t size, NodeType type) {
            init_data_();
            if (type == NodeType::MID) {
                buf_front_ = (BUFFER_SIZE - size) / 2;
                buf_back_ = buf_front_ + size;
            } else {
                buf_front_ = 0;
                buf_back_ = size;
            }
            construct(data_ + buf_front_, src, size);
        }

        // Destructor
        ~node() {
            stl::destroy(data_ + buf_front_, data_ + buf_back_);
            free(data_);
        }

        /* Various status functions */

        bool empty() { return buf_front_ == buf_back_; }

        bool full_back() { return buf_back_ == BUFFER_SIZE; }
        bool full_front() { return buf_front_ == 0; }


        void push_frontbuf(const T &val) {
            --buf_front_;
            stl::construct_at(data_ + buf_front_, val);
        }

        void push_frontbuf(T &&val) {
            --buf_front_;
            new (data_ + buf_front_) T(stl::move(val));
        }

        void push_backbuf(const T &val) {
            stl::construct_at(data_ + buf_back_, val);
            ++buf_back_;
        }

        void push_backbuf(T &&val) {
            new (data_ + buf_back_) T(stl::move(val));
            ++buf_back_;
        }

        void pop_frontbuf() { stl::destroy_at(data_ + buf_front_++); }
        void pop_backbuf() { stl::destroy_at(data_ + (--buf_back_)); }
    };   // node


    // Front of the deque
    node *front_;

    // Back of the deque
    node *back_;

    size_t cap_;
    size_t size_;

    // Add a new node to the back of the list
    void add_back() {
        back_->next_ = new node(NodeType::BACK);
        back_->next_->prev_ = back_;
        back_ = back_->next_;
    }

    void add_back(T *data, size_t size) {
        back_->next_ = new node(data, size, NodeType::BACK);
        back_->next_->prev_ = back_;
        back_ = back_->next_;
    }

    // Add a new node to the front of the list
    void add_front() {
        front_->prev_ = new node(NodeType::FRONT);
        front_->prev_->next_ = front_;
        front_ = front_->prev_;
    }

public:

    // default constructor
    deque()
        : front_(nullptr)
        , back_(nullptr)
        , cap_(0)
        , size_(0) {}

    // range constructor
    deque(T *start, T *end)
        : front_(nullptr)
        , back_(nullptr)
        , size_(end - start)
        , cap_(end - start - ((end - start) % BUFFER_SIZE) + BUFFER_SIZE) {
        while (start < end) {
            size_t chunk = (size_t) (end - start) < BUFFER_SIZE ? (size_t) (end - start) : BUFFER_SIZE;
            if (!front_) {
                front_ = new node(start, chunk, NodeType::MID);
                back_ = front_;
            } else {
                add_back(start, chunk);
            }
            start += chunk;
        }
    }

    // Destructor
    ~deque() {
        node *curr = front_;
        while (curr) {
            node *next = curr->next_;
            delete curr;
            curr = next;
        }
    }

    // push to the front of the deque
    void push_front(const T &val) {
        if (!front_) {
            front_ = new node(NodeType::MID);
            back_ = front_;
        }
        if (front_->full_front()) {
            add_front();
        }
        front_->push_frontbuf(val);
        size_++;
    }

    void push_front(T &&val) {
        if (!front_) {
            front_ = new node(NodeType::MID);
            back_ = front_;
        }
        if (front_->full_front()) {
            add_front();
        }
        front_->push_frontbuf(stl::move(val));
        size_++;
    }

    // Push to the back of the deque
    void push_back(const T &val) {
        if (!front_) {
            front_ = new node(NodeType::MID);
            back_ = front_;
        }
        if (back_->full_back()) {
            add_back();
        }
        back_->push_backbuf(val);
        size_++;
    }

    void push_back(T &&val) {
        if (!front_) {
            front_ = new node(NodeType::MID);
            back_ = front_;
        }
        if (back_->full_back()) {
            add_back();
        }
        back_->push_backbuf(stl::move(val));
        size_++;
    }

    // Pop from the front of the deque
    void pop_front() {
        front_->pop_frontbuf();
        if (front_->empty() && front_ != back_) {
            node *old = front_;
            front_ = front_->next_;
            front_->prev_ = nullptr;
            delete old;
        }
        size_--;
    }

    // Pop from the back of the deque
    void pop_back() {
        back_->pop_backbuf();
        if (back_->empty() && front_ != back_) {
            node *old = back_;
            back_ = back_->prev_;
            back_->next_ = nullptr;
            delete old;
        }
        size_--;
    }

    bool empty() {
        return !size_;
    }

    // Return a reference to the front of the queue
    T &front() {
        return *(front_->data_ + front_->buf_front_);
    }
};

}   // namespace stl


#endif   // STL_DEQUE_HPP
