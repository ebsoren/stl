#ifndef STL_UTILITY_HPP
#define STL_UTILITY_HPP

#include <initializer_list>

#include "type_traits.hpp"


namespace stl {
/*

    move cast
    Remove any possible reference from T, then make it an rvalue reference

    marking move as noexcept means that container operations which would like to be able to
    roll back to previous state on failure (for example, vector::resize()) can safely use move,
    instead of relying on slower operations. Also, compilers can avoid the (small) overhead in stack
    unwinding needed for operations that may throw an exception.

*/
template <typename T>
remove_reference_t<T> &&move(T &&arg) noexcept {
    return static_cast<remove_reference_t<T> &&>(arg);
}

/*

    forwarding
    When writing a wrapper, the inner function needs to know whether its passed parameter is an lvalue
    or an rvalue outside of the wrapper, in order to avoid unnecessary copies of rvalues. For example:

    wrapper(T&& arg) {
        func(arg)
    }

    if wrapper(10) is called without forwarding, func actually receives a named argument <arg> which is an lvalue.
    instead, wrapper should be defined:
    wrapper(T&& arg) {
        func(forward<T>(arg));
    }

*/

template <typename T>
T &&forward(remove_reference_t<T> &arg) noexcept {
    return static_cast<T &&>(arg);
}

template <typename T>
T &&forward(remove_reference_t<T> &&arg) noexcept {
    return static_cast<T &&>(arg);
}


/*

    swap
    trivial and very useful
    Use of move operator avoids unnecessary copies.

*/

template <typename T>
void swap(T &a, T &b) noexcept {
    T temp = move(a);
    a = move(b);
    b = move(temp);
}

template <typename T>
void copy_swap(T &a, T &b) noexcept {
    T temp = a;
    a = b;
    b = a;
}


/*

    pair
    Useful, although I hate the naming convention.
    Trivial implemtation for now, may add methods as needed


*/
template <typename T, typename U>
struct pair {
    T first;
    U second;

    pair() = default;

    // Necessary to template constructor because argument l/rvalue deduction used by forward
    // only applies during template argument deduction
    template <typename T2, typename U2>
    pair(T2 &&f, U2 &&s)
        : first(forward<T2>(f))
        , second(forward<U2>(s)) {}
};


template <typename T>
void max(T &&f, T &&s) {
    return f > s ? f : s;
}

template <typename T>
void min(T &&f, T &&s) {
    return !max(f, s);
}


}   // namespace stl

#endif