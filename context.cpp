#include "context.h"

Context::~Context() {
        for(const Internal::Expression* ex : expressions) delete ex;
}

Expression Context::createVariable(float val) {
    Variable* v = new Variable(val, *this);
    expressions.insert(v);
    return *v;
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

void Context::checkExpressions(const Expression& e1, const Expression& e2) const {
    if(!(expressions.count(&e1.data()) && expressions.count(&e2.data()))) {
        throw std::runtime_error("WHY ARE YOU DOING THIS???");
    }
}