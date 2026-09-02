// Minimal thread-safe deque I need for thread pool

#ifndef STL_THREADSAFE_DEQUE_HPP
#define STL_THREADSAFE_DEQUE_HPP

#include <condition_variable>
#include <mutex>
#include <optional>

#include "deque.hpp"

namespace stl {

template<typename T>
class threadsafe_deque{

    private:
        mutable std::mutex m_;
	    stl::deque<T> d_;

    public:
        std::optional<T> try_pop_front() {
            std::lock_guard<std::mutex> lock(m_);
            if (d_.empty()) {
                return std::nullopt;
            }
            T front = std::move(d_.front());
            d_.pop_front();
            return front;
            
        }
        void push_back(T t) {
            std::lock_guard<std::mutex> lock(m_);
            d_.push_back(t);
        }

		

};

}
#endif // STL_THREADSAFE_DEQUE_HPP
