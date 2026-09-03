#ifndef STL_THREADPOOL_HPP
#define STL_THREADPOOL_HPP

#include <functional>
#include <thread>

#include "vector.hpp"

namespace stl {


class Threadpool {

public:
    
    Threadpool() {

    }

    ~Threadpool() {

    }
    
    void enqueue_task(std::function<void()> task) {
        
    }

private:
    size_t num_threads_ = 1;
    stl::vector<std::thread> threads;  
    
};



}





#endif // STL_THREADPOOL_HPP
