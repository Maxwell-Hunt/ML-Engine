#include "context.h"
#include "variable.h"
#include "operations.h"

// TODO: the dot product function needs to be calculated

Expression Context::createVariable(float val) {
    Variable* v = new Variable(val, *this);
    return Expression(v);
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

float Context::computeGradients(const Expression& target, const Expression& source) {
    target.getData()->backPropagate();
    return source.getData()->getPartial();
}