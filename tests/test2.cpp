#include <memory>
#include <unordered_set>
#include <iostream>

class A {
public:
    A(int x) : x{x} {}
    int x;
};

int main() {
    std::shared_ptr<A> a(new A(5));
    std::shared_ptr<A> b = a;

    std::unordered_set<std::shared_ptr<A>> s;
    s.insert(a);
    std::cout << s.count(b);
}