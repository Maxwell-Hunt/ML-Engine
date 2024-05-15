#include <memory>
#include <string>
#include <iostream>

class Base {
public:
    virtual void print() = 0;
};

class A : public Base {
friend class C;
public:
    virtual void print() override { std::cout << name << '\n'; }
private:
    A(std::string name) : name{name} {}
    std::string name;
};

class Append : public Base {
friend class C;
public:
    virtual void print() override { first->print(); second->print(); }
private:
    Append(std::shared_ptr<Base> first, std::shared_ptr<Base> second) : first{first}, second{second} {}
    std::shared_ptr<Base> first;
    std::shared_ptr<Base> second;
};

class B {
public:
    B(Base* a) : a{a} {} 
    B& operator=(const B& other) = delete;
    B& operator=(B&& other) {a = std::move(other.a); return *this;}
    void print() { a->print(); }
    std::shared_ptr<Base> getData() const { return a; }
private:
    std::shared_ptr<Base> a;
};

class C {
public:
    B createB(std::string name) {
        Base* thing = new A(name);
        return B(thing);
    }

    B append(const B& first, const B& second) {
        Base* appended = new Append(first.getData(), second.getData());
        return B(appended);
    }
};

int main() {
    C c;
    B x = c.createB("hi");
    B y = c.createB("hello");
    x = c.append(x, x);
    y = c.append(x, y);
    y.print();
}