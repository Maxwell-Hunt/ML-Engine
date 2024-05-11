#include "vector.h"
#include "context.h"

#include <random>

Vector::Vector(std::size_t size, bool zeros, Context& context) : size{size}, data{std::vector<Expression>{size}}, context{context} {
    if(zeros) {
        for(Expression& ex : data) ex = context.createVariable(0);
    } else {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(-1, 1);
        for(Expression& ex : data) ex = context.createVariable(dis(gen));
    }
}

Vector::Vector(std::vector<Expression>&& data, Context& context) : size{data.size()}, data{std::move(data)}, context{context} {}

Vector::Vector(std::size_t size, float value, Context& context) : size{size}, context{context} {
    for(std::size_t i = 0;i < size;i++) {
        data.push_back(context.createVariable(value));
    }
}

const Expression& Vector::at(std::size_t index) const {
    return data.at(index);
}

std::size_t Vector::getSize() const { return size; }

Context& Vector::getContext() const { return context; }

std::vector<Expression>::iterator Vector::begin() {
    return data.begin();
}

std::vector<Expression>::iterator Vector::end() {
    return data.end();
}

std::vector<Expression> VectorBehaviour::add(const Vector& a, const Vector& b) const {
    std::vector<Expression> v;
    for(std::size_t i = 0;i < a.getSize();i++) {
        v.push_back(a.at(i) + b.at(i));
    }

    return v;
}

std::vector<Expression> VectorBehaviour::sub(const Vector& a, const Vector& b) const {
    std::vector<Expression> v;
    for(std::size_t i = 0;i < a.getSize();i++) {
        v.push_back(a.at(i) - b.at(i));
    }

    return v;
}

std::vector<Expression> VectorBehaviour::mult(const Expression& a, const Vector& b) const {
    std::vector<Expression> v;
    for(std::size_t i = 0;i < b.getSize();i++) {
        v.push_back(b.at(i) * a);
    }

    return v;
}

std::vector<Expression> VectorBehaviour::div(const Vector& a, const Expression& b) const {
    std::vector<Expression> v;
    for(std::size_t i = 0;i < a.getSize();i++) {
        v.push_back(a.at(i) / b);
    }

    return v;
}

// TODO: Change the way this is implemented.  As is, it allocates way more objects than
//       are necessary.  This might require some structural changes to the project.  I.e.
//       we may have to introduce some operation classs which handle arbitrary numbers of operands
Expression VectorBehaviour::dot(const Vector& a, const Vector& b) const {
    Expression result = a.at(0) * b.at(0);
    for(std::size_t i = 1;i < a.getSize();i++) {
        result = result + a.at(i) * b.at(i);
    }
    return result;
}

Vector operator+(const Vector& a, const Vector& b) {
    return a.getContext().add(a, b);
} 

Vector operator-(const Vector& a, const Vector& b) {
    return a.getContext().sub(a, b);
}

Vector operator*(const Vector& a, const Expression& b) {
    return a.getContext().mult(a, b);
}

Vector operator*(const Expression& a, const Vector& b) {
    return a.getContext().mult(a, b);
}

Vector operator/(const Vector& a, const Expression& b) {
    return a.getContext().div(a, b);
}