
#include "stream.hpp"
#include <iostream>
#include <cassert>

using namespace std;

void testFrom() {
    auto t1 = Stream<int>::from(0);

    auto v1 = t1.take(20);
    assert(v1.size() == 20);
    for (int t : v1) assert(t == 0);

    auto v2 = t1.take(30);
    assert(v2.size() == 30);
    for (int t : v2) assert(t == 0);
}

void testMap() {
    auto t1 = Stream<int>::from(0)
                        .map([] (int x) { return x + 1; })
                        .map([] (int x) { return x + 1; });
    auto v1 = t1.take(20);
    for (int t : v1) assert(t == 2);
}

void testStep() {
    auto t1 = Stream<int>::from(0)
                        .step([] (int x) { return x + 1; });
    auto v1 = t1.take(20);
    int i = 0;
    for (int t : v1)  {
        assert(t == i);
        i = i + 1;
    }
}

void testFilter() {
    auto t1 = Stream<int>::from(0)
                        .step([] (int x) { return x + 1; })
                        .filter([] (int x) { return x % 3 == 0; });
    auto v1 = t1.take(20);
    for (int t : v1) {
        assert(t % 3 == 0);
    }
    assert(v1.at(19) == 57);
}

int main() {
    testFrom();
    testMap();
    testFilter();
    testStep();
    return 0;
}
