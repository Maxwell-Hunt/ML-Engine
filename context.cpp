#include "context.h"

Context::~Context() {
        for(const Expression* ex : expressions) delete ex;
}

const Expression& Context::createVariable(float val) {
    Variable* v = new Variable(val, *this);
    expressions.insert(v);
    return *v;
}

const Expression& Context::add(const Expression& e1, const Expression& e2) {
    checkExpressions(e1, e2);

    Addition* sum = new Addition(e1, e2, *this);
    expressions.insert(sum);
    return *sum;
}

const Expression& Context::add(float val, const Expression& e2) {
    const Expression& e1 = createVariable(val);
    return add(e1, e2);
}

const Expression& Context::add(const Expression& e1, float val) {
    const Expression& e2 = createVariable(val);
    return add(e1, e2);
}

const Expression& Context::sub(const Expression& e1, const Expression& e2) {
    checkExpressions(e1, e2);
    Subtraction* diff = new Subtraction(e1, e2, *this);
    expressions.insert(diff);
    return *diff;
}

const Expression& Context::sub(float val, const Expression& e2) {
    const Expression& e1 = createVariable(val);
    return sub(e1, e2);
}

const Expression& Context::sub(const Expression& e1, float val) {
    const Expression& e2 = createVariable(val);
    return sub(e1, e2);
}

const Expression& Context::mult(const Expression& e1, const Expression& e2) {
    checkExpressions(e1, e2);
    Multiplication* prod = new Multiplication(e1, e2, *this);
    expressions.insert(prod);
    return *prod;
}

const Expression& Context::mult(float val, const Expression& e2) {
    const Expression& e1 = createVariable(val);
    return mult(e1, e2);
}

const Expression& Context::mult(const Expression& e1, float val) {
    const Expression& e2 = createVariable(val);
    return mult(e1, e2);
}

const Expression& Context::div(const Expression& e1, const Expression& e2) {
    checkExpressions(e1, e2);
    Division* quotient = new Division(e1, e2, *this);
    expressions.insert(quotient);
    return *quotient;
}

const Expression& Context::div(float val, const Expression& e2) {
    const Expression& e1 = createVariable(val);
    return div(e1, e2);
}

const Expression& Context::div(const Expression& e1, float val) {
    const Expression& e2 = createVariable(val);
    return div(e1, e2);
}

const Expression& Context::square(const Expression& ex) {
    Square* sq = new Square(ex, *this);
    expressions.insert(sq);
    return *sq;
}

void Context::checkExpressions(const Expression& e1, const Expression& e2) const {
    if(!(expressions.count(&e1) && expressions.count(&e2))) {
        throw std::runtime_error("WHY ARE YOU DOING THIS???");
    }
}