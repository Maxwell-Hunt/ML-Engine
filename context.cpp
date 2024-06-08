#include "context.h"
#include "variable.h"
#include "operations.h"
#include "tensor.h"
#include <ranges>

// TODO: Turn this class into a collection of functions

Expression Context::createVariable(float val) {
    Variable* v = new Variable(val, *this);
    return Expression(v);
}

Tensor<Expression> Context::createZeroTensor(const std::vector<std::size_t>& shape) {
    Tensor<Expression> t(shape);
    for(Expression& ex : t) ex = createVariable(0.f);
    return t;
}

Tensor<Expression> Context::createRandomTensor(const std::vector<std::size_t>& shape) {
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-1.5f, 1.5f);
    Tensor<Expression> t(shape);
    for(Expression& ex : t) ex = createVariable(dis(gen));
    return t;
}

Expression Context::add(const Expression& e1, const Expression& e2) {
    Addition* sum = new Addition(e1.getData(), e2.getData(), *this);
    return Expression(sum);
}

Expression Context::add(float val, const Expression& e2) {
    Expression e1 = createVariable(val);
    return add(e1, e2);
}

Expression Context::add(const Expression& e1, float val) {
    Expression e2 = createVariable(val);
    return add(e1, e2);
}

Expression Context::sub(const Expression& e1, const Expression& e2) {
    Subtraction* diff = new Subtraction(e1.getData(), e2.getData(), *this);
    return Expression(diff);
}

Expression Context::sub(float val, const Expression& e2) {
    Expression e1 = createVariable(val);
    return sub(e1, e2);
}

Expression Context::sub(const Expression& e1, float val) {
    Expression e2 = createVariable(val);
    return sub(e1, e2);
}

Expression Context::mult(const Expression& e1, const Expression& e2) {
    Multiplication* prod = new Multiplication(e1.getData(), e2.getData(), *this);
    return Expression(prod);
}

Expression Context::mult(float val, const Expression& e2) {
    Expression e1 = createVariable(val);
    return mult(e1, e2);
}

Expression Context::mult(const Expression& e1, float val) {
    Expression e2 = createVariable(val);
    return mult(e1, e2);
}

Expression Context::div(const Expression& e1, const Expression& e2) {
    Division* quotient = new Division(e1.getData(), e2.getData(), *this);
    return Expression(quotient);
}

Expression Context::div(float val, const Expression& e2) {
    Expression e1 = createVariable(val);
    return div(e1, e2);
}

Expression Context::div(const Expression& e1, float val) {
    Expression e2 = createVariable(val);
    return div(e1, e2);
}

Expression Context::square(const Expression& ex) {
    Square* sq = new Square(ex.getData(), *this);
    return Expression(sq);
}

Expression Context::reduceAdd(const Tensor<Expression>& expressions) {
    auto v = expressions.data() | std::views::transform([](const auto& e){ return e.getData(); });
    std::vector<std::shared_ptr<Internal::Expression>> temp(v.begin(), v.end());
    ReduceAdd* sum = new ReduceAdd(std::move(temp), *this);
    return Expression(sum);
}

float Context::computeGradients(const Expression& target, const Expression& source) {
    target.getData()->backPropagate();
    return source.getData()->getPartial();
}

Expression operator+(const Expression& e1, const Expression& e2) {
    return e1.getContext().add(e1, e2);
}

Expression operator+(const Expression& e1, float val) {
    return e1.getContext().add(e1, val);
}

Expression operator+(float val, const Expression& e2) {
    return e2.getContext().add(val, e2);
} 

Expression operator-(const Expression& e1, const Expression& e2) {
    return e1.getContext().sub(e1, e2);
}

Expression operator-(const Expression& e1, float val) {
    return e1.getContext().sub(e1, val);
}

Expression operator-(float val, const Expression& e2) {
    return e2.getContext().sub(val, e2);
} 

void operator+=(Expression& e1, const Expression& e2) {
    e1 = e1 + e2;
}

void operator-=(Expression& e1, const Expression& e2) {
    e1 = e1 - e2;
}

Expression operator*(const Expression& e1, const Expression& e2) {
    return e1.getContext().mult(e1, e2);
}

Expression operator*(const Expression& e1, float val) {
    return e1.getContext().mult(e1, val);
}

Expression operator*(float val, const Expression& e2) {
    return e2.getContext().mult(val, e2);
}

Expression operator/(const Expression& e1, const Expression& e2) {
    return e1.getContext().div(e1, e2);
}

Expression operator/(const Expression& e1, float val) {
    return e1.getContext().div(e1, val);
}

Expression operator/(float val, const Expression& e2) {
    return e2.getContext().div(val, e2);
}