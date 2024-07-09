#include <iostream>

template <typename T>
class A {
public:
    A(T x, T y) : x{x}, y{y} {}
    T x;
    T y;
};

template <typename T>
int operator*(const A<T>& a, const A<T>& b) { return a.x * b.y; }

class B {
public:
    B(int x) : x{x} {}
    int x;
};

int operator*(const B& a, const B& b) { return a.x * b.x; }

int main() {
    A a(B(5), B(2));
    A b(B(3), B(4));

    std::cout << a * b;
}