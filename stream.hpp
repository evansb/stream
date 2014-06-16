/**
 * Stream.hpp
 * An implementation of simple "lazy list" or "stream" or "generators" using C++ Lambda functions
 * Evan Sebastian <evanlhoini@gmail.com>
 */

#ifndef _STREAM_HPP
#define _STREAM_HPP

#include <functional>
#include <vector>
#include <algorithm>

using namespace std;

// Two helper functions
template <class A, class B, class C>
static function<A(C)> compose(const function<C(B)> &f,
                              const function<B(A)> &g) {
    return bind(f, bind(g, placeholders::_1));
}

template <class A>
static function<bool(A)> composeAnd(const function<bool(A)> &f,
                                    const function<bool(A)> &g) {
    return [=] (A x) { return f(x) && g (x); };
}

template <class T>
class Stream {
    vector<T> memo; // Container of memoized data.
    int last; // Last index that has been processed.
    const T& seed; // First seed for the generator.

    // Call order : stepper -> post -> predicate
    // Element will be pushed iff predicate returns true
    function<T (T x)> stepper;
    function<bool (T x)> predicate;
    function<T (T x)> post;

    // Evaluate the stream until last == to
    void eval(const int to);

    // Disable ctors, use builder pattern only.
    Stream() {}
    Stream(const T& seed) :
        seed(seed),
        stepper([] (T x) { return x ;}),
        predicate([] (T x) { return true; }),
        post([] (T x) { return x; }) {}
public:
    static Stream from(const T&);

    Stream& step(const function<T (T x)>&); // Assign a stepper function.
    Stream& filter(const function<bool (T x)>&); // Add a filter function.
    Stream& map(const function<T (T x)>&); // Add a post-processing function.

    vector<T> take(const int); // Get first n elements, evaluate if necessary
    const T& at(const int); // Get one element, evaluate if necessary

    Stream(const Stream<T>& rhs) :
        memo(rhs.memo),
        last(rhs.last),
        seed(rhs.seed),
        stepper(rhs.stepper),
        predicate(rhs.predicate),
        post(rhs.post)
    {}

    Stream& operator=(const Stream<T>&) { return Stream(this); }
    ~Stream() {};
};

template <class T>
Stream<T> Stream<T>::from(const T &seed) {
    Stream<T> newStream(seed);
    return newStream;
}

template <class T>
Stream<T>& Stream<T>::step(const function<T (T x)> &stepper) {
    this->stepper = compose<T,T,T>(stepper, this->stepper);
    return (*this);
}

template <class T>
Stream<T>& Stream<T>::map(const function <T (T x)> &post) {
    this->post = compose<T,T,T>(post, this->post);
    return (*this);
}

template <class T>
Stream<T>& Stream<T>::filter(const function <bool (T x)> &predicate) {
    this->predicate = composeAnd<T>(predicate, this->predicate);
    return (*this);
}

template <class T>
void Stream<T>::eval(int to) {
    if (last >= to) return;
    T last_value = last > 0 ? memo[last] : seed;
    while (last < to) {
        T p_last_value = post(last_value);
        if (predicate(p_last_value)) {
            memo.push_back(p_last_value);
            last = last + 1;
        }
        last_value = stepper(last_value);
    }
}

template <class T>
vector<T> Stream<T>::take(const int n) {
    eval(n);
    vector<T> retval(begin(memo), begin(memo) + n);
    return retval;
}

template <class T>
const T& Stream<T>::at(const int n) {
    if (last < n) { eval(n); }
    return memo.at(n);
}

#endif
