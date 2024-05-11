#include "context.h"

// TODO: the dot product function needs to be calculated

Context::~Context() {
        for(const Internal::Expression* ex : expressions) delete ex;
}

Expression Context::createVariable(float val) {
    Variable* v = new Variable(val, *this);
    expressions.insert(v);
    return Expression(*v);
}

Vector Context::createVector(std::size_t size, bool isZero) {
    return Vector(size, isZero, *this);
}

Vector Context::createVector(std::size_t size, float value) {
    return Vector(size, value, *this);
}

Expression Context::add(const Expression& e1, const Expression& e2) {
    checkExpressions(e1, e2);

    Addition* sum = new Addition(e1.data(), e2.data(), *this);
    expressions.insert(sum);
    return Expression(*sum);
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
    checkExpressions(e1, e2);
    Subtraction* diff = new Subtraction(e1.data(), e2.data(), *this);
    expressions.insert(diff);
    return Expression(*diff);
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
    checkExpressions(e1, e2);
    Multiplication* prod = new Multiplication(e1.data(), e2.data(), *this);
    expressions.insert(prod);
    return Expression(*prod);
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
    checkExpressions(e1, e2);
    Division* quotient = new Division(e1.data(), e2.data(), *this);
    expressions.insert(quotient);
    return Expression(*quotient);
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
    Square* sq = new Square(ex.data(), *this);
    expressions.insert(sq);
    return Expression(*sq);
}

Vector Context::add(const Vector& a, const Vector& b) {
    checkVectors(a, b);
    return Vector(vb.add(a, b), *this);
}

Vector Context::sub(const Vector& a, const Vector& b) {
    checkVectors(a, b);
    return Vector(vb.sub(a, b), *this);
}

Vector Context::mult(const Expression& a, const Vector& b) {
    return Vector(vb.mult(a, b), *this);
}


Vector Context::mult(const Vector& a, const Expression& b) {
    return mult(b, a);
}

Vector Context::div(const Vector& a, const Expression& b) {
    return Vector(vb.div(a, b), *this);
}

Expression Context::dot(const Vector& a, const Vector& b) {
    return vb.dot(a, b);
}

void Context::checkExpressions(const Expression& e1, const Expression& e2) const {
    if(!(expressions.count(&e1.data()) && expressions.count(&e2.data()))) {
        throw std::runtime_error("WHY ARE YOU DOING THIS???");
    }
}

void Context::checkVectors(const Vector& a, const Vector& b) const {
    if(a.getSize() != b.getSize() || a.getSize() == 0) {
        throw std::runtime_error("Vectors must have same size");
    }
}